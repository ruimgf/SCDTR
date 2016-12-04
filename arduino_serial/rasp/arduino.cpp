#include "arduino.h"
#include <thread>
#include <chrono>
#include <ctime>

using namespace std;


arduino::arduino(string port_name)
  :ard(port_name)
  {
  lux[0] = lux[1] = lux[2] = 0;
  duty = 0;
  E=0;
  V_f=0;
  C_e=0;
  occupancy=0;
  ref = 0;

  //connect to	port
  try{
  ard.Open(SerialPort::BaudRate::BAUD_115200);
  }catch(SerialPort::NotOpen){
    std::cout << "not open port" << endl;
  }
  std::cout << "open port" << endl;

  //wait 2 sec - need more code because signals sended by libserial
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  while(1){
    std::this_thread::sleep_for(std::chrono::nanoseconds(1000000));
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    if(elapsed_seconds.count() >= 2){
      break;
    }
  }

  /*asks for id*/
  ard.Write("W");
  /*receive id*/
  string data_id = ard.ReadLine(0,'\n');
  id = stoi(data_id);
  std::cout << "ARD ID:" << id << endl;

}

arduino::~arduino(){
  ard.Close();
}
/*função que recolhe os dados do control loop*/
void arduino::retrive_control_loop_data(){
  /*ask values*/
    ard.Write("W");
  /*lê duty*/
  {
    string data = ard.ReadLine(0,'\n');
    duty = stof(data);
  }
  last_duty.insert_value(duty);
  /*lê lux*/
  lux[2] = lux[1];
  lux[1] = lux[0];
  {
    string data = ard.ReadLine(0,'\n');
    lux[0] = stof(data);
  }
  last_lum.insert_value(lux[0]);

  cout << lux[0] << endl;
  cout << duty << endl;
  /*lê timestamp1*/
  {
    string data = ard.ReadLine(0,'\n');
    cout << "timestamp:" << data <<  endl;
  }

}
