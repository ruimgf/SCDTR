#include <iostream>
#include <boost/asio.hpp>
#include "arduino.h"


using namespace std;
using namespace boost::asio;

/*lê dados de uma porta serial e retorna os dados numa std::string*/
string arduino::read_serial(serial_port port){
  boost::asio::streambuf data_boost_buffer;
  read_until(sp,	data_buffer,	"\n");
  ostringstream data_buffer;
  data_buffer << &data_boost_buffer;
  string data = data_buffer.str();
  return data;
}

arduino::arduino(string port){
  li[0] = li[1] = li[2] = 0;
  E=0;
  V_f=0;
  C_e=0;
  occupancy=0;
  ref = 0;

  //connect to	port
  boost::system::error_code ec;
  sp.open(port,	ec);
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

  sleep(2); // arduino reset when a connection is set, let wait a bit

  /*asks for id*/
  string str_start = "W";
  write(sp,	boost::asio::buffer(str_start));
  /*receive id*/
  string data = read_serial(sp);
  id = stoi(data);
  std::cout << id << endl;

}

arduino::~arduino(){
  boost::system::error_code ec;
  sp.close(ec);
}
