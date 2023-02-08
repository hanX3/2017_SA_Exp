#include "WaveAnalysis.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>

#include "TString.h"
#include "TApplication.h"

#define DEBUGDRAWOPTION

//
int main(int argc, char *argv[])
{
  if(argc != 2){
    std::cout << "need parameter" << std::endl;
    std::cout << "like: ana 0" << std::endl;
    std::cout << "means analysi entry 0 " << std::endl;
    return -1;
  }

  TApplication ac("app", &argc, argv);

  TString file_in = TString::Format("../../../../../data/CsI/inbeam/mod2-DDAS-2017-12-08-14-44-59/data_ch00_300lsb.root");
  std::cout << "analysis " << file_in << std::endl;
  
  int entry = atoi(argv[1]);
  TString file_out = TString::Format("../../../../../data/CsI/inbeam/mod2-DDAS-2017-12-08-14-44-59/draw_data_ch00.root");

  WaveAnalysis *wa = new WaveAnalysis(file_in.Data(), file_out.Data(), 0, 0);
  
  wa->DrawEntry((Long64_t)entry);

  ac.Run();

  return 0;
}
