#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/cpu.h>
#include <linux/cpufreq.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("linux-pm-toolkit");
MODULE_DESCRIPTION("Mock power domain controller with /proc interface");
MODULE_VERSION("0.2");

/* ── Mock power domain table ────────────────────────────────── */

#define NUM_DOMAINS 4

static struct pm_domain {
    const char *name;
    int         enabled;
} domains[NUM_DOMAINS] = {
    { "cpu",    1 },
    { "gpu",    1 },
    { "memory", 1 },
    { "io",     1 },
};

static DEFINE_MUTEX(domains_lock);
static struct proc_dir_entry *pm_ctrl_proc;

/* ── /proc/pm_ctrl read ─────────────────────────────────────── */

static int pm_ctrl_show(struct seq_file *m, void *v)
{
    int i;

    seq_printf(m, "%-10s  %s\n", "domain", "state");
    seq_printf(m, "%-10s  %s\n", "----------", "--------");

    mutex_lock(&domains_lock);
    for (i = 0; i < NUM_DOMAINS; i++) {
        seq_printf(m, "%-10s  %s\n",
                   domains[i].name,
                   domains[i].enabled ? "enabled" : "disabled");
    }
    mutex_unlock(&domains_lock);

    seq_puts(m, "\nUsage: echo \"enable|disable <domain>\" > /proc/pm_ctrl\n");
    return 0;
}

static int pm_ctrl_open(struct inode *inode, struct file *file)
{
    return single_open(file, pm_ctrl_show, NULL);
}

/* ── /proc/pm_ctrl write ────────────────────────────────────── */

static ssize_t pm_ctrl_write(struct file *file, const char __user *ubuf,
                              size_t count, loff_t *ppos)
{
    char kbuf[64];
    char cmd[16], name[32];
    size_t len = min(count, sizeof(kbuf) - 1);
    int i;

    if (copy_from_user(kbuf, ubuf, len))
        return -EFAULT;

    kbuf[len] = '\0';
    if (len > 0 && kbuf[len - 1] == '\n')
        kbuf[len - 1] = '\0';

    if (sscanf(kbuf, "%15s %31s", cmd, name) != 2) {
        pr_warn("pm_ctrl: invalid command '%s' — use: enable|disable <domain>\n",
                kbuf);
        return -EINVAL;
    }

    mutex_lock(&domains_lock);
    for (i = 0; i < NUM_DOMAINS; i++) {
        if (strcmp(domains[i].name, name) != 0)
            continue;

        if (strcmp(cmd, "enable") == 0) {
            domains[i].enabled = 1;
            pr_info("pm_ctrl: domain '%s' -> enabled\n", name);
        } else if (strcmp(cmd, "disable") == 0) {
            domains[i].enabled = 0;
            pr_info("pm_ctrl: domain '%s' -> disabled\n", name);
        } else {
            mutex_unlock(&domains_lock);
            pr_warn("pm_ctrl: unknown command '%s'\n", cmd);
            return -EINVAL;
        }

        mutex_unlock(&domains_lock);
        return count;
    }
    mutex_unlock(&domains_lock);

    pr_warn("pm_ctrl: unknown domain '%s'\n", name);
    return -EINVAL;
}

/* ── proc_ops (kernel 5.6+) ─────────────────────────────────── */

static const struct proc_ops pm_ctrl_ops = {
    .proc_open    = pm_ctrl_open,
    .proc_read    = seq_read,
    .proc_write   = pm_ctrl_write,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

/* ── module lifecycle ───────────────────────────────────────── */

static void report_cpu_frequencies(void)
{
    unsigned int cpu, freq_khz;

    for_each_online_cpu(cpu) {
        freq_khz = cpufreq_get(cpu);
        if (freq_khz)
            pr_info("pm_ctrl: cpu%u = %u kHz\n", cpu, freq_khz);
    }
}

static int __init pm_ctrl_init(void)
{
    pr_info("pm_ctrl: loading on %u online CPUs\n", num_online_cpus());
    report_cpu_frequencies();

    pm_ctrl_proc = proc_create("pm_ctrl", 0644, NULL, &pm_ctrl_ops);
    if (!pm_ctrl_proc) {
        pr_err("pm_ctrl: failed to create /proc/pm_ctrl\n");
        return -ENOMEM;
    }

    pr_info("pm_ctrl: /proc/pm_ctrl ready — %d power domains registered\n",
            NUM_DOMAINS);
    return 0;
}

static void __exit pm_ctrl_exit(void)
{
    proc_remove(pm_ctrl_proc);
    pr_info("pm_ctrl: unloaded, /proc/pm_ctrl removed\n");
}

module_init(pm_ctrl_init);
module_exit(pm_ctrl_exit);
