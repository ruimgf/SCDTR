#include <iostream>
#include <boost/asio.hpp>
#include	<algorithm>
#include	<vector>

#include "raspduino.h"
#include "defs.h"



using namespace std;
using namespace boost::asio;


int main(){

  // raspduino	myarduinos[0]{};
  raspduino arduinos[NUMBER_ILLUM];

  // replace for a system call ls searching for all ports avaliable
  arduinos[0].init(PORT_ILLUM0);

  while(1){
    arduinos[0].read_state();
    arduinos[0].printvalues();
  }


}
