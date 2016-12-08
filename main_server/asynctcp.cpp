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

int lum[3] = {4,5,6};
int duty[3] = {240,245,250};
int ocuppancy[3] = {0,0,1};
int lower[3] = {10,20,30};
int external[3] = {15,30,35};
int reference[3] = {10,1,12};
int power[3] = {100,200,300};
int error_ac[3] = {10120,20330,34400};
int variance[3] = {10,13,123};

std::string tcp_session::process_get(char str[] ){
  string response;
  string number{str[4]};
  int ilum_nr = atoi( number.c_str() ) - 1;
  response += str[2]; // request
  response += " ";
  response += str[4]; // ilum_nr
  response += " ";
  switch (str[2]) {
    case 'l': // lumens
    //response += a->get_current_lux();
    response += std::to_string( a->get_current_lux());
    break;

    case 'd': // duty
    response += std::to_string(a->get_current_duty());
    break;

    case 'o': // ocuppancy
    response += std::to_string(a->get_occupancy());
    break;

    case 'L': // lower bound
    response += std::to_string(a->get_low_lux());
    break;

    case 'O': // external ILLUm
    response += "error";
    break;

    case 'r': // reference
    response += std::to_string(a->get_reference());
    break;

    case 'p': // power
    if(str[4]=='T'){
      // insert code to total
      // overwrite response
      break;
    }
    response += std::to_string(power[ilum_nr]);
    break;

    case 'e':// error confirm essay
    if(str[4]=='T'){
      // insert code to total
      break;
    }
    response += std::to_string(error_ac[ilum_nr]);
    break;

    case 'v': // variance
    response += std::to_string(variance[ilum_nr]);
    break;

    default:
    response = "error";
    break;
  }
  return response;
}

tcp_session::tcp_session(boost::asio::io_service& io_service, arduino* a_)
  : socket_(io_service),
    a(a_)
  {}

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
    switch (question_[0]) {
      case 'g':
      response_ = process_get(question_);
      boost::asio::async_write(socket_,
          boost::asio::buffer(response_, response_.length()),
          boost::bind(&tcp_session::handle_write, this,
            boost::asio::placeholders::error));
      break;
      case 'b':
      response_ = "b ";
      response_ += question_[2];
      if(question_[2]=='l'){
        //response_ = process_lastminute(question_);
      }else if(question_[2]=='d'){
        //response_ = process_lastminute(question_);
      }else{
        response_ = "ERROR";
      }
      boost::asio::async_write(socket_,
          boost::asio::buffer(response_, response_.length()),
          boost::bind(&tcp_session::handle_write, this,
            boost::asio::placeholders::error));
      break;
      case 'c':
        // start stream
      break;
      case 's':
          // armar set do arduino
          // set do arduino deve armar a escrita para o client
          // o read pode também ser armado aqui
      break;
      case 'r':
          // armar reset
          // set do arduino deve armar a escrita para o client
      break;

      default:
      response_ = "error";
      boost::asio::async_write(socket_,
          boost::asio::buffer(response_, response_.length()),
          boost::bind(&tcp_session::handle_write, this,
            boost::asio::placeholders::error));
      break;

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



// tcp_server
tcp_server::tcp_server(boost::asio::io_service& io_service, short port, arduino* a_)
  : io_service_(io_service),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
    a(a_)
{
  tcp_session* new_session = new tcp_session(io_service_, a);
  acceptor_.async_accept(new_session->socket(),
      boost::bind(&tcp_server::handle_accept, this, new_session,
        boost::asio::placeholders::error));
}

void tcp_server::handle_accept(tcp_session* new_session,
    const boost::system::error_code& error){
  if (!error)
  {
    new_session->start();
    new_session = new tcp_session(io_service_,a);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_session,
          boost::asio::placeholders::error));
  }
  else
  {
    delete new_session;
  }
}
