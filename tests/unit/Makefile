CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g -I../../src/monitor
MONITOR_SRCS = ../../src/monitor/sysfs_utils.c \
               ../../src/monitor/cpufreq.c \
               ../../src/monitor/cpuidle.c \
               ../../src/monitor/thermal.c \
               ../../src/monitor/output.c

# Tests added per branch:
#   feature/userspace-cpufreq → test_cpufreq
#   feature/userspace-thermal → test_thermal, test_cpuidle
TESTS = test_cpufreq

.PHONY: all run_tests clean

all: $(TESTS)

test_cpufreq: test_cpufreq.c $(MONITOR_SRCS)
	$(CC) $(CFLAGS) -DMOCK_SYSFS_ROOT=\"tests/unit/mock_sysfs\" -o $@ $^

run_tests: all
	@echo "--- test_cpufreq ---" && ./test_cpufreq

clean:
	rm -f $(TESTS)
