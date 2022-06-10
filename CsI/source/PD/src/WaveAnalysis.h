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
#include <fstream>

struct TRAPZ_PAR{
  Double_t decay_length;   // decay time of input trace (in points)
  UInt_t   rise_length;    // rise time of trapezoid (in points)
  UInt_t   falt_top;       // flat top of trapezoid (in points)
  UInt_t   pre_trigger;    // number of points used for baseline recovery
  UInt_t   peak_delay;     // peak delay after the trigger point
  UInt_t   peak_mean;      // peak mean for energy extraction
};

struct CFD_PAR{
  UInt_t   pre_trigger;    // number of points used for baseline recovery
  // CFD filter from XIA
  UInt_t   fast_length;    // fast length (in points)
  UInt_t   fast_gap;       // fast gap (in points)
  UInt_t   delay_length;   // delay length (in points)
  Double_t scale_factor;   // scaling factor (option: 0.25, 0.125, 0.0625, 0.03125)
  // RC-CR2 filter from CAEN
  UInt_t   smooth_length;  // smooth factor (in points)
  UInt_t   rise_length;    // rise time (in points)

  Int_t    threshold;      // threshold for hit finding
};

struct TRAPZ_RESULT{
  Double_t energy[PILEUPMAX];    // mean value of trapezoidal flat-top points
  Double_t time_tag[PILEUPMAX];  // trigger time tag for each hit
  UShort_t trigger_num;         // number of triggers
};

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

struct FIT_PAR{
  UInt_t fit_start;        // in points
  UInt_t fit_stop;         // in points
  // par[0]: baseline
  // par[1]: t0 (in points)
  // par[2]: tau rc (in points)
  // par[3]: tau fast (in points)
  // par[4]: tau slow (in points)
  // par[5]: am fast
  // par[6]: am slow
  Double_t par[7];
  Double_t par_min[7];
  Double_t par_max[7];
};

struct FIT_RESULT{
  Double_t tau_fast;
  Double_t tau_slow;
  Double_t am_fast;
  Double_t am_slow;
};

//
Double_t RCCR2XX(Double_t *v, UInt_t i, UInt_t window, UInt_t risetime);
Double_t RCCR2YY(Double_t *v, UInt_t i, UInt_t window);
Double_t Integral(Double_t *v, UInt_t i, UInt_t j);
Double_t Fittf(Double_t *i, Double_t *p);

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
  void CFD(Long64_t n);
  void RCCR2(Long64_t n);
  void Trapezoid(Long64_t n);

  //PID
  void CaliQDC(Long64_t n);
  void FitWave(Long64_t n);

private:
  Int_t entry_start;
  Int_t entry_stop;

private:
  TRAPZ_PAR trapz_par;
  CFD_PAR cfd_par;
  TRAPZ_RESULT trapz_result;

  UShort_t ltra;
  UShort_t data[MAXLENGTH];
  Double_t data_bl[MAXLENGTH];
  Double_t data_rccr2[MAXLENGTH];
  Double_t data_trapz[MAXLENGTH];
  Double_t baseline;

  //PID
  QDC_PAR qdc_par;
  QDC_RESULT qdc_result;
  FIT_PAR fit_par;
  FIT_RESULT fit_result;

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
