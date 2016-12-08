#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "arduino.h"
#include "asynctcp.h"
using namespace std;
using namespace boost::asio;

io_service ard1_service;
boost::asio::io_service io_tcp;
arduino * ard1 (new arduino{ard1_service,"/dev/ttyACM0"});

void read_keyboard(){
  while (1) {
    string command;
    getline(std::cin,command);
    if(command=="quit"){
      exit(0);
    }
    /*
    if(command=="c"){
      ard1.change_ocp(0);
    }*/
  }
}
void tcp(){
  try
  {
    tcp_server s(io_tcp, 17000,ard1);
    io_tcp.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}


int main(){
    std::thread t1{read_keyboard};
    std::thread t2{tcp};
    ard1_service.run();
}
