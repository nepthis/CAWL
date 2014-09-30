CC=c++
CFLAGS=-c -Wall
LDFLAGS=-lmysqlcppconn -L/usr/local/lib -lsctp -pthread -std=c++11
SOURCES=main.cpp Major_Tom/*.cpp Ground_control/*.cpp Packets/*.cpp Simulator/*.cpp EBU/*.cpp Netapi/cawl.cpp
EXECUTABLE=cawl

all: cawl

cawl: $(SOURCES)
	$(CC) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)

clean:
	find . -name \*.o -type f -delete
	rm -rf $(EXECUTABLE)
