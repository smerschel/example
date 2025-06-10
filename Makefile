CC = gcc
CFLAGS = -Wall -Wextra -I.
LDFLAGS = 

# Coverage flags
COVERAGE_CFLAGS = -fprofile-arcs -ftest-coverage
COVERAGE_LDFLAGS = -lgcov

# Default target is x86
all: x86

# x86 target
x86: CFLAGS += -DPLATFORM_X86
x86: main.o external_interface.o circ_buf.o command_manager.o bare_mutex.o sys_time.o cvt.o protocol.o telemetry_manager.o packet.o
	$(CC) $(LDFLAGS) -o program $^

# Baremetal target
baremetal: CFLAGS += -DPLATFORM_BAREMETAL
baremetal: main.o external_interface.o circ_buf.o command_manager.o bare_mutex.o sys_time.o cvt.o protocol.o telemetry_manager.o packet.o
	$(CC) $(LDFLAGS) -o program_baremetal $^

# Test targets with coverage
test_circ_buf: CFLAGS += -DPLATFORM_X86 $(COVERAGE_CFLAGS)
test_circ_buf: test_circ_buf.o circ_buf.o bare_mutex.o
	$(CC) $(LDFLAGS) $(COVERAGE_LDFLAGS) -o test_circ_buf $^
	./test_circ_buf

test_command_manager: CFLAGS += -DPLATFORM_X86 $(COVERAGE_CFLAGS)
test_command_manager: test_command_manager.o command_manager.o circ_buf.o external_interface.o bare_mutex.o sys_time.o cvt.o protocol.o packet.o
	$(CC) $(LDFLAGS) $(COVERAGE_LDFLAGS) -o test_command_manager $^
	./test_command_manager

test_protocol: CFLAGS += -DPLATFORM_X86 $(COVERAGE_CFLAGS)
test_protocol: test_protocol.o protocol.o packet.o
	$(CC) $(LDFLAGS) $(COVERAGE_LDFLAGS) -o test_protocol $^
	./test_protocol

test_cvt: CFLAGS += -DPLATFORM_X86 $(COVERAGE_CFLAGS)
test_cvt: test_cvt.o cvt.o sys_time.o
	$(CC) $(LDFLAGS) $(COVERAGE_LDFLAGS) -o test_cvt $^
	./test_cvt

test_sys_time: CFLAGS += -DPLATFORM_X86 $(COVERAGE_CFLAGS)
test_sys_time: test_sys_time.o sys_time.o
	$(CC) $(LDFLAGS) $(COVERAGE_LDFLAGS) -o test_sys_time $^
	./test_sys_time

test: test_circ_buf test_command_manager test_protocol test_cvt test_sys_time

# Coverage report target
coverage: test
	@echo "Generating coverage report..."
	@for file in *.c; do \
		if [ -f $$file ]; then \
			gcov -b $$file; \
		fi \
	done
	@echo "Coverage report generated. Check *.gcov files for details."

# Object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f *.o program program_baremetal test_circ_buf test_command_manager test_protocol test_cvt test_sys_time
	rm -f *.gcno *.gcda *.gcov

.PHONY: all x86 baremetal test test_circ_buf test_command_manager test_protocol test_cvt test_sys_time clean coverage 