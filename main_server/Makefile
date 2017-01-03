server:
	g++ -std=c++11 main.cpp float_circular_buffer.cpp asynctcp.cpp arduino.cpp -lpthread -lboost_thread -lboost_system -I../serial_comunication/raspberry/ -o server.o
server_mac:
	g++ -std=c++11 -v ../serial_comunication/raspberry/raspduino.cpp main_server.cpp -lboost_thread-mt -lpthread -lboost_system -I/usr/local/include/ -L/usr/local/lib -I../serial_comunication/raspberry/ -o server.o -I.
async_client:
	g++ -std=c++11 asynclient.cpp -lpthread -lboost_system -o async_client.o -I/usr/local/include/ -L/usr/local/lib -I.
client_prof:
	g++ -std=c++11 client_prof.cpp -lpthread -lboost_system -o client_prof.o -I/usr/local/include/ -L/usr/local/lib -I.
