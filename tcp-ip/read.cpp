#include <iostream>
#include <boost/asio.hpp>
#include "raspduino.h"

using namespace std;
using namespace boost::asio;


int main(){
    raspduino ard{"/dev/cu.usbmodemFD121"};
    ard.print_id();
    cout << ard.get_duty() << endl;
    cout << ard.get_ext_ilum() << endl;
    cout << ard.get_lower_bound() << endl;
    cout << ard.get_lum() << endl;
    cout << ard.get_occup() << endl;
    cout << ard.get_reference() << endl;
}
