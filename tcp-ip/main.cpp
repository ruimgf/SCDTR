#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <boost/array.hpp>
#include <memory>
#include "tcp_server.h"

#include <iostream>


using namespace boost::asio;
using namespace std;
using ip::tcp;

tcp_server server1;


void keyboard(){
  while (1) {
    string str;
    std::getline (std::cin,str);
    if(str == "quit"){
      exit(0);
    }
  }
}

void attep_client(tcp::socket * socket){
  tcp::socket * socket1 = socket;
  //std::cout << "in" << '\n';
  for (;;) {
    boost::array <char,128>	buf;
    boost::system::error_code err;

    //std::cout << "go read" << '\n';
    size_t len;
    try{

      len =	socket1->read_some(buffer(buf),	err);

    }catch(boost::system::system_error &e){

      boost::system::error_code ec =	e.code();
      std::cerr << "error in set:"<<	ec.value()	<<	std::endl;

    }

    if (err ==	error::eof)			break;	//	Closed cleanly by peer.
    else if (err)		std::cout <<	"Unknown Error";
    //std::cout << "after read" << '\n';
    string question = buf.data();

    string str;

    std::cout.write(buf.data(),	len);

    str = process_data(question);
    write(*socket1,	buffer(str));
  }


}

int main(){
  std::cout << "go" << '\n';
  std::thread t1{keyboard};
  //t1.detach();
  while (1) {
    // mutex de guarda
    tcp::socket * socket = server1.attep_conn();
    std::thread t1{attep_client,socket};
    t1.detach();
  }
}
