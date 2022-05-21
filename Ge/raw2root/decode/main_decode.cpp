#include "MidasData.h"
#include "Decode.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>

#include "TString.h"

int main(int argc, char const *argv[])
{
  if(argc != 3){
    std::cout << "need parameter" << std::endl;
    std::cout << "like: decode 33 0" << std::endl;
	std::cout << "means analysis decode file R33_0 " << std::endl;
	return -1;
  }

  int run = atoi(argv[1]);
  int index = atoi(argv[2]);
  TString file_in = TString::Format("../../../../data/Ge/raw/R%d_%d", run, index);
  std::cout << "decode " << file_in << std::endl;
  
  TString file_out = TString::Format("../../../../data/Ge/root/R%04d_%04d_decode_1.root", run, index);

  Decode *de = new Decode(file_in.Data(), file_out.Data());
  de->Process();

  delete de;

  return 0;
}
