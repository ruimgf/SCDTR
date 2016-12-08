#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "arduino.h"

using namespace std;
using namespace boost::asio;

io_service ard1_service;
arduino ard1(ard1_service,"/dev/ttyACM0");

void read_keyboard(){
  while (1) {
    string command;
    getline(std::cin,command);
    if(command=="quit"){
      exit(0);
    }
    if(command=="c"){
      ard1.change_ocp(0);
    }
  }

}

int main(){
    std::thread t1{read_keyboard};
    ard1_service.run();
}
