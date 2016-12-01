

#ifndef RASPDUINO_H
#define	RASPDUINO_H
#include <iostream>
#include <boost/asio.hpp>

#define DEFAULT_OCCUP_LUX_REF 20;
#define DEFAULT_UNCUP_LUX_REF 10;

using namespace std;
using namespace boost::asio;

class raspduino{
  private:
  boost::asio::io_service io;
  boost::asio::serial_port sp{io};

  int id;
  float li[3]; // li[0] - li, li[1] - l_(i-1),li[2] - l_(i-2) // lux at time i
  float E; // energy
  float V_f; // vflicker
  float duty;
  //float ref=0;
  float C_e; // confort_error
  bool occupancy;
  public:
    raspduino();
    ~raspduino();
    string   send_mensage(string);
    void print_id();
    void change_ocp(bool);
    float get_reference();
    void read_state();
    void init(string port);
    void printvalues();
};

#endif //RASPDUINO_H
