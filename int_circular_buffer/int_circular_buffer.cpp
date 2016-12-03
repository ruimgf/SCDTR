#include "int_circular_buffer.h"
#include<memory>

using namespace std;

int_circular_buffer::int_circular_buffer(int buffer_size){
  buffer = new int[buffer_size];
  actual_position = 0;
  max_position = buffer_size;
  loop_number = 0;
}

int_circular_buffer::~int_circular_buffer(){
  delete [] buffer;
}

int read_actual_value(){
  return buffer[actual_position-1];
}

int read_n_value(int n){
  if(n>=max_position){
    return NULL;
  }
  return buffer[n];
}

int* read_all_values(){
  if(actual_position = 0){
    return NULL;
  }

  if(loop_number == 0){
    int return_vector [actual_position];
    for(int i =0;i<actual_position;i++){
      return_vector[i]=buffer[i];
    }
    return return_vector;
  }

    int return_vector [max_position];
    int j = 0;
    for(int i=actual_position;i<max_position;i++){
      return_vector[j] = buffer[i];
      j=j+1;
    }
    for(int i=0;i<actual_position;i++){
      return_vector[j] = buffer[i];
      j=j+1;
    }

    return return_vector;
}

void insert_value(int value){
  if(actual_position < max_position){
    buffer[actual_position] = value;
    actual_position = actual_position +1;
  }else{
    actual_position = 0;
    buffer[actual_position] = value;
    actual_position = actual_position +1;
  }
}
