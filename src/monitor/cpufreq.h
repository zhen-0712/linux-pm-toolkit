#ifndef CPUFREQ_H
#define CPUFREQ_H

#include "sysfs_utils.h"

#define CPUFREQ_GOVERNOR_LEN  64
#define CPUFREQ_MAX_GOVERNORS 16

typedef struct {
    int  cpu_id;
    long cur_freq_khz;
    long min_freq_khz;
    long max_freq_khz;
    char governor[CPUFREQ_GOVERNOR_LEN];
    char available_governors[CPUFREQ_MAX_GOVERNORS][CPUFREQ_GOVERNOR_LEN];
    int  num_governors;
} cpufreq_info_t;

/*
 * Populate info for a single CPU. Uses reader for all sysfs access.
 * Returns 0 on success, -1 if the CPU has no cpufreq support.
 */
int cpufreq_read(sysfs_reader_fn reader, int cpu_id, cpufreq_info_t *info);

/* Print one JSON object for info to stdout (no trailing newline) */
void cpufreq_print_json(const cpufreq_info_t *info);

/* Print CSV header line */
void cpufreq_print_csv_header(void);

/* Print one CSV row */
void cpufreq_print_csv(const cpufreq_info_t *info);

#endif /* CPUFREQ_H */
