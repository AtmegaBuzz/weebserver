
all: webserver

webserver: src/networking/server.o src/main.o
	gcc src/networking/server.o src/main.o -o webserver

server.o: src/networking/server.c
	gcc server.c

main.o: src/main.o
	gcc main.c

clean:
	rm src/networking/server.o
	rm src/main.o
	rm webserver
	