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
bool GetADCorTDCFlag(int label);

int GetCloverChannel(int label); //0-99
int GetCsIChannel(int label);    //300-399
int GetBGOChannel(int label);    //100-199
int GetLaBr3Channel(int label);  //200-299

int GetCloverID(int label);      //0-9
int GetSegementID(int label);    //0-3
int GetQDCID(int label);         //0-3

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
