compile:
	mkdir -p out/bin
	g++ ./src/main.cpp -o ./out/bin/main.out

calculate:
	./out/bin/main.out data/kroA200.tsp iterated ./out/1.txt ./out/2.txt 68
map:
	python3 ./src/draw.py data/kroA200.tsp iterated

run: compile calculate map
	@echo "Done 🙌🎉"
