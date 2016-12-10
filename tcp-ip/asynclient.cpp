#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <boost/bind.hpp>
using namespace boost::asio;
using ip::tcp;
using namespace std;
using namespace boost::system;

io_service io;
tcp::socket socket_(io);

	//connect and wait
void handle_write(){

}

void read_keyboard(){
  while (1) {
    std::string str1;
    std::getline (std::cin,str1);
    if(str1=="quit"){
      exit(0);
    }else{

      boost::asio::async_write(socket_,boost::asio::buffer(str1, str1.length()),boost::bind(&handle_write));
    }

  }
}

char response[10000];

void handle_read(const boost::system::error_code& error,size_t bytes_transferred){
  string saida{response};
  std::cout << saida << '\n';

    socket_.async_read_some(boost::asio::buffer(response,10000),
        &handle_read);
}




int main(){
  std::cout << "qui" << '\n';
  boost::system::error_code err;
  std::cout << "oalalal" << '\n';

  tcp::resolver	resolver(io);
  std::cout << "oalalal" << '\n';

  tcp::resolver::query query("127.0.0.1",	"17000");
  std::cout << "oalalal" << '\n';

  tcp::resolver::iterator endpoint =	resolver.resolve(query);
  std::cout << "oalalal" << '\n';
  socket_.connect(*endpoint,err);
  boost::array <char,100000>	buf;
  std::thread t1{read_keyboard};


  socket_.async_read_some(boost::asio::buffer(response,10000),&handle_read);

  io.run();

  std::cout << "go out" << '\n';
}
