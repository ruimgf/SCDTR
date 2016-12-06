#include <iostream>
#include <boost/asio.hpp>
#include "raspduino.h"

using namespace std;
using namespace boost::asio;


int main(){
    raspduino ard;
    ard.init("/dev/ttyACM3");
    ard.print_id();


    int i = 0;
    while (1) {
      i++;
      usleep(10000);
      ard.read_state();
      ard.printvalues();
      if(i==1000){
        //std::cout << "go get duty" << '\n';
        ard.change_ocp(0);
      }
      if(i==10000){
        break;
      }

    }

}
