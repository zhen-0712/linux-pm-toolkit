#include "cpuidle.h"
#include "sysfs_utils.h"

#include <stdio.h>
#include <string.h>

int cpuidle_read(sysfs_reader_fn reader, int cpu_id, cpuidle_info_t *info)
{
    char path[256];
    memset(info, 0, sizeof(*info));
    info->cpu_id = cpu_id;

    /*
     * Probe states by attempting to read each state's name file.
     * Stop at the first missing state rather than calling opendir(),
     * so the mock sysfs reader works correctly in unit tests.
     */
    for (int s = 0; s < CPUIDLE_MAX_STATES; s++) {
        snprintf(path, sizeof(path),
                 "/sys/devices/system/cpu/cpu%d/cpuidle/state%d/name",
                 cpu_id, s);
        if (sysfs_read_str(reader, path,
                           info->states[s].name,
                           sizeof(info->states[s].name)) != 0)
            break;

        snprintf(path, sizeof(path),
                 "/sys/devices/system/cpu/cpu%d/cpuidle/state%d/usage",
                 cpu_id, s);
        sysfs_read_long(reader, path, &info->states[s].usage);

        snprintf(path, sizeof(path),
                 "/sys/devices/system/cpu/cpu%d/cpuidle/state%d/time",
                 cpu_id, s);
        sysfs_read_long(reader, path, &info->states[s].time_us);

        long disabled = 0;
        snprintf(path, sizeof(path),
                 "/sys/devices/system/cpu/cpu%d/cpuidle/state%d/disable",
                 cpu_id, s);
        sysfs_read_long(reader, path, &disabled);
        info->states[s].disabled = (int)disabled;

        info->num_states = s + 1;
    }

    return (info->num_states > 0) ? 0 : -1;
}

void cpuidle_print_json(const cpuidle_info_t *info)
{
    printf("{\"cpu\":%d,\"states\":[", info->cpu_id);
    for (int s = 0; s < info->num_states; s++) {
        if (s > 0) printf(",");
        printf("{\"name\":\"%s\",\"usage\":%ld,\"time_us\":%ld,\"disabled\":%d}",
               info->states[s].name,
               info->states[s].usage,
               info->states[s].time_us,
               info->states[s].disabled);
    }
    printf("]}");
}

void cpuidle_print_csv_header(void)
{
    printf("cpu_id,state_name,usage,time_us,disabled\n");
}

void cpuidle_print_csv(const cpuidle_info_t *info)
{
    for (int s = 0; s < info->num_states; s++) {
        printf("%d,%s,%ld,%ld,%d\n",
               info->cpu_id,
               info->states[s].name,
               info->states[s].usage,
               info->states[s].time_us,
               info->states[s].disabled);
    }
}
