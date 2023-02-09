#include "WaveAnalysis.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>

#include "TString.h"
#include "TApplication.h"

int main(int argc, char *argv[])
{
  if(argc != 4){
    std::cout << "need parameter" << std::endl;
    std::cout << "like: ana 39 0 600" << std::endl;
    return -1;
  }

  int run = atoi(argv[1]);
  TString file_in = TString::Format("../../../../../data/CsI/source/run_R%04d.root", run);
  std::cout << "analysis " << file_in << std::endl;
  
  int q1_start = atoi(argv[2]);
  int q1_stop = atoi(argv[3]);

  TString file_out = TString::Format("./run_R%04d_qdc_%d.root", run, q1_stop);

  WaveAnalysis *wa = new WaveAnalysis(file_in.Data(), file_out.Data(), q1_start, q1_stop);
  
  wa->Process();
  delete wa;

  return 0;
}
