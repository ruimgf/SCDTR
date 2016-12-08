#include <iostream>
#include <boost/asio.hpp>
#include "arduino.h"

using namespace std;
using namespace boost::asio;


int main(){
    io_service ard1_service;
    arduino ard1(ard1_service,"/dev/ttyACM0");
    ard1_service.run();
}
