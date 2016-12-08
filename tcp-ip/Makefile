async:
	g++ -std=c++11 main.cpp asynctcp.cpp -lpthread -lboost_system -o async_tcp.o -I/usr/local/include/ -L/usr/local/lib -I.

server:
	g++ -std=c++11 main.cpp tcp_server.cpp -lpthread -lboost_system -o tcp_server.o -I/usr/local/include/ -L/usr/local/lib -I.

client:
	g++ -std=c++11 client.cpp -lpthread -lboost_system -o tcp_client.o -I/usr/local/include/ -L/usr/local/lib -I.
