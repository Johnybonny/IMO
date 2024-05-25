compile:
	mkdir -p out/bin
	g++ ./src/main.cpp -o ./out/bin/main.out

calculate:
	./out/bin/main.out data/kroA200.tsp common-vertices ./out/1.txt ./out/2.txt 12345

run: compile calculate
	@echo "Done 🙌🎉"
