compile:
	mkdir -p out/bin
	g++ ./src/main.cpp -o ./out/bin/main.out

calculate:
	./out/bin/main.out data/kroB200.tsp pastMoves ./out/1.txt ./out/2.txt 10
map:
	python3 ./src/draw.py data/kroB200.tsp pastMoves

testloop: compile
	for number in 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20; do \
    	./out/bin/main.out data/kroB200.tsp candidates ./out/1.txt ./out/2.txt $$number ; \
	done

run: compile calculate map
	@echo "Done 🙌🎉"
