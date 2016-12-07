//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
using namespace boost::asio;
using boost::asio::ip::tcp;
using namespace std;


int lum[3] = {4,5,6};
int duty[3] = {240,245,250};
int ocuppancy[3] = {0,0,1};
int lower[3] = {10,20,30};
int external[3] = {15,30,35};
int reference[3] = {10,1,12};
int power[3] = {100,200,300};
int error_ac[3] = {10120,20330,34400};
int variance[3] = {10,13,123};

std::string process_data(char str[] ){
  string response;
  if(str[0]=='g'){
    string number =  {str,4};
    int ilum_nr = atoi( number.c_str() ) - 1;
    switch (str[2]) {
      case 'l': // lumens

      response += "l ";
      response += str[4];
      response += " ";
      response += std::to_string(lum[ilum_nr]);
      break;
      case 'd': // duty
      response += "d ";
      response += str[4];
      response += " ";
      response += std::to_string(duty[ilum_nr]);

      break;
      case 'o': // ocuppancy
      response += "o ";
      response += str[4];
      response += " ";
      response += std::to_string(ocuppancy[ilum_nr]);

      break;
      case 'L': // lower bound
      response += "L ";
      response += str[4];
      response += " ";
      response += std::to_string(lower[ilum_nr]);

      break;
      case 'O': // external ILLUm
      response += "O ";
      response += str[4];
      response += " ";
      response += std::to_string(external[ilum_nr]);

      break;
      case 'r': // reference
      response += "r ";
      response += str[4];
      response += " ";
      response += std::to_string(reference[ilum_nr]);
      break;

      case 'p': // power
      if(str[4]=='T'){
        // insert code to total
        break;
      }
      response += "p ";
      response += str[4];
      response += " ";
      response += std::to_string(power[ilum_nr]);
      break;
      case 'e':// error confirm essay
      if(str[4]=='T'){
        // insert code to total
        break;
      }
      response += "e ";
      response += str[4];
      response += " ";
      response += std::to_string(error_ac[ilum_nr]);
      break;
      case 'v': // variance
      response += "v ";
      response += str[4];
      response += " ";
      response += std::to_string(variance[ilum_nr]);
      break;

      default:
      break;
    }
  }else if(str[0]=='s'){
    string aux =  {str,2};
    int ilum_nr = atoi( aux.c_str() ) - 1;
    string aux1 =  {str,4};
    int state = atoi( aux1.c_str() );
    // change occupancy
    std::cout << "change illum " << ilum_nr << "to state" << state << '\n';
    response = "sucess";
  } else if(str[0]=='r'){
    // insert code to reset
    response = "sucess";
  }
  if(!response.empty()){

    return response;
  }else{
    response = "error";
    return response;
  }

}
class session
{
public:
  session(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  {
    socket_.async_read_some(boost::asio::buffer(question_, max_length),
        boost::bind(&session::handle_read, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

  void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    {
      response_ = process_data(question_);
      boost::asio::async_write(socket_,
          boost::asio::buffer(response_, response_.length()),
          boost::bind(&session::handle_write, this,
            boost::asio::placeholders::error));
    }
    else
    {
      delete this;
    }
  }

  void handle_write(const boost::system::error_code& error)
  {
    if (!error)
    {
      socket_.async_read_some(boost::asio::buffer(question_, max_length),
          boost::bind(&session::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      delete this;
    }
  }

private:
  tcp::socket socket_;
  enum { max_length = 1024 };
  char question_[max_length]= "ola o meu nome ";
  std::string response_;
};

class server
{
public:
  server(boost::asio::io_service& io_service, short port)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
  {
    session* new_session = new session(io_service_);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&server::handle_accept, this, new_session,
          boost::asio::placeholders::error));
  }

  void handle_accept(session* new_session,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_session->start();
      new_session = new session(io_service_);
      acceptor_.async_accept(new_session->socket(),
          boost::bind(&server::handle_accept, this, new_session,
            boost::asio::placeholders::error));
    }
    else
    {
      delete new_session;
    }
  }

private:
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: async_tcp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    using namespace std; // For atoi.
    server s(io_service, atoi(argv[1]));

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
