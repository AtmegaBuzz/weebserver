
CC = g++
DIR = src

all: webserver

webserver: ${DIR}/networking/server.o ${DIR}/main.o ${DIR}/utils/utils.o
	${CC} ${DIR}/networking/server.o ${DIR}/main.o ${DIR}/utils/utils.o -o webserver

server.o: ${DIR}/networking/server.cpp
	${CC} server.cpp

main.o: ${DIR}/main.o
	${CC} main.cpp

utils.o: ${DIR}/utils/utils.o
	${CC} utils.cpp

clean:
	rm ${DIR}/networking/server.o
	rm ${DIR}/utils/utils.o
	rm ${DIR}/main.o
	rm webserver
	