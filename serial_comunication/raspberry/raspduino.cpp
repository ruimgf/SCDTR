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

  li[0] = li[1] = li[2] = 0;
  E=0;
  V_f=0;
  C_e=0;
  occupancy=0;

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


void raspduino::change_ocp(bool occupancy){

  boost::asio::streambuf buf;
  string mensage{"co"};
  if(occupancy!=0){
    mensage += "O"; // ocupate
  }else{
    mensage += "N"; // not ocupate
  }
  std::cout << mensage << '\n';

  write(sp,	boost::asio::buffer(mensage));

}

float raspduino::get_reference(){

    if(occupancy==0){
      return DEFAULT_UNCUP_LUX_REF;
    }else{
      return DEFAULT_OCCUP_LUX_REF;
    }

}

 void raspduino::read_state(int & lummens, int & duty ){
   std::ostringstream ss;
   boost::asio::streambuf buf;

     read_until(sp,	buf,	"\n");
     ss << &buf;
     if(ss.str().compare(0,1,	"l")==0){
       string response_lum{ss.str(),1};
       lummens = atoi(response_lum.c_str());
     }

     read_until(sp,	buf,	"\n");
     ss << &buf;
     if(ss.str().compare(0,1,	"d")==0){
       string response_duty{ss.str(),1};
       duty = atoi(response_duty.c_str());
     }
 }
