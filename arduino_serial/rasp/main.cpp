#include <SerialPort.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

int main() {
  /* code */
  SerialPort ard("/dev/ttyACM0");
  ard.Open(SerialPort::BaudRate::BAUD_115200);
  string data_s;

  cout << "blabla" << endl ;
  std::this_thread::sleep_for (std::chrono::seconds(5));
  cout << "blabla" << endl;

  int i = 0;
  while(i<10){

    data_s = ard.ReadLine();
    cout << data_s ;
    i=i+1;

  }
  ard.Close();
}
