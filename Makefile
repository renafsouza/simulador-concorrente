all: build run
run:
	./simulador.out
build: 
	g++ src/main.cpp -std=c++11 -lpthread -o simulador.out
clean:
	rm -f simulador