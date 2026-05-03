#include "output.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

output_format_t output_parse_format(const char *str)
{
    if (strcmp(str, "json") == 0) return OUTPUT_JSON;
    if (strcmp(str, "csv")  == 0) return OUTPUT_CSV;
    return OUTPUT_TEXT;
}

/* ── JSON ───────────────────────────────────────────────────── */

static void output_json(
    const cpufreq_info_t  *freqs,    int num_freqs,
    const cpuidle_info_t  *idles,    int num_idles,
    const thermal_zone_t  *thermals, int num_thermals)
{
    printf("{\"ts\":%ld", (long)time(NULL));

    /* cpufreq array */
    printf(",\"cpufreq\":[");
    for (int i = 0; i < num_freqs; i++) {
        if (i > 0) printf(",");
        cpufreq_print_json(&freqs[i]);
    }
    printf("]");

    /* cpuidle array */
    printf(",\"cpuidle\":[");
    for (int i = 0; i < num_idles; i++) {
        if (i > 0) printf(",");
        cpuidle_print_json(&idles[i]);
    }
    printf("]");

    /* thermal array */
    printf(",\"thermal\":[");
    for (int i = 0; i < num_thermals; i++) {
        if (i > 0) printf(",");
        thermal_print_json(&thermals[i]);
    }
    printf("]");

    printf("}\n");
}

/* ── CSV ────────────────────────────────────────────────────── */

static int csv_header_printed = 0;

static void output_csv(
    const cpufreq_info_t  *freqs,    int num_freqs,
    const cpuidle_info_t  *idles,    int num_idles,
    const thermal_zone_t  *thermals, int num_thermals)
{
    if (!csv_header_printed) {
        cpufreq_print_csv_header();
        cpuidle_print_csv_header();
        thermal_print_csv_header();
        csv_header_printed = 1;
    }
    for (int i = 0; i < num_freqs;    i++) cpufreq_print_csv(&freqs[i]);
    for (int i = 0; i < num_idles;    i++) cpuidle_print_csv(&idles[i]);
    for (int i = 0; i < num_thermals; i++) thermal_print_csv(&thermals[i]);
}

/* ── Text ───────────────────────────────────────────────────── */

static void output_text(
    const cpufreq_info_t  *freqs,    int num_freqs,
    const cpuidle_info_t  *idles,    int num_idles,
    const thermal_zone_t  *thermals, int num_thermals)
{
    printf("=== CPU Frequency ===\n");
    for (int i = 0; i < num_freqs; i++) {
        printf("  CPU%-2d  %5ld MHz  governor=%-12s  range=%ld-%ld MHz\n",
               freqs[i].cpu_id,
               freqs[i].cur_freq_khz / 1000,
               freqs[i].governor,
               freqs[i].min_freq_khz / 1000,
               freqs[i].max_freq_khz / 1000);
    }

    printf("=== CPU Idle States ===\n");
    for (int i = 0; i < num_idles; i++) {
        for (int s = 0; s < idles[i].num_states; s++) {
            printf("  CPU%-2d  %-6s  usage=%8ld  time=%10ld us%s\n",
                   idles[i].cpu_id,
                   idles[i].states[s].name,
                   idles[i].states[s].usage,
                   idles[i].states[s].time_us,
                   idles[i].states[s].disabled ? "  [disabled]" : "");
        }
    }

    printf("=== Thermal Zones ===\n");
    for (int i = 0; i < num_thermals; i++) {
        printf("  Zone%-2d  %-20s  %5.1f °C\n",
               thermals[i].zone_id,
               thermals[i].type,
               thermals[i].temp_milli_c / 1000.0);
    }
    printf("\n");
}

/* ── Public API ─────────────────────────────────────────────── */

void output_snapshot(
    output_format_t       fmt,
    const cpufreq_info_t  *freqs,    int num_freqs,
    const cpuidle_info_t  *idles,    int num_idles,
    const thermal_zone_t  *thermals, int num_thermals)
{
    switch (fmt) {
    case OUTPUT_JSON:
        output_json(freqs, num_freqs, idles, num_idles, thermals, num_thermals);
        break;
    case OUTPUT_CSV:
        output_csv(freqs, num_freqs, idles, num_idles, thermals, num_thermals);
        break;
    default:
        output_text(freqs, num_freqs, idles, num_idles, thermals, num_thermals);
        break;
    }
}
