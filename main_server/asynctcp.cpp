#include "asynctcp.h"
#include "globals.h"
/**
 * [tcp_session::process_get method to process a get request from a client]
 * @param  str [request from client]
 * @return     [mensage to send to client]
 */
std::string tcp_session::process_get(char str[] ){
  string response;
  string number{str[4]};

  std::stringstream number_stream{number};
  int ilum_nr ;
  number_stream >> ilum_nr;
  ilum_nr --;
  if(ilum_nr + 1 > ard.size() ){
    return "error";
  }
  response += str[2]; // request
  response += " ";
  response += str[4]; // ilum_nr
  response += " ";
  switch (str[2]) {
    case 'l': // lumens
    response += std::to_string(ard.at(ilum_nr)->get_current_lux());
    break;

    case 'd': // duty
    response += std::to_string(ard.at(ilum_nr)->get_current_duty());
    break;

    case 'o': // ocuppancy
    response += std::to_string(ard.at(ilum_nr)->get_occupancy());
    break;

    case 'L': // lower bound
    response += std::to_string(ard.at(ilum_nr)->get_low_lux());
    break;

    case 'O': // external ILLUm
    response += std::to_string(ard.at(ilum_nr)->get_external()); ;
    break;

    case 'r': // reference
    response += std::to_string(ard.at(ilum_nr)->get_reference());
    break;

    case 'p': // power
    if(str[4]=='T'){
      float total=0;
      for (size_t i = 0; i < ard.size(); i++) {
        total += ard.at(i)->get_power();
      }
      response += std::to_string(total);
      break;
    }

    response += std::to_string(ard.at(ilum_nr)->get_power());
    break;

    case 'e':// error confirm essay
    if(str[4]=='T'){
      float total=0;
      for (size_t i = 0; i < ard.size(); i++) {
        total += ard.at(i)->get_energy();
      }
      response += std::to_string(total);
      break;
    }
    response += std::to_string(ard.at(ilum_nr)->get_energy());
    break;

    case 'v': // variance
    if(str[4]=='T'){
      float total=0;
      for (size_t i = 0; i < ard.size(); i++) {
        total += ard.at(i)->get_variance();
      }
      response += std::to_string(total);
      break;
    }
    response += std::to_string(ard.at(ilum_nr)->get_variance());
    break;
    case 'c': // error confort_error
    if(str[4]=='T'){
      float total=0;
      for (size_t i = 0; i < ard.size(); i++) {
        total += ard.at(i)->get_variance();
      }
      response += std::to_string(total);
      break;
    }
    response += std::to_string(ard.at(ilum_nr)->get_variance());
    break;
    default:
    response = "error";
    break;
  }
  return response;
}
/**
 * [tcp_session::process_b method to process a b request from a client]
 * @param  str [request from client]
 * @return     [mensage to send to client]
 */
std::string tcp_session::process_b(char str[] ){
  response_ = "b ";
  response_ += str[2];
  response_ += " ";
  response_ += str[4];
  string number{str[4]};
  std::stringstream number_stream{number};

  int ilum_nr;
  number_stream >> ilum_nr;
  ilum_nr--;
  std::vector<float> values;
  if(ilum_nr+1 > ard.size()){
    response_ = "ERROR";
    return response_;
  }
  if(str[2]=='l'){
     values =  ard.at(ilum_nr)->get_oneminute_lux();
  }else if(str[2]=='d'){
     values =  ard.at(ilum_nr)->get_oneminute_duty();
  }else{
    response_ = "ERROR";
    return response_;
  }
  response_ += " ";
  response_ += std::to_string(values.at(0));
  for (size_t i = 1; i < values.size(); i++) {
    response_ += ",";
    response_ += std::to_string(values.at(i));
  }
  return response_;


}
/**
 * [tcp_session::process_set method to process a set request from a client]
 * @param  str [request from client]
 * @return     [mensage to send to client]
 */
std::string tcp_session::process_set(char str[] ){

  string number{question_[2]};
  std::stringstream number_stream{number};
  int ilum_nr;
  number_stream >> ilum_nr;
  ilum_nr --;
  if(ilum_nr + 1 > ard.size()){
    response_ = "error";
  }else{
    string oc{question_[4]};
    std::stringstream oct{oc};
    bool oc_set ;
    oct >> oc_set;
    ard[ilum_nr]->change_ocp(oc_set);
    response_ = "ack";
  }
  return response_;

}

/**
 * [tcp_session::process_get method to process a c request from a client]
 * @param  str [request from client]
 * @return     [mensage to send to client]
 */
