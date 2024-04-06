compile:
	mkdir -p out/bin
	g++ ./src/main.cpp -o ./out/bin/main.out

calculate:
	python3 ./src/calculate.py

run: compile calculate
	@echo "Done 🙌🎉"
