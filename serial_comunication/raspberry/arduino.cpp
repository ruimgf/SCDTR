#include "arduino.h"

#define SAMPLE_TIME 10 //ms

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
  P_i=0;
  C_e = 0; E = 0; V_f = 0; occupancy=0; ref_lux=DEFAULT_UNCUP_LUX_REF;
  lower_lux = 20000;
  begin_time = std::chrono::system_clock::now();
  try{
    sp.open(port_name);
    sp.set_option(serial_port_base::baud_rate(115200));
  }catch(boost::system::system_error& error){
    boost::system::error_code ec =	error.code();
    std::cerr<<"ERROR:initialization of serial port "<<ec.message()<<std::endl;
  }
  /*timer to wait arduino to reset*/
  tim_setup.expires_from_now(boost::posix_time::seconds(2));
  tim_setup.async_wait( boost::bind(&arduino::begin_timer_handler, this
    ,boost::asio::placeholders::error) );

}

arduino::~arduino()
{
  sp.close();
}

void arduino::begin_timer_handler(const error_code& ec)
{
  async_read_until(sp, read_buf, "\n", boost::bind(&arduino::read_setup_handler,
    this, boost::asio::placeholders::error) );
}

void arduino::timer_handler(const error_code& ec)
{
  //std::ostringstream os;
  //os << "w" ;
  //escreve W para o arduino para sair do setup
  async_read_until(sp, read_buf, "\n", boost::bind(&arduino::read_handler,
    this, boost::asio::placeholders::error) );
}

void arduino::save_value(float duty_mes, float lux_mes, int time_stamp){
  N++;

  last_lux.insert_value(lux_mes);
  last_duty.insert_value(duty_mes);

  last_ts.insert_value(time_stamp);


  lux[2] = lux[1];
  lux[1] = lux[0];
  lux[0] = lux_mes;

  P_i = duty_mes * SAMPLE_TIME/1000;
  // compute energy
  E += duty_mes * SAMPLE_TIME/1000;

  // compute lower bound
  if (lower_lux > lux_mes  ){
    lower_lux = lux_mes;
  }

  // compute confort_error
  if(ref_lux - lux_mes > 0){
      C_e = (C_e*(N-1) + ref_lux - lux_mes) / N;

  }else{
    C_e = (C_e * (N-1))/N;

  }
  if(N>=3){
      V_f = V_f*(N-1)*pow(SAMPLE_TIME/1000,2);
      V_f += abs(lux[0]-lux[1]-lux[2]);
      V_f = V_f/(N*SAMPLE_TIME/1000);

  }


  std::cout << "lux: "<< last_lux.read_actual_value() << std::endl;
  std::cout << "duty: "<< last_duty.read_actual_value() << std::endl;
  std::cout << "ts : "<< last_ts.read_actual_value() << std::endl;


}

void arduino::read_handler(const error_code& ec)
{
  /*read the values present in buffer*/
  std::string data;
  /*put data into buf*/

  std::istream is(&read_buf);
  std::getline(is, data);
  if(data.length()>1){

    std::size_t begin = data.find("l");
    std::size_t end = data.find("|");
    if (begin!=std::string::npos&&end!=std::string::npos){
      std::string clean_lux{data,begin+1,end-1};
      std::string clean_duty{data,end+1};
      std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
      auto duration = now.time_since_epoch() - begin_time.time_since_epoch();
      auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();


      save_value(std::stoi(clean_duty)/255.0,std::stof(clean_lux),millis);


      tim_setup.expires_from_now(boost::posix_time::millisec(10));
      tim_setup.async_wait( boost::bind(&arduino::timer_handler, this
        ,boost::asio::placeholders::error) );
      return;
    }

  }


  tim_setup.expires_from_now(boost::posix_time::millisec(10));
  tim_setup.async_wait( boost::bind(&arduino::timer_handler, this
    ,boost::asio::placeholders::error) );


}

void arduino::read_setup_handler(const error_code& ec)
{
  /*read the values present in buffer*/
  std::string id_data;
  /*put data into buf*/

  std::istream is(&read_buf);
  std::getline(is, id_data);
  if(id_data.length()>1){

    std::size_t found = id_data.find("arduino");
    if (found!=std::string::npos){
      std::string clean{id_data,found+7};
      id = std::stoi(clean);
      std::cout << id << std::endl;
      tim_setup.expires_from_now(boost::posix_time::millisec(10));
      tim_setup.async_wait( boost::bind(&arduino::timer_handler, this
        ,boost::asio::placeholders::error) );
      return;
    }
  }
  tim_setup.expires_from_now(boost::posix_time::millisec(10));
  tim_setup.async_wait( boost::bind(&arduino::begin_timer_handler, this
    ,boost::asio::placeholders::error) );

}


void arduino::change_ocp(bool change_ocp){
  boost::asio::streambuf buf;
  std::string mensage{"co"};
  if(change_ocp!=0){
    mensage += "O"; // ocupate
    ref_lux = DEFAULT_OCCUP_LUX_REF;
    occupancy = 1;
  }else{
    mensage += "N"; // not ocupate
    ref_lux = DEFAULT_UNCUP_LUX_REF;
    occupancy = 0;
  }
  async_write(sp,	boost::asio::buffer(mensage),boost::bind(&arduino::write_ocp_handler, this
    ,boost::asio::placeholders::error));

}


void arduino::write_ocp_handler(const error_code& ec){
  // do noting

}


float arduino::get_current_lux(){
      return last_lux.read_actual_value();
}

float arduino::get_current_duty(){
      return last_duty.read_actual_value();
}
bool arduino::get_occupancy(){
      return occupancy;
}

float arduino::get_reference(){
    return ref_lux;
}

float arduino::get_power(){
    return P_i;
}

float arduino::get_energy(){
  return E;
}

float arduino::get_error_confort(){
  return C_e;
}
