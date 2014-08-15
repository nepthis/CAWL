CC=c++
CFLAGS=-c -Wall
LDFLAGS=-lmysqlcppconn -L/usr/local/lib -lsctp -pthread -std=c++11
SOURCES=main.cpp Major_Tom/Mobile.cpp Ground_control/Ground.cpp Packets/EBUPacketAnalogOut.cpp Packets/CawlPacket.cpp Packets/EBURelayPacket.cpp Packets/SimPack.cpp Netapi/CawlSocket.cpp Netapi/GatherMetrics.cpp Netapi/Host.cpp Simulator/Sim.cpp EBU/EBUManager.cpp Db/mysqlconnector.cpp
EXECUTABLE=cawl

all: cawl

cawl: $(SOURCES)
	$(CC) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)



clean:
	find . -name \*.o -type f -delete
	rm -rf $(EXECUTABLE)
