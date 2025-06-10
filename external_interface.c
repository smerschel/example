#include "external_interface.h"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define EXT_BUFFER_SIZE 1024

static int tx_socket = -1;
static int rx_socket = -1;
static struct sockaddr_in tx_addr;
static struct sockaddr_in rx_addr;
static u8 ext_buffer[EXT_BUFFER_SIZE];
static CircBuf ext_circ_buf;
static bool is_initialized = false;

// Helper function to set socket to non-blocking mode
static ReturnCode set_nonblocking(int sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) {
        return ERROR_HARDWARE;
    }
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
        return ERROR_HARDWARE;
    }
    return SUCCESS;
}

ReturnCode EXT_Init(void) {
    if (is_initialized) {
        return SUCCESS;
    }

    // Initialize circular buffer
    ReturnCode result = CIR_Init(&ext_circ_buf, ext_buffer, EXT_BUFFER_SIZE);
    if (result != SUCCESS) {
        return result;
    }

    // Create TX socket
    tx_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (tx_socket < 0) {
        return ERROR_HARDWARE;
    }

    // Create RX socket
    rx_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (rx_socket < 0) {
        close(tx_socket);
        return ERROR_HARDWARE;
    }

    // Set sockets to non-blocking mode
    if (set_nonblocking(tx_socket) != SUCCESS || 
        set_nonblocking(rx_socket) != SUCCESS) {
        close(tx_socket);
        close(rx_socket);
        return ERROR_HARDWARE;
    }

    // Configure TX address
    memset(&tx_addr, 0, sizeof(tx_addr));
    tx_addr.sin_family = AF_INET;
    tx_addr.sin_port = htons(EXT_TX_PORT);
    tx_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Configure RX address
    memset(&rx_addr, 0, sizeof(rx_addr));
    rx_addr.sin_family = AF_INET;
    rx_addr.sin_port = htons(EXT_RX_PORT);
    rx_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind RX socket
    if (bind(rx_socket, (struct sockaddr*)&rx_addr, sizeof(rx_addr)) < 0) {
        close(tx_socket);
        close(rx_socket);
        return ERROR_HARDWARE;
    }

    is_initialized = true;
    return SUCCESS;
}

ReturnCode EXT_Send(const u8* data, u32 length) {
    if (!is_initialized || data == NULL || length == 0) {
        return ERROR_INVALID_PARAMETER;
    }

    ssize_t sent = sendto(tx_socket, data, length, 0,
                         (struct sockaddr*)&tx_addr, sizeof(tx_addr));
    
    if (sent < 0) {
        return ERROR_COMMUNICATION;
    }
    
    return SUCCESS;
}

i32 EXT_Recv(u8* buffer, u32 max_length) {
    if (!is_initialized || buffer == NULL || max_length == 0) {
        return -1;
    }
    
    i32 bytes_read = 0;
    u8 data;
    
    // Read bytes from circular buffer
    while (bytes_read < max_length) {
        if (CIR_Get(&ext_circ_buf, &data) != SUCCESS) {
            break;
        }
        buffer[bytes_read++] = data;
    }
    
    return bytes_read;
}

void EXT_Task(void) {
    if (!is_initialized) {
        return;
    }

    // Try to receive data from socket
    u8 recv_buffer[256];
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    ssize_t received = recvfrom(rx_socket, recv_buffer, sizeof(recv_buffer), 0,
                               (struct sockaddr*)&client_addr, &client_len);

    if (received > 0) {
        // Put received data into circular buffer
        for (ssize_t i = 0; i < received; i++) {
            if (CIR_Put(&ext_circ_buf, recv_buffer[i]) != SUCCESS) {
                break;  // Buffer is full
            }
        }
    }
}
