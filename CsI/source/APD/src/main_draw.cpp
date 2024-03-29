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
  if(argc != 3){
    std::cout << "need parameter" << std::endl;
    std::cout << "like: ana 39 0" << std::endl;
    std::cout << "means analysi file 39 entry 0 " << std::endl;
    return -1;
  }

  TApplication ac("app", &argc, argv);

  int run = atoi(argv[1]);
  TString file_in = TString::Format("../../../../../data/CsI/source/run_R%04d.root", run);
  std::cout << "analysis " << file_in << std::endl;
  
  int entry = atoi(argv[2]);
  TString file_out = TString::Format("../../../../../data/CsI/source/run_R%04d_draw.root", run);

  WaveAnalysis *wa = new WaveAnalysis(file_in.Data(), file_out.Data(), 0, 0);
  
  wa->DrawEntry((Long64_t)entry);
  wa->ProcessEntry((Long64_t)entry);
  // wa->DrawMultiRCCR2();

  ac.Run();

  return 0;
}
