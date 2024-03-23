build:
	g++ -std=c++17 -O2 -Wall -Wextra -pedantic src/main.cpp -o power_grid

benchmark:
	./benchmark.sh > benchmark_result.txt 2>&1
