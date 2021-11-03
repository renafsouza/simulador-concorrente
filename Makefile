all: build run
run:
	./simulador
build: 
	g++ src/main.cpp -std=c++11 -lpthread -o simulador
clean:
	rm -f simulador