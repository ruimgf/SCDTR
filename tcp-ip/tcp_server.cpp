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
   tcp::socket socket(io);
   //wait client to	connect
   acceptor.accept(socket);
   //client is accessing service
   write(socket,	buffer("Hello World\n"));
 }
