#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <thread>

#include "tcp_server.h"



using namespace boost::asio;
using namespace std;
using ip::tcp;

tcp_server server1;


void thread_session(){
  while(1){
    server1.attep_conn();
  }

}

int main(){
  std::cout << "go" << '\n';
  std::thread t1{thread_session};
  std::thread t2{thread_session};
  std::thread t3{thread_session};



  while (1) {
    string str;
    std::getline (std::cin,str);
    if(str == "quit"){
      break;
    }
  }
}
