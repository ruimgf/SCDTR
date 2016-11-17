#include <iostream>
using namespace std ;


void symm(int & j){ // pass j in reference 
  j= -1 *j;
}

int main(int argc, char const *argv[]) {
  int i = 3;

  symm(i);
  std::cout << "result: " << i;
  return 0;
}
