#include "WaveAnalysis.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>

#include "TString.h"
#include "TApplication.h"

int main(int argc, char *argv[])
{
  if(argc != 3){
    std::cout << "need parameter" << std::endl;
    std::cout << "like: ana 0 1000" << std::endl;
	std::cout << "means analysi file entry 0 to 1000 " << std::endl;
	return -1;
  }

  TString file_in = TString::Format("../../../../../data/CsI/inbeam/mod2-DDAS-2017-12-08-14-44-59/data_ch00_300lsb.root");
  std::cout << "analysis " << file_in << std::endl;
  
  int entry_start = atoi(argv[1]);
  int entry_stop = atoi(argv[2]);
  TString file_out = TString::Format("../../../../../data/CsI/inbeam/mod2-DDAS-2017-12-08-14-44-59/300lsb/data_ch00_%09d_%09d_ana.root", entry_start, entry_stop);

  WaveAnalysis *wa = new WaveAnalysis(file_in.Data(), file_out.Data(), entry_start, entry_stop);
  
  wa->Process();
  delete wa;

  return 0;
}
