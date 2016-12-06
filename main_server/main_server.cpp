#include <iostream>
#include <boost/asio.hpp>
#include	<algorithm>
#include	<vector>
#include "arduino.h"
#include "defs.h"
#include <cstdlib>
#include <pthread.h>


using namespace std;

/*struct para passar dados para as threads na sua criação*/
typedef struct thread_data{
   int arduino_id;
}thread_data;

arduino arduinos[NUMBER_ILLUM];
pthread_mutex_t cout_mutex;


void *thread_session(void* thread_arg){

  thread_data *my_data;

  my_data = (thread_data*) thread_arg;

  while(1){
      pthread_mutex_lock(&cout_mutex);
      cout << "Thread:" << my_data->arduino_id << endl;
      arduinos[my_data->arduino_id].retrive_control_loop_data();
      pthread_mutex_unlock(&cout_mutex);

  }

  pthread_exit(NULL);
}


int main(){
  pthread_t threads[NUMBER_ILLUM];
  thread_data td[NUMBER_ILLUM];
  arduinos[0](PORT_ILLUM0); /*arduino 1;*/
  arduinos[1](PORT_ILLUM1);
  int rc;

  for(int i=0;i<NUMBER_ILLUM;i++){
    td[i].arduino_id = i;
    rc = pthread_create(&threads[i],NULL,thread_session,(void*)&td[i]);
    if(rc){
      cout << "ERROR CREATING THREAD" << rc << endl;
      exit(-1);
    }

  }


  pthread_exit(NULL);
}
