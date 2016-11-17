#include <iostream>
#include <boost/asio.hpp>
using namespace std;
using namespace boost::asio;
int main()	{
  io_service io;																//initialize services
  serial_port sp(io);								//create io object
  boost::system::error_code ec;



  sp.open("/dev/cu.usbmodemFD121",	ec);				//connect to	port
  if(	ec ){
    cout <<	"Error";
    return -1;
  }
  //set baud rate
  std::cout << "open port" << '\n';
  sp.set_option(serial_port_base::baud_rate(115200),ec);
  if(	ec ){
    cout <<	"Error";
    return -1;
  }
  sleep(2);
  string str_1 = "Who";

  std::cout << str_1 << endl ;
  //getline(cin,	str);
  //std::cout << "SENDING DATA" << '\n';
  //getline(cin,	str_1);
  int nbytes =	write(sp,	boost::asio::buffer(str_1));
  //std::cout << "SENDING DONE " << str_1;
  sleep(2);
  //std::cout << "print" << nbytes << " to serial port";
  for	(;;)
  {
    boost::asio::streambuf str;
    std::cout << "try to read" << '\n';
    read_until(sp,	str,	"\n");
    std::ostringstream ss;
    ss << &str;
    string cmp = ss.str();

    if (cmp.compare(0,12,	"I am arduino")	== 0){
        cout <<	"Este é o arduino 1" << endl ;
    }

  }
}
