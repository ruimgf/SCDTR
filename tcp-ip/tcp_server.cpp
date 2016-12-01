#include "tcp_server.h"


using namespace std;
using namespace boost::asio;
using namespace boost::asio;		using ip::tcp;

class tcp_server {
  private:		tcp::acceptor acceptor_;
  public:		tcp_server(io_service&	io)
  :	acceptor_(io,	tcp::endpoint(tcp::v4(),	10000))		{
    start_accept();
  }
private:
  void start_accept()	{
    tcp_connection::pointer new_con =
    tcp_connection::create(acceptor_.get_io_service());
    acceptor_.async_accept(new_con->socket(),
    boost::bind(&tcp_server::handle_accept,	this,
      new_con));
    }
    void handle_accept(tcp_connection::pointer new_con)		{
      new_con->start();
      start_accept();
    }
  };

  class tcp_connection :		public
  boost::enable_shared_from_this<tcp_connection>	{
  private:
  tcp::socket sock_;
  std::string msg_;
  tcp_connection(io_service&	io)	:		sock_(io)		{}
  void handle_write()			{/*nothing important*/}
  public:
  typedef boost::shared_ptr<tcp_connection>	pointer;
  static pointer create(io_service&	io)	{
  return pointer(new tcp_connection(io));
  }
  tcp::socket&	socket()	{return socket_;}
  void start()	{
  async_write(sock_,buffer(“Hello World”),
  boost::bind(&tcp_connection::handle_write,
  shared_from_this()));
  }
  };