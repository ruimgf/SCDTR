#ifndef ASYNC_TCP_SERVER_H
#define	ASYNC_TCP_SERVER_H
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <mutex>

using namespace boost::asio;
using boost::asio::ip::tcp;
using namespace std;


class tcp_session
{
public:
  tcp_session(boost::asio::io_service& io_service);
  tcp::socket& socket();
  void start(); // start session
  void handle_read(const boost::system::error_code& error,size_t bytes_transferred); // normal read
  void handle_write(const boost::system::error_code& error);// normal write
  void stream_duty(float duty,unsigned long int time_stamp,int id); // stream_duty
  void stream_lux(float lux,unsigned long int time_stamp,int id); // stream_lux
  void handle_write_stream(const boost::system::error_code& error);
private:
  string process_get(char str[]);
  string process_b(char str[] );
  string process_set(char str[] );
  string process_c(char str[] );
  string process_d(char str[] );
  string process_reset(char str[] );
  std::mutex mtx;
  tcp::socket socket_;
  enum { max_length = 1024 };
  char question_[max_length];
  std::string response_;
  std::string response_stream_lux;
  std::string response_stream_duty;
};

class tcp_server
{
public:
  tcp_server(boost::asio::io_service& io_service, short port);
  void handle_accept(tcp_session* new_session,const boost::system::error_code& error);
private:
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
};


#endif //ASYNC_TCP_SERVER.H
