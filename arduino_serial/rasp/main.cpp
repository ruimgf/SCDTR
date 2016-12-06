#include <iostream>
#include <boost/asio.hpp>
#include	<algorithm>
#include	<vector>
#include "arduino.h"
#include "defs.h"
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>

using namespace std;

/*struct para passar dados para as threads na sua criação*/
typedef struct thread_data{
   int arduino_id;
}thread_data;

std::string s1 = "/dev/ttyACM1";
std::string s2 = "/dev/ttyACM0";

arduino* arduinos[2];
pthread_mutex_t cout_mutex;


void *thread_session(void* thread_arg){

  thread_data *my_data;

  my_data = (thread_data*) thread_arg;

  while(1){

      cout << "Thread:" << my_data->arduino_id << endl;
      arduinos[my_data->arduino_id]->retrive_control_loop_data();
  }

  pthread_exit(NULL);
}


int main(){
  pthread_t threads[NUMBER_ILLUM];
  thread_data td[NUMBER_ILLUM];
  int rc;
  arduinos[0] = new arduino{s1};
  arduinos[1] = new arduino{s2};

  /*for(int i=0;i<NUMBER_ILLUM;i++){
    td[i].arduino_id = i;
    rc = pthread_create(&threads[i],NULL,thread_session,(void*)&td[i]);
    if(rc){
      cout << "ERROR CREATING THREAD" << rc << endl;
      exit(-1);
    }

  }*/

  while(1){
    arduinos[0]->retrive_control_loop_data();
    arduinos[1]->retrive_control_loop_data();
  }

  pthread_exit(NULL);
}
