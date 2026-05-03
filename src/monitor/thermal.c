#include "thermal.h"
#include "sysfs_utils.h"

#include <stdio.h>
#include <string.h>

int thermal_read(sysfs_reader_fn reader, int zone_id, thermal_zone_t *zone)
{
    char path[256];
    memset(zone, 0, sizeof(*zone));
    zone->zone_id = zone_id;

    snprintf(path, sizeof(path),
             "/sys/class/thermal/thermal_zone%d/temp", zone_id);
    if (sysfs_read_long(reader, path, &zone->temp_milli_c) != 0)
        return -1;

    snprintf(path, sizeof(path),
             "/sys/class/thermal/thermal_zone%d/type", zone_id);
    sysfs_read_str(reader, path, zone->type, sizeof(zone->type));

    return 0;
}

void thermal_print_json(const thermal_zone_t *zone)
{
    printf("{\"zone\":%d,\"type\":\"%s\",\"temp_mc\":%ld}",
           zone->zone_id, zone->type, zone->temp_milli_c);
}

void thermal_print_csv_header(void)
{
    printf("zone_id,type,temp_milli_c\n");
}

void thermal_print_csv(const thermal_zone_t *zone)
{
    printf("%d,%s,%ld\n",
           zone->zone_id, zone->type, zone->temp_milli_c);
}
