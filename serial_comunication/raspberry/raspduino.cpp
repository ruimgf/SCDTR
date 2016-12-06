#include <iostream>
#include <boost/asio.hpp>
#include "raspduino.h"


using namespace std;
using namespace boost::asio;

raspduino::raspduino(){


  li[0] = li[1] = li[2] = 0;
  E=0;
  V_f=0;  C_e=0;
  occupancy=0;
}

void raspduino::init(string port){

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

  sleep(3); // arduino reset when a connection is set, let wait a bit

  string str_start = "w";

  write(sp,	boost::asio::buffer(str_start));
  sleep(1);
  for	(;;)
  {
    boost::asio::streambuf str;
    string number;
    try
    {
      read_until(sp,	str,	"\n");
    }
    catch (boost::system::system_error &e)
    {
        boost::system::error_code ec =	e.code();
        std::cerr << "error in set:"<<	ec.value()	<<	std::endl;

    }

    std::cout << "after read until" << '\n';
    std::ostringstream ss;
    ss << &str;
    string msa = ss.str();

    if (msa.compare(0,1,	"A")	== 0){
        number =  {msa,2};
        id = atoi( number.c_str() );
        break;
    }
    //break;
  }

  std::cout << id << '\n';
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

 void raspduino::read_state(){
   std::ostringstream ss;
   boost::asio::streambuf buf;


     try
     {
       read_until(sp,	buf,	"\n");
     }
     catch (boost::system::system_error &e)
     {
         boost::system::error_code ec =	e.code();
         std::cout << "error:  " ;
         std::cerr <<	ec.value()	<<	std::endl;
     }

     ss << &buf;
     string response = ss.str();
     if(ss.str().compare(0,1,	"l")==0){
       string response = ss.str();
       int index =0;
       for(int i=0;i<response.length();i++){
         if(response[i]=='|'){
           index=i;
           break;
         }
       }
       string response_lum{ss.str(),1,index-1};
       string response_duty{ss.str(),index+1};

       duty = atoi(response_duty.c_str());
       li[2] = li[1];
       li[1] = li[0];
       li[0] = atoi(response_lum.c_str());
    }

 }

 void raspduino::printvalues(){

   std::cout << "lux " << li[0] << '\n';
   std::cout << "duty " << duty << '\n';

 }
