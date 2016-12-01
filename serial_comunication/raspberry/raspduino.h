

#ifndef RASPDUINO_H
#define	RASPDUINO_H
#include <iostream>
#include <boost/asio.hpp>


using namespace std;
using namespace boost::asio;

class raspduino{
  private:
  boost::asio::io_service io;
  boost::asio::serial_port sp{io};

  int id;
  public:
    raspduino(string);
    ~raspduino();
    string   send_mensage(string);
    void print_id();
    int get_lum();
    int get_duty();
    bool get_occup();
    int get_lower_bound();
    int get_ext_ilum();
    int get_reference();
    void read_state(int &,int &);
};

#endif //RASPDUINO_H
