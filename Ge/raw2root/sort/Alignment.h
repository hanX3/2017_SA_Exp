#ifndef ALIGNMENT_H_
#define ALIGNMENT_H_

#include "set.h"

#include "TBenchmark.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

#include <vector>
#include <iostream>

//
class Alignment
{
public:
  Alignment() = default;
  Alignment(const std::string &filename_in, const std::string &filename_out);
  ~Alignment();

public:
  void Process();
  void GetCloverTimeInterval();
  void GetCsITimeInterval();

private:
  void GetCloverTimestampVector(TTree *tr, std::vector<Long64_t> &vec);
  void GetCsITimestampVector(TTree *tr, std::vector<Long64_t> &vec);

private:
  TBenchmark *benchmark;

  TFile *file_in;
  TTree *tr_Clover[CLOVERCHANNELS];
  TTree *tr_CsI[CSICHANNELS];

  UShort_t ch_Clover;
  Long64_t timestamp_Clover;

  UShort_t ch_CsI;
  UShort_t qdc_CsI[4];
  Long64_t timestamp_CsI;

  Int_t id_clover;//the first clover channel with data
  Int_t id_csi;//the first csi channel with data
  TFile *file_out;
  TH1D *h1[CLOVERCHANNELS-1];//clover vs. clover
  TH1D *h2[CSICHANNELS-1];//csi vs. csi
  TH1D *hh[CSICHANNELS];//clover vs csi

  TH1D *hi_clover[CLOVERCHANNELS];//timestamp interval
  TH1D *hi_csi[CSICHANNELS];//timestamp interval
};

#endif
