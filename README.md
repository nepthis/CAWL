# README - CAWL_nxt#
 
# Summary #

CAWL_nxt is the continuation a software developed as a bachelor thesis work at Luleå Tekniska Universitet (LTU) with the goal of remote controlling a Volvo L110G wheel loader. It was developed as a proof of concept and was never thought to be used for further development. The project uses a hardware platform developed at LTU to communicate with an electronic breakout unit (EBU) and a simulator developed by Oryx as a interface.
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