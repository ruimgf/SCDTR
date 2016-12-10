#ifndef ARDUINO_H
#define	ARDUINO_H
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <sstream>
#include "float_circular_buffer.h"
#include <boost/bind.hpp>
#include <chrono>
#include "globals.h"

#define DEFAULT_OCCUP_LUX_REF 20;
#define DEFAULT_UNCUP_LUX_REF 10;

using namespace boost::asio;
using namespace boost::system;
using boost::asio::ip::tcp;

class arduino{
  private:
  boost::asio::io_service& io;
  boost::asio::serial_port sp;
  boost::asio::deadline_timer tim_setup;
  boost::asio::streambuf read_buf;
  std::vector<tcp_session *> cli_stream_duty;
  std::vector<tcp_session *> cli_stream_lux;
  std::chrono::time_point<std::chrono::system_clock> begin_time;
  int id;
  unsigned long int N = 0;
  float ts; //indicates the # of the current sample;
  float duty;
  float lower_lux;
  float lux[3]; // li[0] - li, li[1] - l_(i-1),li[2] - l_(i-2) // lux at time i
  float C_e; // confort_error
  float E; // energy
  float V_f; // vflicker
  float P_i;
  bool occupancy;
  float_circular_buffer last_lux;//stores last minute history;
  float_circular_buffer last_duty;//stores last_minute history;
  float_circular_buffer last_ts;//sotres last minuit history of ts;
  float ref_lux;

  void timer_handler(const boost::system::error_code& ec);
  void read_setup_handler(const boost::system::error_code& ec);
  void begin_timer_handler(const boost::system::error_code& ec);
  void read_handler(const boost::system::error_code& ec);
  void write_ocp_handler(const boost::system::error_code& ec);
  void save_value(float duty_mes, float lux_mes, int time_stamp);
  public:
    arduino(io_service& io_,std::string port_name);
    ~arduino();
    void change_ocp(bool);
    float get_current_lux();
    bool get_occupancy();
    float get_current_duty();
    std::vector<float> get_oneminute_duty();
    std::vector<float> get_oneminute_lux();
    float get_reference();
    float get_low_lux();
    float get_power();
    float get_energy();
    float get_error_confort();
    float get_variance();
    void attachclistream_lux(tcp_session*);
    void attachclistream_duty(tcp_session*);
    void detachclistream_lux(tcp_session*);
    void detachclistream_duty(tcp_session*);
    void reset();
};

#endif //ARDUN
