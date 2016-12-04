#include "arduino.h"

using namespace std;

int main() {
  arduino ad1{"/dev/ttyACM0"};
  int i =0;
  while(i<100){
    ad1.retrive_control_loop_data();
    i++;
  }
}
