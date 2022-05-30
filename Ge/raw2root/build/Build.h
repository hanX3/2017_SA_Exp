#ifndef BUILD_H_
#define BUILD_H_

#include "set.h"

#include "TBenchmark.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

#include <stdlib.h>
#include <vector>
#include <iostream>

//
class Build
{
public:
  Build() = default;
  Build(const std::string &filename_in1, const std::string &filename_in2, const std::string &filename_out);
  ~Build();

public:
  void Process();

private:
  void Sort();
  void Clear();

private:
  Int_t id_clover;
  Double_t energy_clover;
  Long64_t timestamp_clover;

  UShort_t id_csi;
  UShort_t qdc_csi[4];
  Long64_t timestamp_csi;

private:
  Long64_t total_entry;
  Long64_t *ts;
  Int_t *ts_id;
  Long64_t min_tag[CLOVERNUMBER+CSINUMBER];
  Long64_t max_tag[CLOVERNUMBER+CSINUMBER];

private:
  TBenchmark *benchmark;

  TFile *file_in1;
  TFile *file_in2;
  TTree *tr_clover[CLOVERNUMBER];
  TTree *tr_csi[CSINUMBER];

  TFile *file_out;
  TTree *tr_out;

  Int_t clover_hit;
  Int_t clover_id[CLOVERNUMBER];
  Double_t clover_energy[CLOVERNUMBER];
  Long64_t clover_timestamp[CLOVERNUMBER];
  Int_t csi_hit;
  Int_t csi_id[CSINUMBER];
  Double_t csi_qdc1[CSINUMBER];
  Double_t csi_qdc2[CSINUMBER];
  Double_t csi_qdc3[CSINUMBER];
  Double_t csi_qdc4[CSINUMBER];
  Long64_t csi_timestamp[CSINUMBER];
};


#endif
