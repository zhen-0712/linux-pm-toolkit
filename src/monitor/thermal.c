#include "thermal.h"

#include <stdio.h>
#include <string.h>

int thermal_read(sysfs_reader_fn reader, int zone_id, thermal_zone_t *zone)
{
    /* TODO: implement in feature/userspace-thermal */
    return -1;
}

void thermal_print_json(const thermal_zone_t *zone)
{
    /* TODO: implement in feature/userspace-thermal */
}

void thermal_print_csv_header(void)
{
    printf("zone_id,type,temp_milli_c\n");
}

void thermal_print_csv(const thermal_zone_t *zone)
{
    /* TODO: implement in feature/userspace-thermal */
}
