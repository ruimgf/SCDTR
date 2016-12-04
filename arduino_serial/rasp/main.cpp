#include "arduino.h"

using namespace std;

int main() {
  /* code */
  arduino ard1{"/dev/ttyACM0"};
  while (1) {
    /* code */
    ard1.retrive_control_loop_data();
  }
  return 0;
}
