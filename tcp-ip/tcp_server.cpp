#include "tcp_server.h"
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>


using namespace std;
using namespace boost::asio;
using namespace boost::asio;
using ip::tcp;


tcp_server::tcp_server(){
  acceptor.open(endpoint.protocol());
  acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor.bind(endpoint);
  acceptor.listen();
}

int lum[3] = {4,5,6};
int duty[3] = {240,245,250};
int ocuppancy[3] = {0,0,1};
int lower[3] = {10,20,30};
int external[3] = {15,30,35};
int reference[3] = {10,1,12};
int power[3] = {100,200,300};
int error_ac[3] = {10120,20330,34400};
int variance[3] = {10,13,123};

string process_data(string str ){
  string response;
  if(str[0]=='g'){
    string number =  {str,4};
    int ilum_nr = atoi( number.c_str() ) - 1;
    switch (str[2]) {
      case 'l': // lumens

      response += "l ";
      response += str[4];
      response += " ";
      response += std::to_string(lum[ilum_nr]);
      break;
      case 'd': // duty
      response += "d ";
      response += str[4];
      response += " ";
      response += std::to_string(duty[ilum_nr]);

      break;
      case 'o': // ocuppancy
      response += "o ";
      response += str[4];
      response += " ";
      response += std::to_string(ocuppancy[ilum_nr]);

      break;
      case 'L': // lower bound
      response += "L ";
      response += str[4];
      response += " ";
      response += std::to_string(lower[ilum_nr]);

      break;
      case 'O': // external ILLUm
      response += "O ";
      response += str[4];
      response += " ";
      response += std::to_string(external[ilum_nr]);

      break;
      case 'r': // reference
      response += "r ";
      response += str[4];
      response += " ";
      response += std::to_string(reference[ilum_nr]);
      break;

      case 'p': // power
      if(str[4]=='T'){
        // insert code to total
        break;
      }
      response += "p ";
      response += str[4];
      response += " ";
      response += std::to_string(power[ilum_nr]);
      break;
      case 'e':// error confirm essay
      if(str[4]=='T'){
        // insert code to total
        break;
      }
      response += "e ";
      response += str[4];
      response += " ";
      response += std::to_string(error_ac[ilum_nr]);
      break;
      case 'v': // variance
      response += "v ";
      response += str[4];
      response += " ";
      response += std::to_string(variance[ilum_nr]);
      break;

      default:
      break;
    }
  }else if(str[0]=='s'){
    string aux =  {str,2};
    int ilum_nr = atoi( aux.c_str() ) - 1;
    string aux1 =  {str,4};
    int state = atoi( aux1.c_str() );
    // change occupancy
    std::cout << "change illum " << ilum_nr << "to state" << state << '\n';
    response = "sucess";
  } else if(str[0]=='r'){
    // insert code to reset
    response = "sucess";
  }
  if(!response.empty()){

    return response;
  }else{
    response = "error";
    return response;
  }

}

void tcp_server::attep_conn(){

  //create service socket
  boost::system::error_code err;
  tcp::socket socket(io);
  //wait client to	connect
  acceptor.accept(socket);
  std::cout << "accept" << '\n';
  //client is accessing service
  for (;;) {
    boost::array <char,128>	buf;


    size_t len =	socket.read_some(buffer(buf),	err);
    if (err ==	error::eof)			break;	//	Closed cleanly by peer.
    else if (err)		std::cout <<	"Unknown Error";

    string question = buf.data();

    string str;

    std::cout.write(buf.data(),	len);

    str = process_data(question);

    write(socket,	buffer(str));
  }


}
