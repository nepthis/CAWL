# README - CAWL v.03#
 
# Summary #

CAWL is a software developed as a bachelor thesis work at Luleå Tekniska Universitet (LTU) with the goal of remote controlling a Volvo L120G wheel loader. It was developed as a proof of concept and was never thought to be used for further development. CAWL uses a hardware platform developed at LTU to communicate with the wheel loader called electronic breakout unit (EBU) and a simulator developed by Oryx as a interface.


# Dependencies #

CAWL was tested on Ubuntu 12.04 LTS 32 bit

Libraries used are:
-lmysqlcppconn
-L/usr/local/lib -lsctp 
-pthread
-std=c++11

Mysqlcppconn can be installed with apt-get, <sudo apt-get install  libmysqlcppconn-dev>. This will also install needed boost librarys etc.

Lksctp can be installed with apt-get, <sudo apt-get install libsctp-dev lksctp-tools>.

CAWL was compiled with GCC 4.7

To be able to log measurement-data CAWL needs a MYSQL-database to log data.


# Configuration #

To be able to use CAWL it is necesarry to use specifik IP-addresses. The EBU sends and recieves it´s data to IP-address 10.0.0.1 and it is important that ethernet on mobile side is set to this aadress.

The simulator use IP-address 192.168.2.97 and sends and recieves data at IP-address 192.168.2.100. It is thus important to set the static IP-address 192.168.2.100 on ground side computer.


# Database Configuration #

If the measurement is to be used CAWL needs a MYSQL-database to log mesurementdata to.

The credentials and other necessary information about the database can be found in Db/mysqlconnector.h


# How to run CAWL #

Cawl is started either as mobile or ground depending if the program is running at the wheel loader side or the simulator side.

To start CAWL at the wheel loader side use the command <sudo ./cawl mobile>

To start CAWL at the simulator side use the command <sudo ./cawl ground>