#include "arduino.h"

arduino::arduino(io_service& io_,std::string port_name)
: io(io_),
  sp(io),
  tim_setup(io),
  last_lux(600),
  last_duty(600),
  last_ts(600)
{
  //initialization of variables
  ts = 0;duty = 0; lux[0] = 0; lux[1] = 0; lux[2] = 0;
  C_e = 0; E = 0; V_f = 0; occupancy=0; ref_lux=DEFAULT_UNCUP_LUX_REF;


  try{
    sp.open(port_name);
    sp.set_option(serial_port_base::baud_rate(115200));
  }catch(boost::system::system_error& error){
    boost::system::error_code ec =	error.code();
    std::cerr<<"ERROR:initialization of serial port "<<ec.message()<<std::endl;
  }
  /*timer to wait arduino to reset*/
  tim_setup.expires_from_now(boost::posix_time::seconds(5));
  tim_setup.async_wait( boost::bind(&arduino::timer_handler, this
    ,boost::asio::placeholders::error) );

}

arduino::~arduino()
{
  sp.close();
}

void arduino::timer_handler(const error_code& ec)
{
  std::ostringstream os;
  os << "w" ;
  /*escreve W para o arduino para sair do setup*/
  async_write( sp, buffer(os.str()) ,
  boost::bind(&arduino::write_handler_setup, this,
  boost::asio::placeholders::error) );


}

void arduino::write_handler_setup(const error_code& ec)
{
  /*faz read assync da resposta do arduino*/
  tim_setup.expires_from_now(boost::posix_time::seconds(2));
  tim_setup.async_wait( boost::bind(&arduino::dummy_read, this
    ,boost::asio::placeholders::error) );
}

void arduino::dummy_read(const error_code& ec)
{
  async_read_until(sp, read_buf, '\n', boost::bind(&arduino::read_setup_handler,
    this, boost::asio::placeholders::error) );
}

void arduino::read_setup_handler(const error_code& ec)
{
  /*read the values present in buffer*/
  std::string id_data;
  /*put data into buf*/

  std::istream is(&read_buf);
  std::getline(is, id_data);
  std::cout << id_data << std::endl;

}
