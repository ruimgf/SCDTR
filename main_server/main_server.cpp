#include <iostream>
#include <boost/asio.hpp>
#include	<algorithm>
#include	<vector>

#include "raspduino.h"
#include "defs.h"

raspduino arduinos[NUMBER_ILLUM];

using namespace std;
using namespace boost::asio;


int main(){

  // replace for a system call ls searching for all ports avaliable
  arduinos[0].init(PORT_ILLUM0);

  while(1){
    arduinos[0].read_state();
    arduinos[0].printvalues();
  }


}
