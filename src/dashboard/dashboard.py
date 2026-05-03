#!/usr/bin/env python3
"""
dashboard.py — real-time terminal dashboard for pm_monitor JSON output.

Usage:
    ./src/monitor/pm_monitor --format json | python3 src/dashboard/dashboard.py
"""

from collector import read_snapshots

from rich.live import Live
from rich.table import Table
from rich.columns import Columns
from rich.panel import Panel
from rich import box


def _freq_table(cpufreq: list) -> Table:
    t = Table(title="CPU Frequency", box=box.SIMPLE_HEAD, expand=True)
    t.add_column("CPU",        justify="right",  style="cyan")
    t.add_column("Cur (MHz)",  justify="right")
    t.add_column("Min (MHz)",  justify="right",  style="dim")
    t.add_column("Max (MHz)",  justify="right",  style="dim")
    t.add_column("Governor",                     style="green")
    for c in cpufreq:
        t.add_row(
            str(c["cpu"]),
            str(c["cur_khz"] // 1000),
            str(c["min_khz"] // 1000),
            str(c["max_khz"] // 1000),
            c["governor"],
        )
    return t


def _thermal_table(thermal: list) -> Table:
    t = Table(title="Thermal Zones", box=box.SIMPLE_HEAD, expand=True)
    t.add_column("Zone",      justify="right", style="cyan")
    t.add_column("Type")
    t.add_column("Temp (°C)", justify="right")
    for z in thermal:
        temp_c = z["temp_mc"] / 1000.0
        color = "red" if temp_c >= 80 else "yellow" if temp_c >= 65 else "green"
        t.add_row(str(z["zone"]), z["type"],
                  f"[{color}]{temp_c:.1f}[/{color}]")
    return t


def _idle_table(cpuidle: list) -> Table:
    t = Table(title="C-States", box=box.SIMPLE_HEAD, expand=True)
    t.add_column("CPU",       justify="right", style="cyan")
    t.add_column("State")
    t.add_column("Usage",     justify="right")
    t.add_column("Time (ms)", justify="right")
    for cpu in cpuidle:
        for s in cpu.get("states", []):
            flag = " [dim](off)[/dim]" if s["disabled"] else ""
            t.add_row(
                str(cpu["cpu"]),
                s["name"] + flag,
                str(s["usage"]),
                str(s["time_us"] // 1000),
            )
    return t


def main():
    with Live(refresh_per_second=2, screen=False) as live:
        for snap in read_snapshots():
            panels = []
            if snap.get("cpufreq"):
                panels.append(Panel(_freq_table(snap["cpufreq"])))
            if snap.get("thermal"):
                panels.append(Panel(_thermal_table(snap["thermal"])))
            if snap.get("cpuidle"):
                panels.append(Panel(_idle_table(snap["cpuidle"])))
            if panels:
                live.update(Columns(panels))


if __name__ == "__main__":
    main()