std::string tcp_session::process_c(char str[]){

  string number{question_[4]};
  std::stringstream number_stream{number};
  int ilum_nr;
  number_stream >> ilum_nr;
  ilum_nr --;
  if(question_[2]=='l'){
    ard.at(ilum_nr)-> attachclistream_lux(this);
    socket_.async_read_some(boost::asio::buffer(question_, max_length),
        boost::bind(&tcp_session::handle_read, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }else if(question_[2]=='d'){
    ard.at(ilum_nr)-> attachclistream_duty(this);
    socket_.async_read_some(boost::asio::buffer(question_, max_length),
        boost::bind(&tcp_session::handle_read, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));

  }else{
    return "error";
  }

  return "sucess";
}
/**
 * [tcp_session::process_d method to process a d request from a client]
 * @param  str [request from client]
 * @return     [mensage to send to client]
 */
std::string tcp_session::process_d(char str[]){
  string number{question_[4]};
  std::stringstream number_stream{number};
  int ilum_nr;
  number_stream >> ilum_nr;
  ilum_nr -- ;
  if(question_[2]=='l'){
    ard.at(ilum_nr)->detachclistream_lux(this);
  }else if(question_[2]=='d'){
    ard.at(ilum_nr)-> detachclistream_duty(this);
  }else{
    return "ERROR";
  }
  return "ack";
}

/**
 * [tcp_session::process_reset method to process a reset request from a client]
 * @param  str [request from client]
 * @return     [mensage to send to client]
 */
std::string tcp_session::process_reset(char str[]){
  for (size_t i = 0; i < ard.size(); i++) {
    ard.at(i)->reset();
  }
  return "ack";

}
/**
 * [tcp_session::tcp_session tcp_session constructor]
 * @param io_service [io service to run this section]
 */
tcp_session::tcp_session(boost::asio::io_service& io_service)
  : socket_(io_service)
  {

  }
/**
 * [tcp_session::socket method to get the socket associated with this session]
 * @return [address of socket]
 */
tcp::socket& tcp_session::socket()
{
  return socket_;
}
/**
 * [tcp_session::start start session]
 */
void tcp_session::start()
{
  socket_.async_read_some(boost::asio::buffer(question_, max_length),
      boost::bind(&tcp_session::handle_read, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}
/**
 * [tcp_session::stopallstreams stop all streams]
 */
void tcp_session::stopallstreams(){
  for (size_t i = 0; i < ard.size(); i++) {
    ard.at(i)->detachclistream_duty(this);
    ard.at(i)->detachclistream_lux(this);
  }

}
/**
 * [tcp_session::handle_read method to handle a read from a tcp_session]
 * @param error             [error code]
 * @param bytes_transferred [nr of bytes_transferred]
 */
void tcp_session::handle_read(const boost::system::error_code& error,size_t bytes_transferred){
  if (!error)
  {
    response_="";
    // isto não é um switch porque não dava para criar variaveis la no meio
    //
    if(question_[0]=='g'){
      response_ = process_get(question_);
    }else if(question_[0]=='b'){
      response_ = process_b(question_);
    }else if(question_[0]=='s'){
      // este write talvez possa ser transferido para o call back do write do set
      response_ = process_set(question_);
    }else if(question_[0]=='c'){
      response_ = process_c(question_);
      if(response_!="error"){
        return; // don't write
      }
    }else if(question_[0]=='d'){
      response_ = process_d(question_);
    }else if(question_[0]=='r'){
      response_ = process_reset(question_);
    }
    response_ += '\n';
    boost::asio::async_write(socket_,
        boost::asio::buffer(response_, response_.length()),
        boost::bind(&tcp_session::handle_write, this,
          boost::asio::placeholders::error));
  }
  else
  {
    stopallstreams();
    delete this;
  }
}
/**
 * [tcp_session::handle_write method to handle a write to a client]
 * @param error [error code]
 */
void tcp_session::handle_write(const boost::system::error_code& error){
  if (!error)
  {
    socket_.async_read_some(boost::asio::buffer(question_, max_length),
        boost::bind(&tcp_session::handle_read, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }
  else
  {
    stopallstreams();
    delete this;
  }
}
/**
 * [tcp_session::stream_duty method to stream duty]
 * @param duty       [value of duty]
 * @param time_stamp [time stamp]
 * @param id         [id of iluminarie]
 */
void tcp_session::stream_duty(float duty,unsigned long int time_stamp, int id){
  mtx.lock();
  response_stream_duty = "c d ";
  response_stream_duty += std::to_string(id);
  response_stream_duty += " ";
  response_stream_duty += std::to_string(duty);
  response_stream_duty += " ";
  response_stream_duty += std::to_string(time_stamp);
  response_stream_duty += "\n";
  boost::asio::async_write(socket_,
      boost::asio::buffer(response_stream_duty, response_stream_duty.length()),
      boost::bind(&tcp_session::handle_write, this,
        boost::asio::placeholders::error));
  mtx.unlock();
}

/**
 * [tcp_session::stream_lux method to stream lux]
 * @param lux        [value of lux ]
 * @param time_stamp [time stamp]
 * @param id         [id of iluminary]
 */
void tcp_session::stream_lux(float lux,unsigned long int time_stamp, int id){
  mtx.lock();
  response_stream_lux = "c l ";
  response_stream_lux += std::to_string(id);
  response_stream_lux += " ";
  response_stream_lux += std::to_string(lux);
  response_stream_lux += " ";
  response_stream_lux += std::to_string(time_stamp);
  response_stream_lux += "\n";
  boost::asio::async_write(socket_,
      boost::asio::buffer(response_stream_lux, response_stream_lux.length()),
      boost::bind(&tcp_session::handle_write_stream, this,
        boost::asio::placeholders::error));
  mtx.unlock();
}

/**
 * [tcp_session::handle_write_stream method to handle a write of strem values]
 * @param error [error code]
 */
void tcp_session::handle_write_stream(const boost::system::error_code& error){
  if (!error)
  {

  }
  else
  {
    stopallstreams();
    delete this;
  }
}

/**
 * [tcp_server::tcp_server constructror of tcp server]
 * @param io_service [io service associated to server]
 * @param port       [TCP port of server]
 */
tcp_server::tcp_server(boost::asio::io_service& io_service, short port)
  : io_service_(io_service),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
{
  tcp_session * new_session = new tcp_session(io_service_);
  acceptor_.async_accept(new_session->socket(),
      boost::bind(&tcp_server::handle_accept, this, new_session,
        boost::asio::placeholders::error));
}
/**
 * [tcp_server::handle_accept method to handle a assync accept]
 * @param new_session [ & of struct to handle the connection]
 * @param error       [error code]
 */
void tcp_server::handle_accept(tcp_session* new_session,
    const boost::system::error_code& error){
  if (!error)
  {
    new_session->start();
    new_session = new tcp_session(io_service_);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_session,
          boost::asio::placeholders::error));
  }
  else
  {
    delete new_session;
  }
}
