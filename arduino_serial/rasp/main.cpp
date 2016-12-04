#include <SerialPort.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>

using namespace std;

int main() {
  /* code */
  SerialPort ard("/dev/ttyACM0");
  ard.Open(SerialPort::BaudRate::BAUD_115200);
  string data_s;
  std::chrono::time_point<std::chrono::system_clock> start, end;

  /*
  cout << "blabla" << endl ;
  cout.flush();
  start = std::chrono::system_clock::now();
  while(1){
    std::this_thread::sleep_for(std::chrono::nanoseconds(1000000));
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    if(elapsed_seconds.count() >= 2){
      break;
    }
  }
  cout << "blabla" << endl;
  */

  int i = 1;
  while(i<100){
    data_s = ard.ReadLine();
    cout << data_s ;
    data_s = ard.ReadLine();
    cout << data_s ;
    data_s = ard.ReadLine();
    cout << data_s ;
    cout << endl;
    i = i+1;
  }
  ard.Close();
}
