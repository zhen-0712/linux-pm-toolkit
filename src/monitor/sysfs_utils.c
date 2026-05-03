#include "sysfs_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <dirent.h>

int sysfs_read_file(const char *path, char *buf, size_t len)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;

    size_t n = fread(buf, 1, len - 1, f);
    fclose(f);
    buf[n] = '\0';

    /* strip trailing \n and \r (handles both LF and CRLF) */
    if (n > 0 && buf[n - 1] == '\n') buf[--n] = '\0';
    if (n > 0 && buf[n - 1] == '\r') buf[--n] = '\0';

    return 0;
}

int sysfs_read_long(sysfs_reader_fn reader, const char *path, long *out)
{
    char buf[64];
    if (reader(path, buf, sizeof(buf)) != 0) return -1;

    char *end;
    errno = 0;
    *out = strtol(buf, &end, 10);
    if (end == buf || errno != 0) return -1;

    return 0;
}

int sysfs_read_str(sysfs_reader_fn reader, const char *path, char *out, size_t len)
{
    return reader(path, out, len);
}

/*
 * Count entries under dir_path whose names start with prefix and are
 * followed only by decimal digits (e.g. "cpu0", "state3", "thermal_zone1").
 */
static int count_prefixed_numeric_entries(const char *dir_path,
                                          const char *prefix,
                                          size_t      prefix_len)
{
    DIR *d = opendir(dir_path);
    if (!d) return 0;

    int count = 0;
    struct dirent *entry;

    while ((entry = readdir(d)) != NULL) {
        if (strncmp(entry->d_name, prefix, prefix_len) != 0) continue;

        const char *rest = entry->d_name + prefix_len;
        if (rest[0] == '\0') continue;

        int all_digits = 1;
        for (int i = 0; rest[i]; i++) {
            if (!isdigit((unsigned char)rest[i])) { all_digits = 0; break; }
        }
        if (all_digits) count++;
    }

    closedir(d);
    return count;
}

int sysfs_count_cpus(void)
{
    return count_prefixed_numeric_entries(
        "/sys/devices/system/cpu", "cpu", 3);
}

int sysfs_count_idle_states(int cpu_id)
{
    char path[256];
    snprintf(path, sizeof(path),
             "/sys/devices/system/cpu/cpu%d/cpuidle", cpu_id);
    return count_prefixed_numeric_entries(path, "state", 5);
}

int sysfs_count_thermal_zones(void)
{
    return count_prefixed_numeric_entries(
        "/sys/class/thermal", "thermal_zone", 12);
}
