import io
import json

from collector import read_snapshots


def _stream(*dicts):
    """Build a StringIO of JSON lines from dicts."""
    return io.StringIO("\n".join(json.dumps(d) for d in dicts) + "\n")


def test_reads_single_snapshot():
    data = {"cpufreq": [], "thermal": []}
    snaps = list(read_snapshots(_stream(data)))
    assert len(snaps) == 1
    assert snaps[0] == data


def test_reads_multiple_snapshots():
    snaps = list(read_snapshots(_stream({"ts": 1}, {"ts": 2}, {"ts": 3})))
    assert len(snaps) == 3
    assert [s["ts"] for s in snaps] == [1, 2, 3]


def test_skips_empty_lines():
    stream = io.StringIO('\n\n{"ts":1}\n\n{"ts":2}\n\n')
    snaps = list(read_snapshots(stream))
    assert len(snaps) == 2


def test_skips_invalid_json(capsys):
    stream = io.StringIO('not-json\n{"ts":1}\n')
    snaps = list(read_snapshots(stream))
    assert len(snaps) == 1
    assert snaps[0] == {"ts": 1}
    captured = capsys.readouterr()
    assert "bad JSON" in captured.err


def test_empty_stream_yields_nothing():
    snaps = list(read_snapshots(io.StringIO("")))
    assert snaps == []
