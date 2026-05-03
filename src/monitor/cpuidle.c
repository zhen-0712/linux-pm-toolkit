#include "cpuidle.h"

#include <stdio.h>
#include <string.h>

int cpuidle_read(sysfs_reader_fn reader, int cpu_id, cpuidle_info_t *info)
{
    /* TODO: implement in feature/userspace-thermal */
    return -1;
}

void cpuidle_print_json(const cpuidle_info_t *info)
{
    /* TODO: implement in feature/userspace-thermal */
}

void cpuidle_print_csv_header(void)
{
    printf("cpu_id,state_name,usage,time_us\n");
}

void cpuidle_print_csv(const cpuidle_info_t *info)
{
    /* TODO: implement in feature/userspace-thermal */
}
