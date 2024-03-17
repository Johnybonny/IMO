compile:
	g++ ./src/main.cpp -o ./out/main.out

calculate:
	./out/main.out data/kroA100.tsp
	./out/main.out data/kroB100.tsp


map:
	python3 ./src/draw.py data/kroA100.tsp greedy_cycle
	python3 ./src/draw.py data/kroA100.tsp nearest_neighbor
	python3 ./src/draw.py data/kroA100.tsp regret_heuristics
	python3 ./src/draw.py data/kroB100.tsp greedy_cycle
	python3 ./src/draw.py data/kroB100.tsp nearest_neighbor
	python3 ./src/draw.py data/kroB100.tsp regret_heuristics

run: compile calculate map
	@echo "Done 🙌🎉"
