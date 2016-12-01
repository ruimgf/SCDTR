#include <iostream>
#include <boost/asio.hpp>
#include "raspduino.h"

using namespace std;
using namespace boost::asio;


int main(){
    raspduino ard{"/dev/cu.usbmodemFD121"};
    ard.print_id();
    int lum;
    int duty;
    int i = 0;
    while (1) {
      i++;
      ard.read_state(lum,duty);
      std::cout << "lum" << lum << '\n';
      std::cout << "duty" << duty << '\n';
      std::cout << i << '\n';
      if(i%100==0){
        //std::cout << "go get duty" << '\n';
        ard.change_ocp(0);
      }
    }

}
