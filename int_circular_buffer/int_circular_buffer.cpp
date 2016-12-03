#include "int_circular_buffer.h"

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

int int_circular_buffer::read_actual_value(){
  return buffer[actual_position-1];
}

int int_circular_buffer::read_n_value(int n){
  if(n>=max_position){
    return -10;
  }
  return buffer[n];
}

vector<int> int_circular_buffer::read_all_values(){
  if(actual_position == 0){
    vector<int> fifth;
    return fifth;
  }

  if(loop_number == 0){
    int return_vector [actual_position];
    for(int i =0;i<actual_position;i++){
      return_vector[i]=buffer[i];
    }
    vector<int> fifth (return_vector, return_vector + sizeof(return_vector) / sizeof(int) );
    return fifth;
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

    vector<int> fifth (return_vector, return_vector + sizeof(return_vector) / sizeof(int) );
    return fifth;
}

void int_circular_buffer::insert_value(int value){
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
