#ifndef CUT_H_
#define CUT_H_

#include "set.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TBenchmark.h"

#include <stdlib.h>
#include <fstream>
#include <iostream>

class Cut
{
public:
  Cut() = default;
  Cut(const std::string &filename_in, const std::string &filename_out);
  ~Cut();

public:
  void Process();

private:
  void Clear();
  bool IsInside(Int_t id, Double_t x, Double_t y);
  void ProcessEntry(Long64_t n);

private:
  Double_t ax[CSINUMBER];
  Double_t ay[CSINUMBER];
  Double_t bx[CSINUMBER];
  Double_t by[CSINUMBER];
  Double_t cx[CSINUMBER];
  Double_t cy[CSINUMBER];
  Double_t dx[CSINUMBER];
  Double_t dy[CSINUMBER];

private:
  TBenchmark *benchmark;
  TFile *file_in;
  TTree *tr_in;

  Int_t clover_hit;
  Int_t clover_id[CLOVERNUMBER];
  Double_t clover_energy[CLOVERNUMBER];

  Int_t csi_hit;
  Int_t csi_id[CSINUMBER];
  Double_t csi_qdc1[CSINUMBER];
  Double_t csi_qdc2[CSINUMBER];
  Double_t csi_qdc3[CSINUMBER];
  Double_t csi_qdc4[CSINUMBER];

  Long64_t total_entry;
  Long64_t nevt;

  TFile *file_out;
  TTree *tr_out;
  Int_t clover_cut_hit;
  Int_t clover_cut_id[CLOVERNUMBER];
  Double_t clover_cut_energy[CLOVERNUMBER];

  Int_t csi_cut_hit;
  Int_t csi_cut_id[CSINUMBER];
  Double_t csi_cut_qdc1[CSINUMBER];
  Double_t csi_cut_qdc2[CSINUMBER];
  Double_t csi_cut_qdc3[CSINUMBER];
  Double_t csi_cut_qdc4[CSINUMBER];
  
};

#endif
