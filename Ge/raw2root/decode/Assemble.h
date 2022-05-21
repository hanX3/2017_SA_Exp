#ifndef ASSEMBLE_H_
#define ASSEMBLE_H_

#include "set.h"
#include "Decode.h"

//
class Assemble
{
public:
  Assemble() = default;			
  Assemble(const std::string &filename_in, const std::string &filename_out);
  ~Assemble();

  bool ProcessEntryClover();
  bool ProcessEntryCsI();
  void Process();

private:
  TFile *file_in;
  TTree *tr_in;
  UShort_t label_in;
  UShort_t adc_qdc_in;
  UInt_t timestamp_in;

  TFile *file_out;
  TTree *tr;
  TTree *tr_Clover[CLOVERCHANNELS];
  TTree *tr_CsI[CSICHANNELS];

  UShort_t label;
  UShort_t ch;
  UShort_t adc;
  UShort_t qdc[4];
  UInt_t timestamp;

  TBenchmark *benchmark;
  Long64_t nevt;
  Long64_t nevt_abandon;

  Long64_t total_nevt;

  Long64_t nevt_Clover[CLOVERCHANNELS];
  Long64_t nevt_CsI[CSICHANNELS];
};

#endif
