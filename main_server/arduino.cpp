#include "arduino.h"
#include "globals.h"

#define SAMPLE_TIME 10 //ms

arduino::arduino(io_service& io_,std::string port_name)
: io(io_),
  sp(io),
  tim_setup(io),
  last_lux(6000),
  last_duty(6000),
  last_ts(6000)
{
  //initialization of variables
  external=0;
  ts = 0;duty = 0; lux[0] = 0; lux[1] = 0; lux[2] = 0;
  P_i=0;
  C_e = 0; E = 0; V_f = 0; occupancy=0; ref_lux=DEFAULT_UNCUP_LUX_REF;
  lower_lux = 20000;
  N=0;
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
/**
 * [arduino::begin_timer_handler handler for begin timer callback]
 * @param ec [error code]
 */
void arduino::begin_timer_handler(const boost::system::error_code& ec)
{
  async_read_until(sp, read_buf, "\n", boost::bind(&arduino::read_setup_handler,
    this, boost::asio::placeholders::error) );
}
/**
 * [arduino::timer_handler handler for timer callback]
 * @param ec [description]
 */
void arduino::timer_handler(const boost::system::error_code& ec)
{
  async_read_until(sp, read_buf, "\n", boost::bind(&arduino::read_handler,
    this, boost::asio::placeholders::error) );
}
/**
 * [arduino::save_value method to save one readed value from serial]
 * @param duty_mes   [duty mesure]
 * @param lux_mes    [lux mesure]
 * @param time_stamp [server time of mesure]
 * @param ex         [external ilumination mesure]
 */
void arduino::save_value(float duty_mes, float lux_mes, int time_stamp,float ex){
  N++;

  last_lux.insert_value(lux_mes);
  last_duty.insert_value(duty_mes);
  last_ts.insert_value(time_stamp);


  lux[2] = lux[1];
  lux[1] = lux[0];
  lux[0] = lux_mes;

  P_i = duty_mes * SAMPLE_TIME;
  // compute energy
  E += duty_mes * SAMPLE_TIME;

  external = ex;
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
      V_f = V_f*(N-1)*pow(SAMPLE_TIME,2);
      V_f += abs(lux[0]-2*lux[1]+lux[2]);
      V_f = V_f/((N*pow(SAMPLE_TIME,2)));
  }

  if(cli_stream_duty.size()>0){
    for(int i = 0; i<cli_stream_duty.size();i++ ){
      cli_stream_duty.at(i)->stream_duty(duty_mes,time_stamp,id);
    }
  }

  if(cli_stream_lux.size()>0){
    for(int i = 0; i<cli_stream_lux.size();i++ ){
      cli_stream_lux.at(i)->stream_lux(lux_mes,time_stamp,id);
    }
  }

  //std::cout << "lux: "<< last_lux.read_actual_value() << std::endl;
  //std::cout << "duty: "<< last_duty.read_actual_value() << std::endl;
  //std::cout << "ts : "<< last_ts.read_actual_value() << std::endl;


}
/**
 * [arduino::read_handler method to handler on read from serial]
 * @param ec [error code]
 */
