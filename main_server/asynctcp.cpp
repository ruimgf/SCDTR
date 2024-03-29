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


std::string tcp_session::process_get(char str[] ){
  string response;
  string number{str[4]};
  int ilum_nr = atoi( number.c_str() ) - 1;
  if(ilum_nr + 1 > a.size() ){
    return "error";
  }
  response += str[2]; // request
  response += " ";
  response += str[4]; // ilum_nr
  response += " ";
  switch (str[2]) {
    case 'l': // lumens
    //response += a->get_current_lux();
    response += std::to_string( (a[0])->get_current_lux());
    break;

    case 'd': // duty
    response += std::to_string((a[ilum_nr])->get_current_duty());
    break;

    case 'o': // ocuppancy
    response += std::to_string(a[ilum_nr]->get_occupancy());
    break;

    case 'L': // lower bound
    response += std::to_string(a[ilum_nr]->get_low_lux());
    break;

    case 'O': // external ILLUm
    response += "error";
    break;

    case 'r': // reference
    response += std::to_string(a[ilum_nr]->get_reference());
    break;

    case 'p': // power
    if(str[4]=='T'){
      // insert code to total
      // overwrite response
      break;
    }
    response += std::to_string(a[ilum_nr]->get_power());
    break;

    case 'e':// error confirm essay
    if(str[4]=='T'){
      // insert code to total
      break;
    }
    response += std::to_string(a[ilum_nr]->get_energy());
    break;

    case 'v': // variance
    response += std::to_string(a[ilum_nr]->get_variance());
    break;

    default:
    response = "error";
    break;
  }
  return response;
}

tcp_session::tcp_session(boost::asio::io_service& io_service, std::vector<shared_ptr <arduino>> a_)
  : socket_(io_service),
  a(a_)
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
      if(question_[2]=='l'){
        //response_ = process_lastminute(question_);
      }else if(question_[2]=='d'){
        //response_ = process_lastminute(question_);
      }else{
        response_ = "ERROR";
      }
      // boost::asio::async_write(socket_,
      //     boost::asio::buffer(response_, response_.length()),
      //     boost::bind(&tcp_session::handle_write, this,
      //       boost::asio::placeholders::error));

    }else if(question_[0]=='s'){
      string number{question_[2]};
      int ilum_nr = stoi( number.c_str() ) - 1;
      string oc{question_[4]};
      int oc_set = stoi( oc.c_str() );
      a[ilum_nr]->change_ocp(oc_set);
      response_ = "ack";
      // este write talvez possa ser transferido para o call back do write do set
      boost::asio::async_write(socket_,
          boost::asio::buffer(response_, response_.length()),
          boost::bind(&tcp_session::handle_write, this,
            boost::asio::placeholders::error));


    }else if(question_[0]=='c'){
      string number{question_[2]};
      int ilum_nr = stoi( number.c_str() ) - 1;
      if(question_[2]=='l'){
        //response_ = process_lastminute(question_);

      }else if(question_[2]=='d'){
        //response_ = process_lastminute(question_);
        //a[ilum_nr]->attachstreamduty(socket_);
        socket_.async_read_some(boost::asio::buffer(question_, max_length),
            boost::bind(&tcp_session::handle_read, this,
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred));
      }else{
        response_ = "ERROR";
      }
    }else if(question_[0]=='r'){


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



// tcp_server
tcp_server::tcp_server(boost::asio::io_service& io_service, short port)
  : io_service_(io_service),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
{

}
void tcp_server::attacharduino(shared_ptr <arduino> ard){
    a.insert(a.end(),ard);
}
void tcp_server::start_accept(){
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
