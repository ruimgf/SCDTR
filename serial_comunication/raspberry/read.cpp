#include <iostream>
#include <boost/asio.hpp>
#include "raspduino.h"

using namespace std;
using namespace boost::asio;


int main(){
    raspduino ard;
    ard.init("/dev/ttyACM0");
    ard.print_id();
    int lum;
    int duty;
    int i = 0;
    while (1) {
      i++;
      ard.printvalues();
      /*if(i%100==0){
        //std::cout << "go get duty" << '\n';
        ard.change_ocp(0);
      }*/
    }

}
