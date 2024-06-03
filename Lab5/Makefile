compile:
	mkdir -p out/bin
	g++ ./src/main.cpp -o ./out/bin/main.out

calculate:
	./out/bin/main.out data/kroB200.tsp no_local_search ./out/1.txt ./out/2.txt 20 50 regret
map:
	python3 ./src/draw.py data/kroB200.tsp regret

run: compile calculate map
	@echo "Done 🙌🎉"
