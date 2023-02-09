#ifndef WAVEANALYSIS_H_
#define WAVEANALYSIS_H_

#include "set.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TF1.h"
#include "TGraph.h"
#include "TBenchmark.h"
#include "TMultiGraph.h"

#include <string.h>
#include <stdlib.h>

#include <iostream>

struct QDC_PAR{
  UInt_t q1_start;         // in points
  UInt_t q1_stop;          // in points
  UInt_t q2_start;         // in points
  UInt_t q2_stop;          // in points
};

struct QDC_RESULT{
  Double_t qdc_short;      // 
  Double_t qdc_long;       // 
};

//
Double_t Integral(Double_t *v, UInt_t i, UInt_t j);

//
class WaveAnalysis
{
public:
  WaveAnalysis() = default;
  WaveAnalysis(const std::string &filename_in, const std::string &filename_out, int a, int b);
  ~WaveAnalysis();

public:
  void Process();
  bool ProcessEntry(Long64_t n);
#ifdef DEBUGDRAWOPTION
  void DrawEntry(Long64_t n);
  void DrawMultiRCCR2();
#endif

private:
  bool GetWave(Long64_t n);

  //PID
  void CaliQDC(Long64_t n);

private:

  UShort_t ltra;
  UShort_t data[MAXLENGTH];
  Double_t data_pol[MAXLENGTH];
  Double_t data_bl[MAXLENGTH];
  Double_t baseline;

  //PID
  QDC_PAR qdc_par;
  QDC_RESULT qdc_result;

private:
  TBenchmark *benchmark;

  TFile *file_in;
  TTree *tr_in;
  Long64_t total_entry;

  TFile *file_out;
  TTree *tr_out;

#ifdef DEBUGDRAWOPTION
  TCanvas *cav1;
  TCanvas *cav2;
#endif
};

#endif
