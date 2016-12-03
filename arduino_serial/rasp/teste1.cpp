#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;

int main()	{
  io_service io;																//initialize services
  serial_port sp(io);								//create io object
  boost::system::error_code ec;
  sp.open("/dev/ttyACM0",	ec);				//connect to	port
  if(	ec )	{cout <<	"Error in OPEN";	return -1;}
  //set baud rate
  sp.set_option(serial_port_base::baud_rate(115200),ec);
  if(	ec )	{
    cout <<	"Error in SET";	return -1;
  }
  for	(;;)
  {
    boost::asio::streambuf str;
    read_until(sp,	str,	'\n');
    cout <<	&str;
  }
}