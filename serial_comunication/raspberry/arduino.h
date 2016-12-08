#ifndef ARDUINO_H
#define	ARDUINO_H
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <sstream>
#include "float_circular_buffer.h"
#include <boost/bind.hpp>


#define DEFAULT_OCCUP_LUX_REF 20;
#define DEFAULT_UNCUP_LUX_REF 10;

using namespace boost::asio;
using namespace boost::system;

class arduino{
  private:
  boost::asio::io_service& io;
  boost::asio::serial_port sp;
  boost::asio::deadline_timer tim_setup;
  boost::asio::streambuf read_buf;

  int id;
  float ts; //indicates the # of the current sample;
  float duty;
  float lux[3]; // li[0] - li, li[1] - l_(i-1),li[2] - l_(i-2) // lux at time i
  float C_e; // confort_error
  float E; // energy
  float V_f; // vflicker
  bool occupancy;
  float_circular_buffer last_lux;//stores last minute history;
  float_circular_buffer last_duty;//stores last_minute history;
  float_circular_buffer last_ts;//sotres last minuit history of ts;
  float ref_lux;

  void timer_handler(const error_code& ec);
  void write_handler_setup(const error_code& ec);
  void dummy_read(const error_code& ec);
  void read_setup_handler(const error_code& ec);
  void read_control_loop_handler(const error_code& ec);

  public:
    arduino(io_service& io_,std::string port_name);
    ~arduino();
};

#endif //ARDUN
