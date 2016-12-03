#ifndef ARDUINO_H
#define	ARDUINO_H
#include <iostream>
#include <boost/asio.hpp>
#include "float_circular_buffer.h"

using namespace std;

class arduino{
  private:
  boost::asio::io_service io;
  boost::asio::serial_port sp{io};
  int id;
  float duty;
  float li[3]; // li[0] - li, li[1] - l_(i-1),li[2] - l_(i-2) // lux at time i
  float E; // energy
  float V_f; // vflicker
  float C_e; // confort_error
  bool occupancy;
  float ref;
  float_circular_buffer last_lum{100};//tamanho do buffer = 100 pois o nosso control loop = 10 ms
  float_circular_buffer last_duty{100};

  public:
    arduino();
    ~arduino();
    std::string read_serial(boost::asio::serial_port port);

};

#endif //ARDUINO_H
