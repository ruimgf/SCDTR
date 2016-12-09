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
  tcp_session(boost::asio::io_service& io_service,std::vector<shared_ptr <arduino>> a_);
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
  std::vector<shared_ptr <arduino>> a;
};

class tcp_server
{
public:
  tcp_server(boost::asio::io_service& io_service, short port);
  void handle_accept(tcp_session* new_session,const boost::system::error_code& error);
  void attacharduino(shared_ptr <arduino> ard);
  void start_accept();
private:
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
  std::vector<shared_ptr <arduino>> a;
};


#endif //ASYNC_TCP_SERVER.H
