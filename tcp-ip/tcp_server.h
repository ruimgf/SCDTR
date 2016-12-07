

#ifndef TCP_SERVER_H
#define	TCP_SERVER_H
#include <iostream>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <memory>

#define tcp_ip "127.0.0.1"

using namespace boost::asio;
using	ip::tcp;

std::string process_data(std::string str );

class tcp_server {
  private:
    boost::asio::io_service io;
    boost::asio::ip::tcp::acceptor acceptor{io};
    boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::tcp::v4(), 17000};
  public:
    tcp::socket * attep_conn();
    tcp_server();
};


#endif //TCP_SERVER.H
