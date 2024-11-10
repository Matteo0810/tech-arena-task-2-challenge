default_target: build
.PHONY: default_target

build:
	mkdir -p build && cd build && cmake .. && make && ./main
.PHONY: build