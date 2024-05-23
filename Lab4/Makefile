compile:
	mkdir -p out/bin
	g++ ./src/main.cpp -o ./out/bin/main.out

calculate:
	./out/bin/main.out data/kroB200.tsp large-scale ./out/1.txt ./out/2.txt 50 no_use_local_s
map:
	python3 ./src/draw.py data/kroB200.tsp large-scale-a

run: compile calculate map
	@echo "Done 🙌🎉"
