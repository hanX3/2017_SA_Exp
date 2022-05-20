#ifndef DECODE_H_
#define DECODE_H_

#include "set.h"
#include "MidasData.h"
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TBenchmark.h"

//
int GetLabelType(int label);
int GetCloverID(int label);
int GetSegementID(int label);
bool GetADCorTDCFlag(int label);
int GetCsIID(int label);
int GetQDCID(int label);

//
class Decode
{
public:
  Decode() = default;
  Decode(const std::string &filename_in, const std::string &filename_out);
  ~Decode();

  bool ProcessEntry();
  void Process();

private:
  MidasData *md;
  TBenchmark *benchmark;
  Long64_t nevt;

private:
  TFile *file_out;
  TTree *tr;

  UShort_t label;
  UShort_t adc_qdc;
  UInt_t timestamp;
};

#endif
