server:
	g++ -std=c++11 -w main.cpp float_circular_buffer.cpp asynctcp.cpp arduino.cpp -lpthread -lboost_thread -lboost_system -I../serial_comunication/raspberry/ -o server.o
server_mac:
	g++ -std=c++11 -v ../serial_comunication/raspberry/raspduino.cpp main_server.cpp -lboost_thread-mt -lpthread -lboost_system -I/usr/local/include/ -L/usr/local/lib -I../serial_comunication/raspberry/ -o server.o -I.
