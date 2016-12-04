#ifndef ARDUINO_H
#define	ARDUINO_H
#include <iostream>
#include "float_circular_buffer.h"
#include <string.h>
#include "SerialPort.h"

using namespace std;

class arduino{
  private:
    /*atributes*/
  SerialPort ard;
  int id;
  float duty;
  float lux[3]; // lux[0] - li, lux[1] - l_(i-1),lux[2] - l_(i-2) // lux at time i
  float E; // energy
  float V_f; // vflicker
  float C_e; // confort_error
  bool occupancy;
  float ref;
  float_circular_buffer last_lum{100};//tamanho do buffer = 100 pois o nosso control loop = 10 ms
  float_circular_buffer last_duty{100};

  public:
    arduino(std::string);
    ~arduino();
    void retrive_control_loop_data();


};

#endif //ARDUINO_H
