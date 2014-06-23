CC=c++
CFLAGS=-c -Wall
LDFLAGS=-lmysqlcppconn -L/usr/local/lib -lsctp -pthread -std=c++11
SOURCES=main.cpp Major_Tom/Mobile.cpp Ground_control/Ground.cpp Packets/EBUPacketAnalogOut.cpp Packets/CawlPacket.cpp Packets/EBURelayPacket.cpp Packets/SimPack.cpp Netapi/CawlSocket.cpp Netapi/GatherMetrics.cpp Netapi/Host.cpp Simulator/Sim.cpp EBU/EBUManager.cpp Db/mysqlconnector.cpp
EXECUTABLE=cawl

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(SOURCES) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	find . -name \*.o -type f -delete
	rm $(EXECUTABLE)
