#ifndef WAVEANALYSIS_H_
#define WAVEANALYSIS_H_

#include "set.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TMultiGraph.h"

#include <string.h>
#include <stdlib.h>

#include <iostream>

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
  Float_t energy;
  Float_t time;
};

//
Double_t RCCR2XX(Double_t *v, UInt_t i, UInt_t window, UInt_t risetime);
Double_t RCCR2YY(Double_t *v, UInt_t i, UInt_t window);

//
class WaveAnalysis
{
public:
  WaveAnalysis() = default;
  WaveAnalysis(const std::string &filename_in, const std::string &filename_out);
  ~WaveAnalysis();

public:
  bool ProcessEntry(Long64_t n);
  void DrawDraw(Long64_t n);
  void DrawMultiRCCR2();

private:
  bool GetWave(Long64_t n);
  void CFD(Long64_t n);
  void RCCR2(Long64_t n);
  void Trapezoid(Long64_t n);

private:
  TRAPZ_PAR trapz_par;
  CFD_PAR cfd_par;
  TRAPZ_RESULT trapz_result;

  UShort_t ltra;
  UShort_t data[MAXLENGTH];
  Short_t data_rccr2[MAXLENGTH];
  Short_t data_trapz[MAXLENGTH];
  Double_t baseline;

private:
  TFile *file_in;
  TTree *tr_in;

  TCanvas *cav;
};

#endif
