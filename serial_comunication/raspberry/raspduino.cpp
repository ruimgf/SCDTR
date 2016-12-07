
#include <iostream>
#include <boost/asio.hpp>
#include "raspduino.h"


using namespace std;
using namespace boost::asio;

raspduino::raspduino():
        sp(io)
{
  li[0] = li[1] = li[2] = 0;
  E=0;
  V_f=0;  C_e=0;
  occupancy=0;
}

std::string raspduino::readLine()
    {
        //Reading data char by char, code is optimized for simplicity, not speed
        char c;
        std::string result;
        for(;;)
        {
            try{
              boost::asio::read(sp,boost::asio::buffer(&c,1));
              switch(c)
              {
                  case '\r':
                      break;
                  case '\n':
                      return result;
                  default:
                      result+=c;
              }
            }catch (boost::system::system_error &e)
            {
                usleep(1000);
            }

        }
    }


void raspduino::init(string port){
  serial_port_base::baud_rate BAUD(115200);
  serial_port_base::flow_control FLOW( serial_port_base::flow_control::none );
  serial_port_base::parity PARITY( serial_port_base::parity::none );
  serial_port_base::stop_bits STOP( serial_port_base::stop_bits::one );
  serial_port_base::character_size SIZE(8);

  boost::system::error_code ec;
  sp.open(port,	ec);				//connect to	port
  if(	ec ){
    cout <<	"Error Cannot Open Port" << endl;
    exit(-1);
  }

  sp.set_option( BAUD );
  sp.set_option( FLOW );
  sp.set_option( PARITY );
  sp.set_option( STOP );
  sp.set_option( SIZE );

  if(	ec ){
    cout <<	"Error Cannot set baud_rate" << endl;
    exit(-1);
  }

  usleep(3000000); // arduino reset when a connection is set, let wait a bit
  /*flush port*/

  string str_start = "w";
  write(sp,	boost::asio::buffer(str_start));
  for	(;;)
  {
    string data;
    try
    {
      data = readLine();
    }
    catch (boost::system::system_error &e)
    {
        boost::system::error_code ec =	e.code();
        std::cerr << "error in read setup:"<<	ec.message()	<<	std::endl;
    }



    if (data.compare(0,1,	"A")	== 0){
        string number;
        number =  {data,2};
        id = atoi( number.c_str() );
        break;
    } //break;
  }

  std::cout << id << '\n';
}

raspduino::~raspduino(){
  try
  {
    sp.close();
  }
  catch (boost::system::system_error &e)
  {
      boost::system::error_code ec =	e.code();
      std::cerr << "error in close:"<<	ec.message()	<<	std::endl;

  }
}

string raspduino::send_mensage(string str){
  string data;
  write(sp,	boost::asio::buffer(str));

  data = readLine();

  return data;

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
  //std::cout << mensage << '\n';

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

   string ss;


     try
     {
       ss = readLine();
     }
     catch (boost::system::system_error &e)
     {
         boost::system::error_code ec =	e.code();
         std::cout << "error in read state:  " ;
         std::cerr <<	ec.message()	<<	std::endl;
     }

     string response = ss;
     if(ss.compare(0,1,	"l")==0){
       string response = ss;
       int index =0;
       for(int i=0;i<response.length();i++){
         if(response[i]=='|'){
           index=i;
           break;
         }
       }
       string response_lum{ss,1,index-1};
       string response_duty{ss,index+1};

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
