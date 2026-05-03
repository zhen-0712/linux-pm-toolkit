#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "sysfs_utils.h"
#include "cpufreq.h"
#include "cpuidle.h"
#include "thermal.h"
#include "output.h"

static volatile int running = 1;

static void handle_sigint(int sig)
{
    (void)sig;
    running = 0;
}

static void usage(const char *prog)
{
    fprintf(stderr,
        "Usage: %s [options]\n"
        "  -i, --interval <sec>   polling interval in seconds (default: 1)\n"
        "  -f, --format <fmt>     output format: json|csv|text (default: text)\n"
        "  -n, --count <n>        number of samples (0 = infinite, default: 0)\n"
        "  -h, --help             show this help\n",
        prog);
}

int main(int argc, char *argv[])
{
    int interval_sec = 1;
    output_format_t fmt = OUTPUT_TEXT;
    int max_samples = 0;

    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interval") == 0) && i + 1 < argc)
            interval_sec = atoi(argv[++i]);
        else if ((strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--format") == 0) && i + 1 < argc)
            fmt = output_parse_format(argv[++i]);
        else if ((strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--count") == 0) && i + 1 < argc)
            max_samples = atoi(argv[++i]);
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        }
    }

    signal(SIGINT, handle_sigint);

    int num_cpus     = sysfs_count_cpus();
    int num_thermals = sysfs_count_thermal_zones();

    if (num_cpus <= 0) {
        fprintf(stderr, "Error: could not detect CPUs via sysfs\n");
        return 1;
    }

    cpufreq_info_t *freqs    = calloc(num_cpus,                   sizeof(*freqs));
    cpuidle_info_t *idles    = calloc(num_cpus,                   sizeof(*idles));
    thermal_zone_t *thermals = calloc(num_thermals > 0 ? num_thermals : 1,
                                      sizeof(*thermals));

    if (!freqs || !idles || !thermals) {
        fprintf(stderr, "Error: allocation failed\n");
        return 1;
    }

    int sample = 0;
    while (running && (max_samples == 0 || sample < max_samples)) {
        /* only count CPUs that actually have cpufreq/cpuidle support */
        int valid_freqs = 0;
        int valid_idles = 0;

        for (int c = 0; c < num_cpus; c++) {
            if (cpufreq_read(sysfs_read_file, c, &freqs[valid_freqs]) == 0)
                valid_freqs++;
            if (cpuidle_read(sysfs_read_file, c, &idles[valid_idles]) == 0)
                valid_idles++;
        }
        for (int z = 0; z < num_thermals; z++)
            thermal_read(sysfs_read_file, z, &thermals[z]);

        if (valid_freqs == 0 && valid_idles == 0 && num_thermals == 0) {
            fprintf(stderr,
                "Warning: no cpufreq/cpuidle/thermal data available on this system.\n"
                "  On WSL2, Hyper-V does not expose standard Linux PM sysfs interfaces.\n"
                "  Run on bare-metal Linux or a KVM/QEMU VM for real data.\n");
            break;
        }

        output_snapshot(fmt,
                        freqs, valid_freqs,
                        idles, valid_idles,
                        thermals, num_thermals);
        fflush(stdout);

        sample++;
        if (max_samples == 0 || sample < max_samples)
            sleep(interval_sec);
    }

    free(freqs);
    free(idles);
    free(thermals);
    return 0;
}
