//Declaração de cabeçalhos de funções
#include <iostream>
#include "input.h"
using namespace std;

int main(int argc, char* argv[]) {
  Input in;
  in.load("Instance1.txt");
  std::cout << "O horizonte eh "<<in.horizonGet()<<endl;

  for(unsigned i=0; i < in.shift_request_.size(); i++){
    cout<<in.shift_request_[i].employee_id_<<","
    <<in.shift_request_[i].day_<<","
    <<in.shift_request_[i].shift_id_<<","
    <<in.shift_request_[i].weight_<<endl;
  }
  return 0;
}
