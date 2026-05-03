# linux-pm-toolkit

![CI](https://github.com/zhen-0712/linux-pm-toolkit/actions/workflows/ci.yml/badge.svg)

A Linux CPU power management monitoring toolkit demonstrating hands-on work with sysfs, cpufreq, cpuidle, and thermal subsystems — from userspace tooling down to a kernel module.

## Architecture

```
/sys/devices/system/cpu/   /sys/class/thermal/
          |                        |
     sysfs_utils.c  (abstraction layer)
          |
  ┌───────┼────────┐
cpufreq  cpuidle  thermal
  └───────┼────────┘
          |
      output.c  (JSON / CSV)
          |  stdout pipe
   dashboard/collector.py
          |
   dashboard/dashboard.py  (real-time visualization)
```

**Part B (kernel space):**
```
pm_ctrl.c  →  /proc/pm_ctrl  →  read/write from userspace
```

## Components

| Path | Description |
|---|---|
| `src/monitor/` | C userspace tool — reads cpufreq, cpuidle, thermal via sysfs |
| `src/dashboard/` | Python real-time dashboard (matplotlib / rich TUI) |
| `src/kernel/` | Linux kernel module with `/proc` interface |
| `tests/unit/` | C unit tests with mock sysfs fixtures |
| `tests/integration/` | Shell integration tests |

## Quick Start

```bash
# Build userspace monitor
make monitor

# Run with dashboard
make run

# Run tests
make test

# Build kernel module (requires kernel headers)
make kernel
sudo insmod src/kernel/pm_ctrl.ko
cat /proc/pm_ctrl
```

## Sysfs Paths Used

| Subsystem | Path | Description |
|---|---|---|
| cpufreq | `/sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq` | Current frequency (kHz) |
| cpufreq | `/sys/devices/system/cpu/cpu*/cpufreq/scaling_governor` | Active governor |
| cpufreq | `/sys/devices/system/cpu/cpu*/cpufreq/scaling_available_governors` | Available governors |
| cpufreq | `/sys/devices/system/cpu/cpu*/cpufreq/cpuinfo_min_freq` | Min frequency |
| cpufreq | `/sys/devices/system/cpu/cpu*/cpufreq/cpuinfo_max_freq` | Max frequency |
| cpuidle | `/sys/devices/system/cpu/cpu*/cpuidle/state*/name` | C-state name |
| cpuidle | `/sys/devices/system/cpu/cpu*/cpuidle/state*/usage` | Entry count |
| cpuidle | `/sys/devices/system/cpu/cpu*/cpuidle/state*/time` | Time spent (µs) |
| thermal | `/sys/class/thermal/thermal_zone*/temp` | Temperature (millidegrees C) |
| thermal | `/sys/class/thermal/thermal_zone*/type` | Zone type |

## Branch Strategy

| Branch | Content |
|---|---|
| `main` | Stable, demo-ready |
| `feature/project-skeleton` | Repo init, structure, Makefile |
| `feature/sysfs-exploration` | Exploration script, path documentation |
| `feature/userspace-cpufreq` | cpufreq reader + unit tests |
| `feature/userspace-thermal` | thermal + cpuidle readers + integration |
| `feature/python-dashboard` | Python collector + dashboard |
| `feature/kernel-module-base` | Hello-world .ko |
| `feature/kernel-proc-interface` | /proc/pm_ctrl read/write |

## Requirements

- Linux kernel 4.x+ (tested on 5.x / 6.x)
- GCC, make
- Python 3.8+: `pip install -r src/dashboard/requirements.txt`
- Kernel headers (for module build): `sudo apt install linux-headers-$(uname -r)`
