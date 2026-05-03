"""
collector.py — reads JSON lines from stdin (piped from pm_monitor)
and yields parsed snapshot dicts.
"""

import sys
import json
from typing import Iterator


def read_snapshots(stream=None) -> Iterator[dict]:
    """Yield parsed JSON snapshot dicts from stream (default: stdin)."""
    if stream is None:
        stream = sys.stdin
    for line in stream:
        line = line.strip()
        if not line:
            continue
        try:
            yield json.loads(line)
        except json.JSONDecodeError as e:
            print(f"[collector] bad JSON: {e}", file=sys.stderr)

