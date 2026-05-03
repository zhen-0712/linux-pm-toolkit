#include "cpufreq.h"
#include "sysfs_utils.h"

#include <stdio.h>
#include <string.h>

int cpufreq_read(sysfs_reader_fn reader, int cpu_id, cpufreq_info_t *info)
{
    char path[256];
    memset(info, 0, sizeof(*info));
    info->cpu_id = cpu_id;

    snprintf(path, sizeof(path),
             "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_cur_freq", cpu_id);
    if (sysfs_read_long(reader, path, &info->cur_freq_khz) != 0)
        return -1;  /* no cpufreq support on this CPU */

    snprintf(path, sizeof(path),
             "/sys/devices/system/cpu/cpu%d/cpufreq/cpuinfo_min_freq", cpu_id);
    sysfs_read_long(reader, path, &info->min_freq_khz);

    snprintf(path, sizeof(path),
             "/sys/devices/system/cpu/cpu%d/cpufreq/cpuinfo_max_freq", cpu_id);
    sysfs_read_long(reader, path, &info->max_freq_khz);

    snprintf(path, sizeof(path),
             "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_governor", cpu_id);
    sysfs_read_str(reader, path, info->governor, sizeof(info->governor));

    /* parse space-separated list of available governors */
    char avail[512] = {0};
    snprintf(path, sizeof(path),
             "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_available_governors", cpu_id);
    sysfs_read_str(reader, path, avail, sizeof(avail));

    info->num_governors = 0;
    char *saveptr;
    char *tok = strtok_r(avail, " ", &saveptr);
    while (tok && info->num_governors < CPUFREQ_MAX_GOVERNORS) {
        strncpy(info->available_governors[info->num_governors], tok,
                CPUFREQ_GOVERNOR_LEN - 1);
        info->available_governors[info->num_governors][CPUFREQ_GOVERNOR_LEN - 1] = '\0';
        info->num_governors++;
        tok = strtok_r(NULL, " ", &saveptr);
    }

    return 0;
}

void cpufreq_print_json(const cpufreq_info_t *info)
{
    printf("{\"cpu\":%d,\"cur_khz\":%ld,\"min_khz\":%ld,\"max_khz\":%ld,"
           "\"governor\":\"%s\"}",
           info->cpu_id, info->cur_freq_khz,
           info->min_freq_khz, info->max_freq_khz,
           info->governor);
}

void cpufreq_print_csv_header(void)
{
    printf("cpu_id,cur_freq_khz,min_freq_khz,max_freq_khz,governor\n");
}

void cpufreq_print_csv(const cpufreq_info_t *info)
{
    printf("%d,%ld,%ld,%ld,%s\n",
           info->cpu_id, info->cur_freq_khz,
           info->min_freq_khz, info->max_freq_khz,
           info->governor);
}
