#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "arduino.h"
#include "asynctcp.h"

using namespace std;
using namespace boost::asio;
std::vector<std::shared_ptr <arduino>> ard;

io_service ard1_service;
io_service ard2_service;
boost::asio::io_service io_tcp;
shared_ptr <arduino> ard1 (new arduino{ard1_service,"/dev/ttyACM0"});
//shared_ptr <arduino> ard2 (new arduino{ard2_service,"/dev/ttyACM1"});
void read_keyboard(){
  while (1) {
    string command;
    getline(std::cin,command);
    if(command=="quit"){
      exit(0);
    }

    if(command=="c"){
     std::cout << ard1->get_current_lux() << '\n';
    }
  }
}
void tcp(){
  try
  {
    tcp_server s(io_tcp, 17000);
    io_tcp.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}

void ard_thread(){
  try
  {
    ard2_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}


int main(){
    ard.push_back(ard1);
    //ard.push_back(ard2);
    std::thread t1{read_keyboard};
    std::thread t2{tcp};
    //std::thread t3{ard};
    ard1_service.run();
}
