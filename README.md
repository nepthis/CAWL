# README - CAWL_nxt#
 
# Summary #

CAWL_nxt is the continuation a software developed as a bachelor thesis work at Luleå Tekniska Universitet (LTU) with the goal of remote controlling a Volvo L120G wheel loader. It was developed as a proof of concept and was never thought to be used for further development. The project uses a hardware platform developed at LTU to communicate with an electronic breakout unit (EBU) and a simulator developed by Oryx as a interface.
In this project more functions will be added together with more stability etc. 


# Dependencies #

The main application was tested on Ubuntu 12.04 LTS 32 bit

Libraries used are:
-lmysqlcppconn
-L/usr/local/lib -lsctp 
-pthread
-std=c++11

Mysqlcppconn can be installed with apt-get, <sudo apt-get install  libmysqlcppconn-dev>. This will also install needed boost librarys etc.

Lksctp can be installed with apt-get, <sudo apt-get install libsctp-dev lksctp-tools>.

The program was compiled with GCC 4.7

To be able to log measurement-data a MYSQL-database is needed.


# Configuration #

To be able to use the main program it is necessary to use specific IP-addresses. The EBU sends and receives it´s data to IP-address 10.0.0.1 and it is important that Ethernet on mobile side is set to this address.

The simulator use IP-address 192.168.2.97 and sends and receives data at IP-address 192.168.2.100. It is thus important to set the static IP-address 192.168.2.100 on ground side computer.


# Database Configuration #

If the measurement is to be used the CAWL socket needs a MYSQL-database to log the measurementdata into.
The credentials and other necessary information about the database can be found in Db/mysqlconnector.h


# How to run the program #

The main program is started either as mobile or ground depending if the program is running at the wheel loader side or the simulator side.

To start CAWL at the wheel loader side use the command <sudo ./cawl mobile>

To start CAWL at the simulator side use the command <sudo ./cawl ground>