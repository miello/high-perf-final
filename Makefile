build:
	g++ -std=c++17 -O2 -Wall -Wextra -pedantic src/main.cpp -o power_grid
	g++ -std=c++17 -O2 -Wall -Wextra -pedantic src/validator.cpp -o validator
	
benchmark:
	./benchmark.sh > benchmark_result.txt 2>&1

run:
	./power_grid ./PowerGrid/$(FILE) $(FILE).out