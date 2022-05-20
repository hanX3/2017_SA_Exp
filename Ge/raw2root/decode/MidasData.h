#ifndef MIDASDATA_H_
#define MIDASDATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "set.h"

class MidasData{
public:
  MidasData() = default;
  MidasData(const std::string &name);
  ~MidasData();

  unsigned int GetLabel() { return label; }
  unsigned int GetADC_TDC_QDC() { return adc_tdc_qdc; }
  unsigned int GetTimestamp() { return timestamp; }

public:
  bool ProcessBuff();
  void PrintInfo();

private:
  void Clear();
  int GetBuffFlag();
  bool GetNextTwoWords();

private:
  FILE *fp;
  unsigned int buff[2];  //read 8 bytes data every time

  unsigned short adc_tdc_qdc;  //adc,tdc,qdc value
  unsigned short label;
  unsigned int timestamp;

  const static unsigned int kMaskadctdcqdc  =     0x00003fff;
  const static unsigned int kShiftadctdcqdc =     0;
  const static unsigned int kMasklabel =          0x0fff0000;
  const static unsigned int kShiftlabel =         16;
  const static unsigned int kMasklabelFlag =      0xf0000000;
  const static unsigned int kShiftlabelFlag =     28;
  const static unsigned int kMasktimestamp =      0x0fffffff;
  const static unsigned int kShifttimestamp =     0;
  const static unsigned int kMasktimestampFlag =  0xf0000000;
  const static unsigned int kShifttimestampFlag = 24;
};

#endif
