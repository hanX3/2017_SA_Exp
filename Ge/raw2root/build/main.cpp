#include "Build.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>

#include "TString.h"

int main(int argc, char const *argv[])
{
  if(argc != 3){
    std::cout << "need parameter" << std::endl;
    std::cout << "like: sort 33 0" << std::endl;
    std::cout << "means analysis file R33_0 " << std::endl;
    return -1;
  }

  int run = atoi(argv[1]);
  int index = atoi(argv[2]);
  TString file_in1 = TString::Format("../../../../data/Ge/root/R%04d_%04d_decode_2.root", run, index);
  TString file_in2 = TString::Format("../../../../data/Ge/root/R%04d_%04d_sort_2.root", run, index);
  std::cout << "build " << file_in1 << std::endl;
  
  TString file_out = TString::Format("../../../../data/Ge/root/R%04d_%04d_build_1.root", run, index);

  Build *me = new Build(file_in1.Data(), file_in2.Data(), file_out.Data());
  me->Process();

  delete me;

  return 0;
}
