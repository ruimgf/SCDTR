#include <SerialPort.h>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;

int main() {
  /* code */
  SerialPort ard("/dev/ttyACM0");
  ard.Open(SerialPort::BaudRate::BAUD_115200);
  string data_s;

  cout << "blabla" << endl ;
  sleep(2);
  cout << "blabla" << endl;

  int i = 0;
  while(i<10){

    data_s = ard.ReadLine();
    cout << data_s ;
    i=i+1;

  }
  ard.Close();
}
