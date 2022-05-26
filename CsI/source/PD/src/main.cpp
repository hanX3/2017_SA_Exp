#include "WaveAnalysis.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>

#include "TString.h"
#include "TApplication.h"

int main(int argc, char *argv[])
{
  if(argc != 2){
    std::cout << "need parameter" << std::endl;
    std::cout << "like: ana 33" << std::endl;
	std::cout << "means analysi file R33_0 " << std::endl;
	return -1;
  }

  TApplication ac("app", &argc, argv);

  int run = atoi(argv[1]);
  TString file_in = TString::Format("../../../../../data/CsI/source/data_R%04d.root", run);
  std::cout << "analysis " << file_in << std::endl;
  
  TString file_out = TString::Format("../../../../../data/CsI/source/data_R%04d_ana.root", run);

  WaveAnalysis *wa = new WaveAnalysis(file_in.Data(), file_out.Data());
  //wa->DrawDraw(113);
  wa->DrawMultiRCCR2();
  
  ac.Run();

  //delete wa;

  return 0;
}
