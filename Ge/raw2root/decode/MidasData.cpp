#include "MidasData.h"
#include <assert.h>
#include <iostream>

using std::hex; using std::cout;

//
MidasData::MidasData(const std::string &name)
{
  fp = fopen(name.c_str(), "rb");
  if(!fp){
    std::cout << "can not open " << name << "\n";
    assert(1);
  }

  buff[0] = 0;
  buff[1] = 0;

  adc_tdc_qdc = 0;
  label = 0;
  timestamp = 0;
}

//
MidasData::~MidasData()
{
  fclose(fp);
}

//
void MidasData::Clear()
{
  buff[0] = 0;
  buff[1] = 0;

  adc_tdc_qdc = 0;
  label = 0;
  timestamp = 0;
}

//
int MidasData::GetBuffFlag()
{
  if(buff[0] == 0x45594245 && buff[1] == 0x41544144){
    return 1;
  }
  if(buff[0] == 0x5e5e5e5e && buff[1] == 0x5e5e5e5e){
    return 2;
  }
  if(((buff[1]&kMasklabelFlag)>>kShiftlabelFlag) != 0xc){
    return 3;
  }

  return 4; //read useful data
}

//
bool MidasData::GetNextTwoWords()
{
  size_t i = fread(&buff, sizeof(unsigned int), 2, fp);

  if(i != 2){
    std::cout << "cannot read two words." << "\n";
    return false;
  }

#ifdef DEBUGMIDASDATA
  std::cout << hex << "buff[0] = " << buff[0] << "\n";
  std::cout << hex << "buff[1] = " << buff[1] << "\n";
#endif

  //if read block header, block tail or 8340
  //then read two words again, until read useful data
  while(GetBuffFlag() != 4){
    //read block header
    if(GetBuffFlag() == 1){
#ifdef DEBUGMIDASDATA
      std::cout << "block header" << "\n";
#endif
      fseek(fp, 16, SEEK_CUR);

      if((fread(&buff, sizeof(unsigned int), 2, fp)) != 2){
#ifdef DEBUGMIDASDATA
        std::cout << "block header, then error" << "\n";
        std::cout << "current position " << ftell(fp) << "\n";
#endif
        return false;
      }
#ifdef DEBUGMIDASDATA
      std::cout << hex << "buff[0] = " << buff[0] << "\n";
      std::cout << hex << "buff[1] = " << buff[1] << "\n";
#endif
    }
    //read block tail
    else if(GetBuffFlag() == 2){
#ifdef DEBUGMIDASDATA
      std::cout << "block tail" << "\n";
#endif
      if((fread(&buff, sizeof(unsigned int), 2, fp)) != 2){
#ifdef DEBUGMIDASDATA
        std::cout << "block tail, then error" << "\n";
        std::cout << "current position " << ftell(fp) << "\n";
#endif
        return false;
      }
#ifdef DEBUGMIDASDATA
      std::cout << hex << "buff[0] = " << buff[0] << "\n";
      std::cout << hex << "buff[1] = " << buff[1] << "\n";
#endif
    }
    //read 0x8340
    else{
#ifdef DEBUGMIDASDATA
          std::cout << "like 0x8340" << "\n";
#endif
          if((fread(&buff, sizeof(unsigned int), 2, fp)) != 2){
#ifdef DEBUGMIDASDATA
            std::cout << "like 0x8340, then error " << "\n";
            std::cout << "current position " << ftell(fp) << "\n";
#endif
            return false;
          }
#ifdef DEBUGMIDASDATA
          std::cout << hex << "buff[0] = " << buff[0] << "\n";
          std::cout << hex << "buff[1] = " << buff[1] << "\n";
#endif
        }
  }
  return true;
}

//
void MidasData::PrintInfo()
{
  std::cout << hex << "timestamp = " << timestamp << "\n";
  std::cout << hex << "adc_tdc_qdc   = " << adc_tdc_qdc << "\n";
  std::cout << hex << "label     = " << label << "\n";
  std::cout << "\n";
}

//
bool MidasData::ProcessBuff()
{
  Clear();

  if(!GetNextTwoWords()){
    return false;
  }

  timestamp   = ((buff[0]&kMasktimestamp)>>kShifttimestamp);
  adc_tdc_qdc = ((buff[1]&kMaskadctdcqdc)>>kShiftadctdcqdc);
  label       = ((buff[1]&kMasklabel)>>kShiftlabel);
 
#ifdef DEBUGMIDASDATA
  PrintInfo();
#endif

  return true;
}

