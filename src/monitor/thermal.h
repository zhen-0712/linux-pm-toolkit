#ifndef THERMAL_H
#define THERMAL_H

#include "sysfs_utils.h"

#define THERMAL_TYPE_LEN    32
#define THERMAL_MAX_TRIPS   8

typedef struct {
    int  zone_id;
    char type[THERMAL_TYPE_LEN]; /* e.g. "x86_pkg_temp", "acpitz" */
    long temp_milli_c;           /* temperature in millidegrees Celsius */
} thermal_zone_t;

/*
 * Populate thermal info for a single zone.
 * Returns 0 on success, -1 if zone does not exist.
 */
int thermal_read(sysfs_reader_fn reader, int zone_id, thermal_zone_t *zone);

void thermal_print_json(const thermal_zone_t *zone);
void thermal_print_csv_header(void);
void thermal_print_csv(const thermal_zone_t *zone);

#endif /* THERMAL_H */
