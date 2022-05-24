#ifndef SORT_H_
#define SORT_H_

#include "set.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string.h> //memset()
#include <stdlib.h> //malloc()

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TBenchmark.h"

void GetCloverCali(Double_t p[CLOVERCHANNELS][3]);

struct CloverData{
  Int_t id;//0-40 or 0-10
  Double_t energy;//cali
  Long64_t timestamp;
};

//
class Sort
{
public:
  Sort() = default;
  Sort(const std::string &filename_in, const std::string &filename_out);
  ~Sort();

public:
  void Process();

private:
  Double_t par[CLOVERCHANNELS][3];

private:
  void AddBack(Int_t c);//0,1,2,3,4,5,6,7,8,9,10

private:
  TBenchmark *benchmark;
  TFile *file_in;
  TFile *file_out;
};

#endif
