#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cpu.h>
#include <linux/cpufreq.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("linux-pm-toolkit");
MODULE_DESCRIPTION("Power management controller demo: reads cpufreq from kernel space");
MODULE_VERSION("0.1");

/*
 * Print current frequency for every online CPU via cpufreq_get().
 * cpufreq_get() calls into the cpufreq driver and returns the cached
 * frequency in kHz — the same value userspace reads from
 * /sys/devices/system/cpu/cpuN/cpufreq/scaling_cur_freq.
 */
static void report_cpu_frequencies(void)
{
    unsigned int cpu;
    unsigned int freq_khz;

    for_each_online_cpu(cpu) {
        freq_khz = cpufreq_get(cpu);
        if (freq_khz)
            pr_info("pm_ctrl: cpu%u freq = %u kHz (%u MHz)\n",
                    cpu, freq_khz, freq_khz / 1000);
        else
            pr_info("pm_ctrl: cpu%u cpufreq not available\n", cpu);
    }
}

static int __init pm_ctrl_init(void)
{
    pr_info("pm_ctrl: loaded — %u online CPUs\n", num_online_cpus());
    report_cpu_frequencies();
    /* /proc interface added in feature/kernel-proc-interface */
    return 0;
}

static void __exit pm_ctrl_exit(void)
{
    pr_info("pm_ctrl: unloaded\n");
}

module_init(pm_ctrl_init);
module_exit(pm_ctrl_exit);
