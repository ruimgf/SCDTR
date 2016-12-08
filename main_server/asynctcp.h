#ifndef ASYNC_TCP_SERVER_H
#define	ASYNC_TCP_SERVER_H
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "arduino.h"


using namespace boost::asio;
using boost::asio::ip::tcp;
using namespace std;


class tcp_session
{
public:
  tcp_session(boost::asio::io_service& io_service,arduino* a_);
  tcp::socket& socket();
  void start();
  void handle_read(const boost::system::error_code& error,size_t bytes_transferred);
  void handle_write(const boost::system::error_code& error);
  string process_get(char str[]);
private:
  tcp::socket socket_;
  enum { max_length = 1024 };
  char question_[max_length];
  std::string response_;
  arduino* a;
};

class tcp_server
{
public:
  tcp_server(boost::asio::io_service& io_service, short port,arduino* a_);
  void handle_accept(tcp_session* new_session,const boost::system::error_code& error);
private:
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
  arduino* a;
};


#endif //ASYNC_TCP_SERVER.H