void arduino::read_handler(const boost::system::error_code& ec)
{
  /*read the values present in buffer*/
  std::string data;
  /*put data into buf*/

  std::istream is(&read_buf);
  std::getline(is, data);
  if(data.length()>1){

    std::size_t begin = data.find("l");
    std::size_t begin_d = data.find("d");
    std::size_t begin_e = data.find("e");
    if (begin!=std::string::npos&&begin_d!=std::string::npos&&begin_e!=std::string::npos){
      std::string clean_lux{data,begin+1,begin_d-1};
      std::stringstream clean_lux_{clean_lux};
      std::string clean_duty{data,begin_d+1,begin_e-1};
      std::stringstream clean_duty_{clean_duty};
      std::string clean_external{data,begin_e+1};
      std::stringstream clean_external_{clean_external};
      std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
      auto duration = now.time_since_epoch() - begin_time.time_since_epoch();
      auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
      int duty_;
      float lux_;
      float external_;


      try{
        clean_duty_ >> duty_;
        clean_lux_ >> lux_;
        clean_external_ >> external_;
        save_value(duty_,lux_,millis,external_);
      }catch(int error){
        std::cout << "error in convertion" << '\n';
      }



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
/**
 * [arduino::read_setup_handler method to handler setup read]
 * @param ec [error code]
 */
void arduino::read_setup_handler(const boost::system::error_code& ec)
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

/**
 * [arduino::change_ocp method to change ocupantion on iluminary]
 * @param change_ocp [bool representing true - occupate , false - not ocupate]
 */
void arduino::change_ocp(bool change_ocp){
  boost::asio::streambuf buf;
  std::string mensage;
  if(change_ocp!=0){
    mensage = "O"; // ocupate
    ref_lux = DEFAULT_OCCUP_LUX_REF;
    occupancy = 1;
  }else{
    mensage = "N"; // not ocupate
    ref_lux = DEFAULT_UNCUP_LUX_REF;
    occupancy = 0;
  }
  async_write(sp,	boost::asio::buffer(mensage),boost::bind(&arduino::write_ocp_handler, this
    ,boost::asio::placeholders::error));

}

/**
 * [arduino::write_ocp_handler write handler]
 * @param ec [error code]
 */
void arduino::write_ocp_handler(const boost::system::error_code& ec){
  // do noting

}

/**
 * [arduino::get_current_lux method to get current lux on iluminary]
 * @return [lux mesure]
 */
float arduino::get_current_lux(){
      return last_lux.read_actual_value();
}

/**
 * [arduino::get_low_lux method to get the lowest lux mesure on this desk]
 * @return [lowest lux value]
 */
float arduino::get_low_lux(){
      return lower_lux;
}
/**
 * [arduino::get_external method to get external ilumination mesure]
 * @return [external ilumination mesure]
 */
float arduino::get_external(){
      return external;
}
/**
 * [arduino::get_current_duty method to get current duty mesure]
 * @return [description]
 */
float arduino::get_current_duty(){
      return last_duty.read_actual_value();
}
/**
 * [arduino::get_occupancy method to get ocuppancy]
 * @return [ocupation value]
 */
bool arduino::get_occupancy(){
      return occupancy;
}
/**
 * [arduino::get_reference method to get reference value]
 * @return [reference value]
 */
float arduino::get_reference(){
    return ref_lux;
}
/**
 * [arduino::get_power method to get instatain power consumation]
 * @return [power consumation]
 */
float arduino::get_power(){
    return P_i;
}
/**
 * [arduino::get_energy method to get energy]
 * @return [energy consumation]
 */
float arduino::get_energy(){
  return E;
}
/**
 * [arduino::get_error_confort method to get error confort]
 * @return [error confort value]
 */
float arduino::get_error_confort(){
  return C_e;
}
/**
 * [arduino::get_variance method to get variance value]
 * @return [variance value ]
 */
float arduino::get_variance(){
      return V_f;
}
/**
 * [arduino::get_oneminute_duty method to get duty last minute history]
 * @return [vector of last minute duty history]
 */
std::vector<float> arduino::get_oneminute_duty(){
      return last_duty.read_all_values();
}
/**
 * [arduino::get_oneminute_lux method to get lux last minute history]
 * @return [vector of last minute lux history]
 */
std::vector<float> arduino::get_oneminute_lux(){
      return last_lux.read_all_values();
}
/**
 * [arduino::attachclistream_duty method to start new client to recive duty stream]
 * @param cli [tcp_session to recive stream]
 */
void arduino::attachclistream_duty(tcp_session* cli){
    cli_stream_duty.push_back(cli);
}
/**
 * [arduino::attachclistream_lux method to start new client to recive lux stream]
 * @param cli [tcp_session to recive stream]
 */
void arduino::attachclistream_lux(tcp_session* cli){
    cli_stream_lux.push_back(cli);
}
/**
 * [arduino::attachclistream_lux method to stop client to recive lux stream]
 * @param cli [tcp_session to stop recive stream]
 */
void arduino::detachclistream_lux(tcp_session* cli){
    for (size_t i = 0; i < cli_stream_lux.size(); i++) {
      if(cli_stream_lux.at(i)==cli){
        cli_stream_lux.erase(cli_stream_lux.begin()+i);
        break;
      }
    }

}
/**
 * [arduino::attachclistream_duty method to stop client to recive duty stream]
 * @param cli [tcp_session to stop recive stream]
 */
void arduino::detachclistream_duty(tcp_session* cli){
  for (size_t i = 0; i < cli_stream_duty.size(); i++) {
    if(cli_stream_duty.at(i)==cli){
      cli_stream_duty.erase(cli_stream_duty.begin()+i);
      break;
    }
  }

}
/**
 * [arduino::reset method to reset iluminarie and recalibrate]
 */
void arduino::reset(){
  ts = 0;duty = 0; lux[0] = 0; lux[1] = 0; lux[2] = 0;
  P_i=0;
  C_e = 0; E = 0; V_f = 0; occupancy=0; ref_lux=DEFAULT_UNCUP_LUX_REF;
  lower_lux = 20000;
  string mensage{"R"};
  async_write(sp,	boost::asio::buffer(mensage),boost::bind(&arduino::write_ocp_handler, this
    ,boost::asio::placeholders::error));
}
