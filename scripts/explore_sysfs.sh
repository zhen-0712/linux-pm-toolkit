#!/usr/bin/env bash
# Day 1: explore all PM-relevant sysfs paths on this machine.
# Run once to understand what your system exposes before coding.

echo "====== CPU count ======"
ls /sys/devices/system/cpu/ | grep -E '^cpu[0-9]+$' | wc -l

echo ""
echo "====== cpufreq (cpu0) ======"
for f in /sys/devices/system/cpu/cpu0/cpufreq/*; do
    echo "$f = $(cat $f 2>/dev/null)"
done

echo ""
echo "====== cpuidle states (cpu0) ======"
for state in /sys/devices/system/cpu/cpu0/cpuidle/state*/; do
    echo "--- $(basename $state) ---"
    for f in "$state"*; do
        echo "  $(basename $f) = $(cat $f 2>/dev/null)"
    done
done

echo ""
echo "====== thermal zones ======"
for zone in /sys/class/thermal/thermal_zone*/; do
    echo "--- $(basename $zone) ---"
    echo "  type = $(cat ${zone}type 2>/dev/null)"
    echo "  temp = $(cat ${zone}temp 2>/dev/null) (millideg C)"
done

echo ""
echo "====== /sys/power/ ======"
for f in /sys/power/*; do
    echo "$f = $(cat $f 2>/dev/null | head -1)"
done
