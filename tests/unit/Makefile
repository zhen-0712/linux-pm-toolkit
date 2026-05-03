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
TESTS = test_cpufreq test_thermal test_cpuidle

.PHONY: all run_tests clean

all: $(TESTS)

test_cpufreq: test_cpufreq.c $(MONITOR_SRCS)
	$(CC) $(CFLAGS) -DMOCK_SYSFS_ROOT=\"mock_sysfs\" -o $@ $^

test_thermal: test_thermal.c $(MONITOR_SRCS)
	$(CC) $(CFLAGS) -DMOCK_SYSFS_ROOT=\"mock_sysfs\" -o $@ $^

test_cpuidle: test_cpuidle.c $(MONITOR_SRCS)
	$(CC) $(CFLAGS) -DMOCK_SYSFS_ROOT=\"mock_sysfs\" -o $@ $^

run_tests: all
	@echo "--- test_cpufreq ---" && ./test_cpufreq
	@echo "--- test_thermal ---" && ./test_thermal
	@echo "--- test_cpuidle ---" && ./test_cpuidle

clean:
	rm -f $(TESTS)
