

#ifndef TCP_SERVER_H
#define	TCP_SERVER_H
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#define tcp_ip "127.0.0.1"

using namespace boost::asio;
using	ip::tcp;


// class tcp_server {
//   private:
//     //initialize services
//     boost::asio::io_service io;
//     //create a	listening socket
//     //	bind and start listening at port 10000
//     tcp::acceptor acceptor{io,tcp_ip,	10000};
//   public:
//     void attep_conn();
// };


#endif //TCP_SERVER.H
