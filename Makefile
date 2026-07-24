
.PHONY: build
build:

	@echo "Building:"
	@mkdir -p build
	@cd build && \
	cmake -DCMAKE_CXX_COMPILER=clang++-17 -DCMAKE_C_COMPILER=clang-17 .. && \
	cmake --build .

