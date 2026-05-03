#ifndef MOCK_READER_H
#define MOCK_READER_H

/*
 * Mock sysfs reader for unit tests.
 *
 * Redirects all sysfs path reads to a local mock_sysfs/ directory tree.
 * e.g. /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq
 *  →   mock_sysfs/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq
 *
 * Path is relative to tests/unit/ because test binaries run from that directory
 * (make -C tests/unit changes CWD to tests/unit/ before executing binaries).
 */

#include "../../src/monitor/sysfs_utils.h"

#include <stdio.h>
#include <string.h>

#ifndef MOCK_SYSFS_ROOT
#define MOCK_SYSFS_ROOT "mock_sysfs"
#endif

static int mock_sysfs_read(const char *path, char *buf, size_t len)
{
    char full_path[512];
    /* Strip leading slash so we can prepend the mock root */
    const char *rel = (path[0] == '/') ? path + 1 : path;
    snprintf(full_path, sizeof(full_path), "%s/%s", MOCK_SYSFS_ROOT, rel);

    FILE *f = fopen(full_path, "r");
    if (!f) return -1;

    size_t n = fread(buf, 1, len - 1, f);
    fclose(f);
    buf[n] = '\0';

    /* Strip trailing newline */
    if (n > 0 && buf[n - 1] == '\n')
        buf[n - 1] = '\0';

    return 0;
}

#endif /* MOCK_READER_H */
