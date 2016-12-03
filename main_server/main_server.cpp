#include <iostream>
#include <boost/asio.hpp>
#include	<algorithm>
#include	<vector>
#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

#include "raspduino.h"
#include "defs.h"

raspduino arduinos[NUMBER_ILLUM];

using namespace std;
using namespace boost::asio;

void thread_session(int arduino_number){

  while(1){

      arduinos[arduino_number].read_state();
      arduinos[arduino_number].printvalues();

  }


}


int main(){
  int i;
  // replace for a system call ls searching for all ports avaliable
  arduinos[0].init(PORT_ILLUM0);
  
  boost::thread t(boost::bind(thread_session,0));

  t.join();
}
