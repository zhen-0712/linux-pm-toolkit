#ifndef OUTPUT_H
#define OUTPUT_H

#include "cpufreq.h"
#include "cpuidle.h"
#include "thermal.h"

typedef enum {
    OUTPUT_JSON,
    OUTPUT_CSV,
    OUTPUT_TEXT
} output_format_t;

/*
 * Print a complete snapshot (all CPUs + all thermal zones) as one line.
 * JSON mode: one JSON object per line (suitable for piping to dashboard).
 * CSV mode:  one row per CPU (writes header only on first call).
 * Text mode: human-readable table.
 */
void output_snapshot(
    output_format_t     fmt,
    const cpufreq_info_t  *freqs,   int num_freqs,
    const cpuidle_info_t  *idles,   int num_idles,
    const thermal_zone_t  *thermals, int num_thermals
);

output_format_t output_parse_format(const char *str);

#endif /* OUTPUT_H */
