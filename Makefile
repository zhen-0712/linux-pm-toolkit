.PHONY: all monitor kernel test clean run help

all: monitor

monitor:
	$(MAKE) -C src/monitor

kernel:
	$(MAKE) -C src/kernel

test: monitor
	$(MAKE) -C tests/unit
	@echo "--- Running unit tests ---"
	./tests/unit/run_tests
	@echo "--- Running integration tests ---"
	bash tests/integration/test_full_read.sh

clean:
	$(MAKE) -C src/monitor clean
	$(MAKE) -C src/kernel clean
	$(MAKE) -C tests/unit clean

run: monitor
	./src/monitor/pm_monitor --interval 1 --format json | python3 src/dashboard/dashboard.py

help:
	@echo "Targets:"
	@echo "  all      - build userspace monitor (default)"
	@echo "  monitor  - build C monitor tool"
	@echo "  kernel   - build kernel module"
	@echo "  test     - run all unit + integration tests"
	@echo "  run      - start monitor piped into dashboard"
	@echo "  clean    - remove all build artifacts"
