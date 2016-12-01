#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "tcp_server.h"

using namespace boost::asio;
using ip::tcp;

int main(){
  tcp_server server1;

  server1.attep_conn();
  

}
