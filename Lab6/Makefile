compile:
	mkdir -p out/bin
	g++ ./src/main.cpp -o ./out/bin/main.out

calculate:
	./out/bin/main.out data/kroA200.tsp ./out/1.txt ./out/2.txt ./out/3.txt common-edges compare-best

map:
	python3 ./src/draw.py data/kroA200.tsp common-edges

run: compile calculate map
	@echo "Done 🙌🎉"
