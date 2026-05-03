#ifndef CPUIDLE_H
#define CPUIDLE_H

#include "sysfs_utils.h"

#define CPUIDLE_NAME_LEN   32
#define CPUIDLE_MAX_STATES 16

typedef struct {
    char name[CPUIDLE_NAME_LEN]; /* e.g. "C1", "C6" */
    long usage;                  /* number of times entered */
    long time_us;                /* total time spent in state (microseconds) */
    int  disabled;               /* 1 if state is disabled */
} cpuidle_state_t;

typedef struct {
    int              cpu_id;
    cpuidle_state_t  states[CPUIDLE_MAX_STATES];
    int              num_states;
} cpuidle_info_t;

/*
 * Populate idle state info for a single CPU.
 * Returns 0 on success, -1 if cpuidle is not available.
 */
int cpuidle_read(sysfs_reader_fn reader, int cpu_id, cpuidle_info_t *info);

void cpuidle_print_json(const cpuidle_info_t *info);
void cpuidle_print_csv_header(void);
void cpuidle_print_csv(const cpuidle_info_t *info);

#endif /* CPUIDLE_H */
