
all: webserver

webserver: src/networking/server.o src/main.o src/utils/utils.o
	g++ src/networking/server.o src/main.o src/utils/utils.o -o webserver

server.o: src/networking/server.cpp
	g++ server.cpp

main.o: src/main.o
	g++ main.cpp

utils.o: src/utils/utils.o
	g++ utils.cpp

clean:
	rm src/networking/server.o
	rm src/utils/utils.o
	rm src/main.o
	rm webserver
	