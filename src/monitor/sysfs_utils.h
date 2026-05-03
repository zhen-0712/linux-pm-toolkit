#ifndef SYSFS_UTILS_H
#define SYSFS_UTILS_H

#include <stddef.h>

/*
 * Abstraction over sysfs file reads.
 * Production code uses sysfs_read_file(); unit tests inject a mock via
 * the sysfs_reader_fn pointer to read from a local mock_sysfs/ tree.
 */

/* Returns 0 on success, -1 on error. Buf is null-terminated, trailing newline stripped. */
typedef int (*sysfs_reader_fn)(const char *path, char *buf, size_t len);

/* Default production reader */
int sysfs_read_file(const char *path, char *buf, size_t len);

/* Convenience: read a single long integer value from path */
int sysfs_read_long(sysfs_reader_fn reader, const char *path, long *out);

/* Convenience: read a trimmed string value from path */
int sysfs_read_str(sysfs_reader_fn reader, const char *path, char *out, size_t len);

/* Count entries matching /sys/devices/system/cpu/cpu[0-9]* */
int sysfs_count_cpus(void);

/* Count entries matching a glob-like prefix + "state[0-9]*" pattern */
int sysfs_count_idle_states(int cpu_id);

/* Count thermal zones matching /sys/class/thermal/thermal_zone[0-9]* */
int sysfs_count_thermal_zones(void);

#endif /* SYSFS_UTILS_H */
