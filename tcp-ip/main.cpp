#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "tcp_server.h"

using namespace boost::asio;
using ip::tcp;

int main(){

  boost::asio::io_service io;
  boost::asio::ip::tcp::acceptor acceptor(io);

  for (;;) {
    tcp::socket socket(io);
    //wait client to	connect
    acceptor.accept(socket);
    //client is accessing service
    write(socket,	buffer("Hello World\n"));
  }

}
