#include "int_circular_buffer.h"
#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std;

int main(){
  
  vector<int> fifth;
  int_circular_buffer buffer1{10};

  buffer1.insert_value(10);
  buffer1.insert_value(20);

  fifth = buffer1.read_all_values();

  cout << "The contents of fifth are:";
  for (vector<int>::iterator it = fifth.begin(); it != fifth.end(); ++it)
    cout << ' ' << *it;
  cout << '\n';


}
