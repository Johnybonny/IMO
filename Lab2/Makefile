compile:
	g++ ./src/main.cpp -o ./out/main.out

calculate:
	./out/main.out data/kroA100.tsp regret todo todo
	./out/main.out data/kroA100.tsp random todo todo
	./out/main.out data/kroB100.tsp regret todo todo
	./out/main.out data/kroB100.tsp random todo todo

run: compile calculate
	@echo "Done 🙌🎉"
