CC=c++
CFLAGS=-c -Wall
LDFLAGS=-lmysqlcppconn -L/usr/local/lib -lsctp -pthread -std=c++11
SOURCES=main.cpp Major_Tom/Mobile.cpp Ground_control/Ground.cpp Packets/AnalogOut.cpp Packets/RelayOut.cpp Packets/SimPack.cpp Simulator/Sim.cpp EBU/EBUManager.cpp Packets/DigitalOut.cpp Packets/ImuPack.cpp Packets/DigitalIn.cpp Packets/AnalogIn.cpp
EXECUTABLE=cawl

all: cawl

cawl: $(SOURCES)
	$(CC) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)

clean:
	find . -name \*.o -type f -delete
	rm -rf $(EXECUTABLE)
