#!/usr/bin/env bash
# Integration test: run pm_monitor for 2 samples and validate output

set -e

MONITOR=./src/monitor/pm_monitor

if [ ! -f "$MONITOR" ]; then
    echo "ERROR: $MONITOR not built. Run 'make monitor' first."
    exit 1
fi

echo "--- Integration: text output ---"
$MONITOR --interval 0 --count 1 --format text
echo "OK"

echo "--- Integration: JSON output (validate structure) ---"
output=$($MONITOR --interval 0 --count 1 --format json)
echo "$output" | python3 -c "import sys, json; json.load(sys.stdin); print('JSON valid: OK')"

echo "--- Integration: CSV output ---"
$MONITOR --interval 0 --count 2 --format csv | head -5
echo "OK"

echo "All integration tests passed."
