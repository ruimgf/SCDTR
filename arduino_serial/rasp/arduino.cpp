#include "arduino.h"

using namespace std;
using namespace boost::asio;

/*lê dados de uma porta serial e retorna os dados numa std::string*/
string arduino::read_serial(){
  boost::asio::streambuf data_boost_buffer;
  read_until(sp,	data_boost_buffer,	"\n");
  ostringstream data_buffer;
  data_buffer << &data_boost_buffer;
  string data = data_buffer.str();
  return data;
}

arduino::arduino(string port_name){
  lux[0] = lux[1] = lux[2] = 0;
  duty = 0;
  E=0;
  V_f=0;
  C_e=0;
  occupancy=0;
  ref = 0;

  //connect to	port
  boost::system::error_code ec;
  sp.open(port_name,	ec);
  if(	ec ){
    cout <<	"Error";
    exit(-1);
  }
  std::cout << "open port" << '\n';
  sp.set_option(serial_port_base::baud_rate(115200),ec);
  if(	ec ){
    cout <<	"Error";
    exit(-1);
  }

  sleep(2); // arduino reset when a connection is set, let wait a bit

  /*asks for id*/
  string str_start = "W";
  write(sp,	boost::asio::buffer(str_start));
  /*receive id*/
  string data = read_serial();
  id = stoi(data);
  cout << id << endl;

}

arduino::~arduino(){
  boost::system::error_code ec;
  sp.close(ec);
}
/*função que recolhe os dados do control loop*/
void arduino::retrive_control_loop_data(){
  /*lê duty*/
  {
    string data = read_serial();
    duty = stof(data);
    /*envia notificação*/
    string str_start = "R";
    write(sp,	boost::asio::buffer(str_start));
  }
  last_duty.insert_value(duty);
  /*lê lux*/
  lux[2] = lux[1];
  lux[1] = lux[0];
  {
    string data = read_serial();
    lux[0] = stof(data);
    /*envia notificação*/
    string str_start = "R";
    write(sp,	boost::asio::buffer(str_start));
  }
  last_lum.insert_value(lux[0]);

  cout << lux[0] << endl;
  cout << duty << endl;

}
