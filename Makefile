all: build run
run:
	./simulador.out
build: 
	gcc src/main.c -std=c++11 -lpthread -o simulador.out
clean:
	rm -f simulador