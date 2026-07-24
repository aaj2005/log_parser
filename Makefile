COMPILER=-DCMAKE_CXX_COMPILER=clang++-17 -DCMAKE_C_COMPILER=clang-17


.PHONY: build
build:

	@echo "Building:"
	@mkdir -p build
	@cd build && \
	cmake ${COMPILER} .. && \
	cmake --build .

.PHONY: debug
debug:

	@echo "Building:"
	@mkdir -p build
	@cd build && \
	cmake ${COMPILER} -DCMAKE_BUILD_TYPE=Debug .. && \
	cmake --build .
	gdb build/log_analyser

