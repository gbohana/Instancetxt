//Declaração de cabeçalhos de funções
#include <iostream>
#include "input.h"
using namespace std;

int main(int argc, char* argv[]) {
  Input in;
  in.load("Instance1.txt");
  std::cout << "O horizonte eh "<<in.horizonGet()<<endl;

  for(unsigned i=0; i < in.shift_request.size(); i++){
    cout<<in.shift_request[i].employeeID<<","
    <<in.shift_request[i].day<<","
    <<in.shift_request[i].shiftID<<","
    <<in.shift_request[i].weight<<endl;
  }
  return 0;
}