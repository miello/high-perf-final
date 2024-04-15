build:
	g++ -fopenmp -std=c++17 -O2 -Wall -Wextra -pedantic src/main.cpp -o power_grid
	g++ -std=c++17 -O2 -Wall -Wextra -pedantic src/validator.cpp -o validator
	
benchmark:
	./benchmark.sh > benchmark_result_$$(date +%s).txt 2>&1

run:
	make build
	time -p ./power_grid ./PowerGrid/$(FILE) $(FILE).out