

#ifndef RASPDUINO_H
#define	RASPDUINO_H
#include <iostream>
#include <boost/asio.hpp>

#define DEFAULT_OCCUP_LUX_REF = 20;
#define DEFAULT_UNCUP_LUX_REF = 10;

using namespace std;
using namespace boost::asio;

class raspduino{
  private:
  boost::asio::io_service io;
  boost::asio::serial_port sp{io};

  int id;
  float li[3]; // li[0] - li, li[1] - l_(i-1),li[2] - l_(i-2) // lux at time i
  float E=0; // energy
  float V_f=0; // vflicker
  //float ref=0;
  float C_e=0; // confort_error
  bool occupancy=0;
  public:
    raspduino(string);
    ~raspduino();
    string   send_mensage(string);
    void print_id();
    int get_lum();
    int get_duty();
    void change_ocp(bool);
    int get_lower_bound();
    int get_ext_ilum();
    float get_reference();
    void read_state(int &,int &);
};

#endif //RASPDUINO_H
