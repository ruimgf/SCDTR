//
// async_tcp_echo_tcp_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "asynctcp.h"
#include "globals.h"

std::string tcp_session::process_get(char str[] ){
  string response;
  string number{str[4]};
  int ilum_nr = atoi( number.c_str() ) - 1;
  if(ilum_nr + 1 > ard.size() ){
    return "error";
  }
  response += str[2]; // request
  response += " ";
  response += str[4]; // ilum_nr
  response += " ";
  switch (str[2]) {
    case 'l': // lumens
    //response += a->get_current_lux();
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
    response += "error";
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

tcp_session::tcp_session(boost::asio::io_service& io_service)
  : socket_(io_service)
  {

  }

tcp::socket& tcp_session::socket()
{
  return socket_;
}

void tcp_session::start()
{
  socket_.async_read_some(boost::asio::buffer(question_, max_length),
      boost::bind(&tcp_session::handle_read, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void tcp_session::handle_read(const boost::system::error_code& error,size_t bytes_transferred){
  if (!error)
  {
    response_="";
    int ilum_nr;
    // isto não é um switch porque não dava para criar variaveis la no meio
    //
    if(question_[0]=='g'){
      response_ = process_get(question_);
      boost::asio::async_write(socket_,
          boost::asio::buffer(response_, response_.length()),
          boost::bind(&tcp_session::handle_write, this,
            boost::asio::placeholders::error));
    }else if(question_[0]=='b'){
      response_ = "b ";
      response_ += question_[2];
      response_ += " ";
      response_ += question_[4];
      string number{question_[4]};
      int ilum_nr = stoi( number.c_str() ) - 1;
      std::vector<float> values;
      if(ilum_nr+1 > ard.size()){
        response_ = "ERROR";
        boost::asio::async_write(socket_,
            boost::asio::buffer(response_, response_.length()),
            boost::bind(&tcp_session::handle_write, this,
              boost::asio::placeholders::error));
      }
      if(question_[2]=='l'){
         values =  ard.at(ilum_nr)->get_oneminute_lux();
      }else if(question_[2]=='d'){
        values =  ard.at(ilum_nr)->get_oneminute_duty();
      }else{
        response_ = "ERROR";
        boost::asio::async_write(socket_,
            boost::asio::buffer(response_, response_.length()),
            boost::bind(&tcp_session::handle_write, this,
              boost::asio::placeholders::error));
        return;
      }
      response_ += " ";
      response_ += std::to_string(values.at(0));
      for (size_t i = 1; i < values.size(); i++) {
        response_ += ",";
        response_ += std::to_string(values.at(i));
      }
      boost::asio::async_write(socket_,
          boost::asio::buffer(response_, response_.length()),
          boost::bind(&tcp_session::handle_write, this,
            boost::asio::placeholders::error));

    }else if(question_[0]=='s'){
      string number{question_[2]};
      int ilum_nr = stoi( number.c_str() ) - 1;
      if(ilum_nr + 1 > ard.size()){
        response_ = "error";

      }else{
        string oc{question_[4]};
        int oc_set = stoi( oc.c_str() );
        ard[ilum_nr]->change_ocp(oc_set);
        response_ = "ack";
      }

      // este write talvez possa ser transferido para o call back do write do set
      boost::asio::async_write(socket_,
          boost::asio::buffer(response_, response_.length()),
          boost::bind(&tcp_session::handle_write, this,
            boost::asio::placeholders::error));


    }else if(question_[0]=='c'){
      string number{question_[4]};
      int ilum_nr = stoi( number.c_str() ) - 1;
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
        response_ = "ERROR";
        boost::asio::async_write(socket_,
            boost::asio::buffer(response_, response_.length()),
            boost::bind(&tcp_session::handle_write, this,
              boost::asio::placeholders::error));
      }
    }else if(question_[0]=='d'){
      response_ = "ack";
      if(question_[2]=='l'){
        ard.at(ilum_nr)->detachclistream_lux(this);

      }else if(question_[2]=='d'){
        response_ = "ack";
        ard.at(ilum_nr)-> detachclistream_duty(this);

      }else{
        response_ = "ERROR";
      }
      boost::asio::async_write(socket_,
          boost::asio::buffer(response_, response_.length()),
          boost::bind(&tcp_session::handle_write, this,
            boost::asio::placeholders::error));
    }else if(question_[0]=='r'){
      for (size_t i = 0; i < ard.size(); i++) {
        ard.at(i)->reset();
      }
      response_ = "ack";
      boost::asio::async_write(socket_,
          boost::asio::buffer(response_, response_.length()),
          boost::bind(&tcp_session::handle_write, this,
            boost::asio::placeholders::error));
    }else{
      response_ = "error";
      boost::asio::async_write(socket_,
          boost::asio::buffer(response_, response_.length()),
          boost::bind(&tcp_session::handle_write, this,
            boost::asio::placeholders::error));
    }

  }
  else
  {
    delete this;
  }
}

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
    delete this;
  }
}

void tcp_session::stream_duty(float duty,unsigned long int time_stamp, int id){
  response_ = "c d ";
  response_ += std::to_string(id);
  response_ += std::to_string(duty);
  response_ += " ";
  response_ += std::to_string(time_stamp);
  boost::asio::async_write(socket_,
      boost::asio::buffer(response_, response_.length()),
      boost::bind(&tcp_session::handle_write, this,
        boost::asio::placeholders::error));

}


void tcp_session::stream_lux(float lux,unsigned long int time_stamp, int id){
  response_ = "c l ";
  response_ += std::to_string(id);
  response_ += " ";
  response_ += std::to_string(lux);
  response_ += " ";
  response_ += std::to_string(time_stamp);
  boost::asio::async_write(socket_,
      boost::asio::buffer(response_, response_.length()),
      boost::bind(&tcp_session::handle_write_stream, this,
        boost::asio::placeholders::error));
}

void tcp_session::handle_write_stream(const boost::system::error_code& error){
  if (!error)
  {

  }
  else
  {
    delete this;
  }
}

// tcp_server
tcp_server::tcp_server(boost::asio::io_service& io_service, short port)
  : io_service_(io_service),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
{
  tcp_session * new_session = new tcp_session(io_service_);
  acceptor_.async_accept(new_session->socket(),
      boost::bind(&tcp_server::handle_accept, this, new_session,
        boost::asio::placeholders::error));
}

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
