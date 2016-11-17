#include <iostream>
#include <boost/asio.hpp>
#include "raspduino.h"

using namespace std;
using namespace boost::asio;


int main(){
    raspduino ard{"/dev/cu.usbmodemFD121"};
    string str;
    ard.print_id();
    cout << str;
    cout << ard.get_duty();
}
