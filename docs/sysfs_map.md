# Linux PM Sysfs Path Reference

All paths read by `pm_monitor`. Verified on Linux kernel 5.x / 6.x x86_64.
Run `scripts/explore_sysfs.sh` to dump live values on your machine.

---

## cpufreq — `/sys/devices/system/cpu/cpu<N>/cpufreq/`

| File | Type | Example | Description |
|---|---|---|---|
| `scaling_cur_freq` | long (kHz) | `2400000` | Current frequency |
| `scaling_governor` | string | `powersave` | Active governor |
| `scaling_available_governors` | string (space-separated) | `performance powersave` | Available governors |
| `cpuinfo_min_freq` | long (kHz) | `400000` | Hardware minimum |
| `cpuinfo_max_freq` | long (kHz) | `3600000` | Hardware maximum |
| `scaling_min_freq` | long (kHz) | `400000` | Current policy minimum |
| `scaling_max_freq` | long (kHz) | `3600000` | Current policy maximum |

CPU count: `ls /sys/devices/system/cpu/cpu[0-9]* -d | wc -l`

---

## cpuidle — `/sys/devices/system/cpu/cpu<N>/cpuidle/state<S>/`

| File | Type | Example | Description |
|---|---|---|---|
| `name` | string | `C1`, `C6` | State name |
| `desc` | string | `MWAIT 0x00` | Description |
| `usage` | long | `12345` | Number of times entered |
| `time` | long (µs) | `9876543` | Total time in state |
| `disable` | int (0/1) | `0` | Whether state is disabled |
| `latency` | long (µs) | `2` | Exit latency |

State count per CPU: `ls /sys/devices/system/cpu/cpu0/cpuidle/ | wc -l`

---

## thermal — `/sys/class/thermal/thermal_zone<Z>/`

| File | Type | Example | Description |
|---|---|---|---|
| `temp` | long (millideg C) | `45000` | Current temperature (45.0 °C) |
| `type` | string | `x86_pkg_temp` | Zone type identifier |
| `mode` | string | `enabled` | Whether zone is active |

Common zone types: `x86_pkg_temp`, `acpitz`, `iwlwifi_1`, `pch_skylake`

Zone count: `ls /sys/class/thermal/thermal_zone* -d | wc -l`

---

## power — `/sys/power/`

| File | Example | Description |
|---|---|---|
| `state` | `freeze mem disk` | Supported sleep states |
| `mem_sleep` | `s2idle [deep]` | Available suspend-to-RAM modes |
| `disk` | `platform shutdown reboot` | Hibernate backends |

---

## Notes

- All frequency values in **kHz** (divide by 1000 for MHz, by 1000000 for GHz)
- Temperature in **millidegrees Celsius** (divide by 1000 for °C)
- cpuidle `time` in **microseconds**
- Not all CPUs expose all C-states; count varies by hardware
