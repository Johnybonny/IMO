compile:
	mkdir -p out/bin
	g++ ./src/main.cpp -o ./out/bin/main.out

calculate:
	./out/bin/main.out data/kroB200.tsp candidates ./out/1.txt ./out/2.txt 10
map:
	python3 ./src/draw.py data/kroB200.tsp candidate

run: compile calculate map
	@echo "Done 🙌🎉"
