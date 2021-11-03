all: build run
run:
	./simulador
build: 
	g++ main.cpp -std=c++11 -lpthread -o simulador
clean:
	rm -f simulador