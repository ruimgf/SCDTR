#include "tcp_server.h"


using namespace std;
using namespace boost::asio;
using namespace boost::asio;
using ip::tcp;


tcp_server::tcp_server(){
  acceptor.open(endpoint.protocol());
  acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor.bind(endpoint);
  acceptor.listen();
}

void tcp_server::attep_conn(){
   //create service socket
   std::cout << "create socket" << '\n';
   tcp::socket socket(io);
   //wait client to	connect
   std::cout << "go accept" << '\n';
   acceptor.accept(socket);
   std::cout << "accept" << '\n';
   //client is accessing service
   for (;;) {
     
     write(socket,	buffer("Hello World\n"));
   }


 }
