default_target: build

build:
	mkdir -p build && cd build && cmake .. && make && ./main
.PHONY: build