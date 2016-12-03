#include "float_circular_buffer.h"

using namespace std;

float_circular_buffer::float_circular_buffer(float buffer_size){
  buffer = new float[buffer_size];
  actual_position = 0;
  max_position = buffer_size;
  loop_number = 0;
}

float_circular_buffer::~float_circular_buffer(){
  delete [] buffer;
}

float float_circular_buffer::read_actual_value(){
  return buffer[actual_position-1];
}

float float_circular_buffer::read_n_value(float n){
  if(n>=max_position){
    return -10;
  }
  return buffer[n];
}

vector<float> float_circular_buffer::read_all_values(){
  if(actual_position == 0){
    vector<float> fifth;
    return fifth;
  }

  if(loop_number == 0){
    float return_vector [actual_position];
    for(float i =0;i<actual_position;i++){
      return_vector[i]=buffer[i];
    }
    vector<float> fifth (return_vector, return_vector + sizeof(return_vector) / sizeof(float) );
    return fifth;
  }

    float return_vector [max_position];
    float j = 0;
    for(float i=actual_position;i<max_position;i++){
      return_vector[j] = buffer[i];
      j=j+1;
    }
    for(float i=0;i<actual_position;i++){
      return_vector[j] = buffer[i];
      j=j+1;
    }

    vector<float> fifth (return_vector, return_vector + sizeof(return_vector) / sizeof(float) );
    return fifth;
}

void float_circular_buffer::insert_value(float value){
  if(actual_position < max_position){
    buffer[actual_position] = value;
    actual_position = actual_position +1;
  }else{
    loop_number = loop_number +1;
    actual_position = 0;
    buffer[actual_position] = value;
    actual_position = actual_position +1;
  }
}
