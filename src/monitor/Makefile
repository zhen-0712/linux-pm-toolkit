CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c11 -g
TARGET  = pm_monitor

SRCS    = main.c sysfs_utils.c cpufreq.c cpuidle.c thermal.c output.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

# Header dependencies
main.o:       main.c sysfs_utils.h cpufreq.h cpuidle.h thermal.h output.h
sysfs_utils.o: sysfs_utils.c sysfs_utils.h
cpufreq.o:    cpufreq.c cpufreq.h sysfs_utils.h
cpuidle.o:    cpuidle.c cpuidle.h sysfs_utils.h
thermal.o:    thermal.c thermal.h sysfs_utils.h
output.o:     output.c output.h cpufreq.h cpuidle.h thermal.h
