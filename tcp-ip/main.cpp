#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <thread>
<<<<<<< HEAD

#include "asynctcp.h"
=======
#include <boost/array.hpp>
#include <memory>
#include "tcp_server.h"
>>>>>>> master

#include <iostream>


using namespace boost::asio;
using namespace std;
using ip::tcp;


void read_keyboard(){
  while (1) {
    string command;
    getline(std::cin,command);
    if(command=="quit"){


int main(int argc, char* argv[])
{

  try
  {
    // if (argc != 2)
    // {
    //   std::cerr << "Usage: async_tcp_echo_server <port>\n";
    //   return 1;
    // }
    std::thread t1{read_keyboard};
    boost::asio::io_service io_service;

    using namespace std; // For atoi.
    tcp_server s(io_service, 17000);
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
