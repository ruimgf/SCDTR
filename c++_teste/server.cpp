#include <boost/asio.hpp>
using namespace boost::asio;
using	ip::tcp;
int main()
{
//initialize services
  io_service io;
//create a	listening socket
//	bind and startlistening at port 10000
  tcp::acceptor acceptor(io,tcp::endpoint(tcp::v4(),	10000));
  for	(;;){
  //create service socket
  tcp::socketsocket(io);
  //wait client to	connect
  tcp::acceptor.accept(socket);
  //client is accessing service
  write(socket,	buffer(“Hello World\n”));
  }
}
