#include <iostream>
#include <boost/asio.hpp>
#include "raspduino.h"


using namespace std;
using namespace boost::asio;

raspduino::raspduino(string port){
  boost::system::error_code ec;
  sp.open(port,	ec);				//connect to	port
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

  string str_start = "Who";

  write(sp,	boost::asio::buffer(str_start));

  for	(;;)
  {
    boost::asio::streambuf str;
    string number;
    read_until(sp,	str,	"\n");
    std::ostringstream ss;
    ss << &str;
    string msa = ss.str();

    if (msa.compare(0,12,	"I am arduino")	== 0){
        number =  {msa,13};
        id = atoi( number.c_str() );
        break;
    }
  }

}

raspduino::~raspduino(){
  boost::system::error_code ec;
  sp.close(ec);
}

string raspduino::send_mensage(string str){

  boost::asio::streambuf buf;
  write(sp,	boost::asio::buffer(str));

  read_until(sp,	buf,	"\n");

  std::ostringstream ss;
  ss << &buf;
  return ss.str();

}

void raspduino::print_id(){
    cout << id << endl;
}

int raspduino::get_lum(){

  boost::asio::streambuf buf;
  write(sp,	boost::asio::buffer("g l"));

  read_until(sp,	buf,	"\n");

  std::ostringstream ss;
  ss << &buf;
  string response{ss.str(),2};
  return atoi(response.c_str());

}


int raspduino::get_duty(){
  boost::asio::streambuf buf;

  write(sp,	boost::asio::buffer("g d"));

  read_until(sp,	buf,	"\n");

  std::ostringstream ss;
  ss << &buf;
  string response{ss.str(),2};

  return atoi(response.c_str());

}
bool raspduino::get_occup(){

  boost::asio::streambuf buf;
  write(sp,	boost::asio::buffer("g o"));

  read_until(sp,	buf,	"\n");

  std::ostringstream ss;
  ss << &buf;

  string response{ss.str(),2};
  return atoi(response.c_str());

}

int raspduino::get_lower_bound(){

  boost::asio::streambuf buf;
  write(sp,	boost::asio::buffer("g L"));

  read_until(sp,	buf,	"\n");

  std::ostringstream ss;
  ss << &buf;

  string response{ss.str(),2};
  return atoi(response.c_str());

}

int raspduino::get_ext_ilum(){
  boost::asio::streambuf buf;
  write(sp,	boost::asio::buffer("g O"));

  read_until(sp,	buf,	"\n");

  std::ostringstream ss;
  ss << &buf;
  string response{ss.str(),2};
  return atoi(response.c_str());


}
int raspduino::get_reference(){
  boost::asio::streambuf buf;
  write(sp,	boost::asio::buffer("g r"));

  read_until(sp,	buf,	"\n");

  std::ostringstream ss;
  ss << &buf;

  string response{ss.str(),2};
  return atoi(response.c_str());

}
