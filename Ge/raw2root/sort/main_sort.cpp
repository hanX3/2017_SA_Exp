#include "Sort.h"

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
    std::cout << "means analysis decode file R33_0 " << std::endl;
    return -1;
  }

  int run = atoi(argv[1]);
  int index = atoi(argv[2]);
  TString file_in = TString::Format("../../../../data/Ge/root/R%04d_%04d_decode_2.root", run, index);
  std::cout << "sort " << file_in << std::endl;
  
  TString file_out = TString::Format("../../../../data/Ge/root/R%04d_%04d_sort_2.root", run, index);

  Sort *so = new Sort(file_in.Data(), file_out.Data());
  so->Process();

  delete so;

  return 0;
}
