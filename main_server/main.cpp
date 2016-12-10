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

void ard_thread_1(){
  try
  {
    ard1_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}

void ard_thread_2(){
  try
  {
    ard2_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}

int main(int argc, char *argv[]){
    if(argc < 3 ){
      std::cout << "usage server.o [ard1 port] [ard2 port]" << '\n';
      exit(0);
    }
    shared_ptr <arduino> ard1 (new arduino{ard1_service,argv[1]});
    shared_ptr <arduino> ard2 (new arduino{ard2_service,argv[2]});
    ard.push_back(ard1);
    ard.push_back(ard2);
    std::thread t1{ard_thread_1};
    std::thread t2{ard_thread_2};
    std::thread t3{tcp};
    //ard1_service.run();
    while (1) {
      string command;
      getline(std::cin,command);
      if(command=="quit"){
        io_tcp.stop();
        ard1_service.stop();
        ard2_service.stop();
        exit(0);
      }

    }
}
