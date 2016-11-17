#include <iostream>
#include <boost/asio.hpp>
using namespace std;
using namespace boost::asio;

class raspduino{
  private:
  boost::asio::io_service io;
  boost::asio::serial_port sp{io};
  boost::system::error_code ec;
  public:
    raspduino();
    string   send_mensage(string);

};

raspduino::raspduino(){
  sp.open("/dev/cu.usbmodemFD121",	ec);				//connect to	port
  if(	ec ){
    cout <<	"Error";
    exit(-1);
  }
  std::cout << "open port" << '\n';
  sp.set_option(serial_port_base::baud_rate(115200),ec);
  if(	ec ){
    cout <<	"Error";
    exit(-1);
  }
  sleep(2);

  string str_1 = "Who";

  write(sp,	boost::asio::buffer(str_1));

  for	(;;)
  {
    boost::asio::streambuf str;
    read_until(sp,	str,	"\n");
    std::ostringstream ss;
    ss << &str;
    string msa = ss.str();

    if (msa.compare(0,12,	"I am arduino")	== 0){
        cout <<	"Este é o arduino 1" << endl ;
        break;
    }
  }

}

string raspduino::send_mensage(string str){

  boost::asio::streambuf buf;
  write(sp,	boost::asio::buffer(str));
  read_until(sp,	buf,	"\n");
  std::ostringstream ss;
  ss << &buf;
  return ss.str();

}

int main(){
    raspduino ard;
    string str;
    str = ard.send_mensage("L");
    cout << str;
}
