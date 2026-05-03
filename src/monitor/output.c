#include "output.h"

#include <stdio.h>
#include <string.h>

output_format_t output_parse_format(const char *str)
{
    if (strcmp(str, "json") == 0) return OUTPUT_JSON;
    if (strcmp(str, "csv")  == 0) return OUTPUT_CSV;
    return OUTPUT_TEXT;
}

void output_snapshot(
    output_format_t      fmt,
    const cpufreq_info_t  *freqs,    int num_freqs,
    const cpuidle_info_t  *idles,    int num_idles,
    const thermal_zone_t  *thermals, int num_thermals)
{
    /* TODO: implement in feature/userspace-thermal */
    (void)fmt; (void)freqs; (void)num_freqs;
    (void)idles; (void)num_idles;
    (void)thermals; (void)num_thermals;
}
