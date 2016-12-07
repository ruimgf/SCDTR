#include <iostream>
#include <boost/asio.hpp>
#include "raspduino.h"

using namespace std;
using namespace boost::asio;


int main(){
    raspduino ard;
    ard.init("/dev/ttyACM0");
    ard.print_id();


    int i = 0;
    while (1) {
      i++;
      ard.read_state();
      ard.printvalues();
      if(i==10){
        std::cout << "go get duty" << endl;
        ard.change_ocp(0);
      }
      if(i==1000){
        break;
      }

    }

}
