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
  bool IsInsideProton(Int_t id, Double_t x, Double_t y);
  bool IsInsideAlpha(Int_t id, Double_t x, Double_t y);
  void ProcessEntry(Long64_t n);

private:
  Double_t ax_proton[CSINUMBER];
  Double_t ay_proton[CSINUMBER];
  Double_t bx_proton[CSINUMBER];
  Double_t by_proton[CSINUMBER];
  Double_t cx_proton[CSINUMBER];
  Double_t cy_proton[CSINUMBER];
  Double_t dx_proton[CSINUMBER];
  Double_t dy_proton[CSINUMBER];

  Double_t ax_alpha[CSINUMBER];
  Double_t ay_alpha[CSINUMBER];
  Double_t bx_alpha[CSINUMBER];
  Double_t by_alpha[CSINUMBER];
  Double_t cx_alpha[CSINUMBER];
  Double_t cy_alpha[CSINUMBER];
  Double_t dx_alpha[CSINUMBER];
  Double_t dy_alpha[CSINUMBER];
  
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

  Int_t proton_num;
  Int_t alpha_num;

  Int_t csi_cut_hit;
  Int_t csi_cut_id[CSINUMBER];
  Int_t csi_cut_type[CSINUMBER];//1-->proton, 2-->alpha
  Double_t csi_cut_qdc1[CSINUMBER];
  Double_t csi_cut_qdc2[CSINUMBER];
  Double_t csi_cut_qdc3[CSINUMBER];
  Double_t csi_cut_qdc4[CSINUMBER];
  
};

#endif
