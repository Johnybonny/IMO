compile:
	mkdir -p out/bin
	g++ ./src/main.cpp -o ./out/bin/main.out

calculate:
	./out/bin/main.out data/kroB200.tsp ./out/1.txt ./out/2.txt ./out/3.txt common-edges compare-ils2

map:
	python3 ./src/draw.py data/kroB200.tsp common-edges-ils2

run: compile calculate map
	@echo "Done 🙌🎉"
