#include "Decode.h"
#include <iostream>
#include <stdio.h>
#include <iomanip>      //std::setw
#include <string.h>     //memcpy()

//
Decode::Decode(const std::string &filename_in, const std::string &filename_out)
{
  md = new MidasData(filename_in.c_str());
  benchmark = new TBenchmark;
  nevt = 0;

  label = 0;
  adc_qdc = 0;
  timestamp = 0;

  file_out = TFile::Open(filename_out.c_str(), "recreate");
  tr = new TTree("tr", "tmp rootfile 1");

  tr->Branch("label", &label, "label/s");
  tr->Branch("adc_qdc", &adc_qdc, "adc_qdc/s");
  tr->Branch("timestamp", &timestamp, "timestamp/i");
}

//
Decode::~Decode()
{
  delete md;
}

//
bool Decode::ProcessEntry()
{
  if(!md->ProcessBuff()){
#ifdef DEBUGDECODE
    std::cout << "end of raw file" << std::endl;
#endif
    return false;
  }

  nevt++;
#ifdef DEBUGDECODE
  if(nevt<10){
    md->PrintInfo();
  }

  if(nevt%1000000==0){
    std::cout << std::dec << "nevt " << nevt << std::endl;
  }
  if(GetLabelType(label)==2 && nevt<1000){
    md->PrintInfo();
  }
#endif

  label = md->GetLabel();
  if((GetLabelType(label)==1 && !GetADCorTDCFlag(label)) || (GetLabelType(label)==2)){
    adc_qdc = md->GetADC_TDC_QDC();
    timestamp = md->GetTimestamp();

    tr->Fill();
  }

  return true;
}

//
void Decode::Process()
{
  benchmark->Start("decode");

  while(true){
    label = 0;
    adc_qdc = 0;
    timestamp = 0;

    if(!ProcessEntry()){
      break;
    }
  }

  file_out->cd();
  tr->Write();
  file_out->Close();

  benchmark->Show("decode");
}

//
//clover or csi or lips
//clover,lips -- 1
//csi         -- 2
//bgo         -- 3
//LaBr3       -- 4
//unkonwn     -- 5
int GetLabelType(int label)
{
  if((label&0x0fff) >= 0x780 && (label&0x0fff) <= 0x87f){
    return 2;
  }

  switch(label){
    case 0x040 :
    case 0x041 :
    case 0x042 :
    case 0x043 :
    case 0x050 :
    case 0x051 :
    case 0x052 :
    case 0x053 :

    case 0x048 :
    case 0x049 :
    case 0x04a :
    case 0x04b :
    case 0x058 :
    case 0x059 :
    case 0x05a :
    case 0x05b :

    case 0x060 :
    case 0x061 :
    case 0x062 :
    case 0x063 :
    case 0x070 :
    case 0x071 :
    case 0x072 :
    case 0x073 :

    case 0x068 :
    case 0x069 :
    case 0x06a :
    case 0x06b :
    case 0x078 :
    case 0x079 :
    case 0x07a :
    case 0x07b :

    case 0x080 :
    case 0x081 :
    case 0x082 :
    case 0x083 :
    case 0x090 :
    case 0x091 :
    case 0x092 :
    case 0x093 :

    case 0x088 :
    case 0x089 :
    case 0x08a :
    case 0x08b :
    case 0x098 :
    case 0x099 :
    case 0x09a :
    case 0x09b :

    case 0x0a0 :
    case 0x0a1 :
    case 0x0a2 :
    case 0x0a3 :
    case 0x0b0 :
    case 0x0b1 :
    case 0x0b2 :
    case 0x0b3 :

    case 0x0a8 :
    case 0x0a9 :
    case 0x0aa :
    case 0x0ab :
    case 0x0b8 :
    case 0x0b9 :
    case 0x0ba :
    case 0x0bb :

    case 0x0c0 :
    case 0x0c1 :
    case 0x0c2 :
    case 0x0c3 :
    case 0x0d0 :
    case 0x0d1 :
    case 0x0d2 :
    case 0x0d3 :

    case 0x0c8 :
    case 0x0c9 :
    case 0x0ca :
    case 0x0cb :
    case 0x0d8 :
    case 0x0d9 :
    case 0x0da :
    case 0x0db : return 1;//clovers
      break;

    case 0x140 :
    case 0x141 :
    case 0x142 :
    case 0x143 :
    case 0x144 :
    case 0x145 :
    case 0x150 :
    case 0x151 :
    case 0x152 :
    case 0x153 :
    case 0x154 :
    case 0x155 :
	
    case 0x148 :
    case 0x149 :
    case 0x14a :
    case 0x14b :
    case 0x14c :
    case 0x14d :
    case 0x158 :
    case 0x159 :
    case 0x15a :
    case 0x15b :
    case 0x15c :
    case 0x15d :

    case 0x180 :
    case 0x182 :
    case 0x183 :
    case 0x184 :
    case 0x185 :
    case 0x186 :
    case 0x190 :
    case 0x192 :
    case 0x193 :
    case 0x194 :
    case 0x195 :
    case 0x196 :

    case 0x188 :
    case 0x189 :
    case 0x18a :
    case 0x18b :
    case 0x18c :
    case 0x18d :
    case 0x198 :
    case 0x199 :
    case 0x19a :
    case 0x19b :
    case 0x19c :
    case 0x19d :

    case 0x1a0 :
    case 0x1a1 :
    case 0x1a2 :
    case 0x1a3 :
    case 0x1a5 :
    case 0x1a6 :
    case 0x1a7 :
    case 0x1b0 :
    case 0x1b1 :
    case 0x1b2 :
    case 0x1b3 :
    case 0x1b5 :
    case 0x1b6 :
    case 0x1b7 : return 3;//bgo

    case 0x146 :
    case 0x156 : return 4;//LaBr3

    case 0x14f :
    case 0x15f : return 5;//unkonwn

    default:
      return -1;
      break;
  }
}

//Get  adc or tdc
//0 -- adc
//1 -- tdc
bool GetADCorTDCFlag(int label)
{
  if(((label>>4)%2) == 0){
    return false;
  }
  return true;
}

//Get clover channel
int GetCloverChannel(int label)
{
  switch (label) {
    case 0x040 : return 0;
    case 0x041 : return 1;
    case 0x042 : return 2;
    case 0x043 : return 3;
      break;

    case 0x048 : return 4;
    case 0x049 : return 5;
    case 0x04a : return 6;
    case 0x04b : return 7;
      break;

    case 0x060 : return 8;
    case 0x061 : return 9;
    case 0x062 : return 10;
    case 0x063 : return 11;
      break;

    case 0x068 : return 12;
    case 0x069 : return 13;
    case 0x06a : return 14;
    case 0x06b : return 15;
      break;

    case 0x080 : return 16; 
    case 0x081 : return 17;
    case 0x082 : return 18;
    case 0x083 : return 19;
      break;

    case 0x088 : return 20;
    case 0x089 : return 21;
    case 0x08a : return 22;
    case 0x08b : return 23;
      break;

    case 0x0a0 : return 24;
    case 0x0a1 : return 25;
    case 0x0a2 : return 26;
    case 0x0a3 : return 27;//broken clover
      break;

    case 0x0a8 : return 28;
    case 0x0a9 : return 29;
    case 0x0aa : return 30;
    case 0x0ab : return 31;
      break;

    case 0x0c0 : return 32;
    case 0x0c1 : return 33;
    case 0x0c2 : return 34;
    case 0x0c3 : return 35;
      break;

    case 0x0c8 : return 36;
    case 0x0c9 : return 37;
    case 0x0ca : return 38;
    case 0x0cb : return 39;//lips clover
      break;

    default : 
      return -1;
      break;
  }
}

//Get BGO Channel
int GetBGOChannel(int label)
{
  switch(label){
    case 0x140 : return 100; 
    case 0x141 : return 101;
    case 0x142 : return 102;
    case 0x143 : return 103;
    case 0x144 : return 104;
    case 0x145 : return 105;
	
    case 0x148 : return 106;
    case 0x149 : return 107;
    case 0x14a : return 108;
    case 0x14b : return 109;
    case 0x14c : return 110;
    case 0x14d : return 111;

    case 0x180 : return 112;
    case 0x182 : return 113;
    case 0x183 : return 114;
    case 0x184 : return 115;
    case 0x185 : return 116;
    case 0x186 : return 117;

    case 0x188 : return 118;
    case 0x189 : return 119;
    case 0x18a : return 120;
    case 0x18b : return 121;
    case 0x18c : return 122;
    case 0x18d : return 123;

    case 0x1a0 : return 124;
    case 0x1a1 : return 125;
    case 0x1a2 : return 126;
    case 0x1a3 : return 127;
    case 0x1a5 : return 128;
    case 0x1a6 : return 129;
    case 0x1a7 : return 130;
    case 0x1b7 : return 131;
      break;

    default : 
      return -1;
      break;
  }
}

//Get LaBr3 Channel
int GetLaBr3Channel(int label)
{
  switch(label){
    case 0x146 : return 200;
      break;

    default :
      return -1;
      break;
  }
}

//Get CsI Channel
int GetCsIChannel(int label)
{
  switch(label) {
    case 0x780 :
    case 0x781 :
    case 0x782 :
    case 0x783 : return 300;
      break;

    case 0x784 :
    case 0x785 :
    case 0x786 :
    case 0x787 : return 301;
      break;

    case 0x788 :
    case 0x789 :
    case 0x78a :
    case 0x78b : return 302;
      break;

    case 0x78c :
    case 0x78d :
    case 0x78e :
    case 0x78f : return 303;
      break;

    case 0x790 :
    case 0x791 :
    case 0x792 :
    case 0x793 : return 304;
      break;

    case 0x794 :
    case 0x795 :
    case 0x796 :
    case 0x797 : return 305;
      break;

    case 0x798 :
    case 0x799 :
    case 0x79a :
    case 0x79b : return 306;
      break;

    case 0x79c :
    case 0x79d :
    case 0x79e :
    case 0x79f : return 307;
      break;

    case 0x7a0 :
    case 0x7a1 :
    case 0x7a2 :
    case 0x7a3 : return 308;
      break;

    case 0x7a4 :
    case 0x7a5 :
    case 0x7a6 :
    case 0x7a7 : return 309;
      break;

    case 0x7a8 :
    case 0x7a9 :
    case 0x7aa :
    case 0x7ab : return 310;
      break;

    case 0x7ac :
    case 0x7ad :
    case 0x7ae :
    case 0x7af : return 311;
      break;

    case 0x7b0 :
    case 0x7b1 :
    case 0x7b2 :
    case 0x7b3 : return 312;
      break;

    case 0x7b4 :
    case 0x7b5 :
    case 0x7b6 :
    case 0x7b7 : return 313;
      break;

    case 0x7b8 :
    case 0x7b9 :
    case 0x7ba :
    case 0x7bb : return 314;
      break;

    case 0x7bc :
    case 0x7bd :
    case 0x7be :
    case 0x7bf : return 315;
      break;

    case 0x7c0 :
    case 0x7c1 :
    case 0x7c2 :
    case 0x7c3 : return 316;
      break;

    case 0x7c4 :
    case 0x7c5 :
    case 0x7c6 :
    case 0x7c7 : return 317;
      break;

    case 0x7c8 :
    case 0x7c9 :
    case 0x7ca :
    case 0x7cb : return 318;
      break;

    case 0x7cc :
    case 0x7cd :
    case 0x7ce :
    case 0x7cf : return 319;
      break;

    case 0x7d0 :
    case 0x7d1 :
    case 0x7d2 :
    case 0x7d3 : return 320;
      break;

    case 0x7d4 :
    case 0x7d5 :
    case 0x7d6 :
    case 0x7d7 : return 321;
      break;

    case 0x7d8 :
    case 0x7d9 :
    case 0x7da :
    case 0x7db : return 322;
      break;

    case 0x7dc :
    case 0x7dd :
    case 0x7de :
    case 0x7df : return 323;
      break;

    case 0x7e0 :
    case 0x7e1 :
    case 0x7e2 :
    case 0x7e3 : return 324;
      break;

    case 0x7e4 :
    case 0x7e5 :
    case 0x7e6 :
    case 0x7e7 : return 325;
      break;

    case 0x7e8 :
    case 0x7e9 :
    case 0x7ea :
    case 0x7eb : return 326;
      break;

    case 0x7ec :
    case 0x7ed :
    case 0x7ee :
    case 0x7ef : return 327;
      break;

    case 0x7f0 :
    case 0x7f1 :
    case 0x7f2 :
    case 0x7f3 : return 328;
      break;

    case 0x7f4 :
    case 0x7f5 :
    case 0x7f6 :
    case 0x7f7 : return 329;
      break;

    case 0x7f8 :
    case 0x7f9 :
    case 0x7fa :
    case 0x7fb : return 330;
      break;

    case 0x7fc :
    case 0x7fd :
    case 0x7fe :
    case 0x7ff : return 331;
      break;

    case 0x800 :
    case 0x801 :
    case 0x802 :
    case 0x803 : return 332;
      break;

    case 0x804 :
    case 0x805 :
    case 0x806 :
    case 0x807 : return 333;
      break;

    case 0x808 :
    case 0x809 :
    case 0x80a :
    case 0x80b : return 334;
      break;

    case 0x80c :
    case 0x80d :
    case 0x80e :
    case 0x80f : return 335;
      break;

    case 0x810 :
    case 0x811 :
    case 0x812 :
    case 0x813 : return 336;
      break;

    case 0x814 :
    case 0x815 :
    case 0x816 :
    case 0x817 : return 337;
      break;

    case 0x818 :
    case 0x819 :
    case 0x81a :
    case 0x81b : return 338;
      break;

    case 0x81c :
    case 0x81d :
    case 0x81e :
    case 0x81f : return 339;
      break;

    case 0x820 :
    case 0x821 :
    case 0x822 :
    case 0x823 : return 340;
      break;

    case 0x824 :
    case 0x825 :
    case 0x826 :
    case 0x827 : return 341;
      break;

    case 0x828 :
    case 0x829 :
    case 0x82a :
    case 0x82b : return 342;
      break;

    case 0x82c :
    case 0x82d :
    case 0x82e :
    case 0x82f : return 343;
      break;

    case 0x830 :
    case 0x831 :
    case 0x832 :
    case 0x833 : return 344;
      break;

    case 0x834 :
    case 0x835 :
    case 0x836 :
    case 0x837 : return 345;
      break;

    case 0x838 :
    case 0x839 :
    case 0x83a :
    case 0x83b : return 346;
      break;

    case 0x83c :
    case 0x83d :
    case 0x83e :
    case 0x83f : return 347;
      break;

    case 0x840 :
    case 0x841 :
    case 0x842 :
    case 0x843 : return 348;
      break;

    case 0x844 :
    case 0x845 :
    case 0x846 :
    case 0x847 : return 349;
      break;

    case 0x848 :
    case 0x849 :
    case 0x84a :
    case 0x84b : return 350;
      break;

    case 0x84c :
    case 0x84d :
    case 0x84e :
    case 0x84f : return 351;
      break;

    case 0x850 :
    case 0x851 :
    case 0x852 :
    case 0x853 : return 352;
      break;

    case 0x854 :
    case 0x855 :
    case 0x856 :
    case 0x857 : return 353;
      break;

    case 0x858 :
    case 0x859 :
    case 0x85a :
    case 0x85b : return 354;
      break;

    case 0x85c :
    case 0x85d :
    case 0x85e :
    case 0x85f : return 355;
      break;

    case 0x860:
    case 0x861 :
    case 0x862 :
    case 0x863 : return 356;
      break;

    case 0x864 :
    case 0x865 :
    case 0x866 :
    case 0x867 : return 357;
      break;

    case 0x868 :
    case 0x869 :
    case 0x86a :
    case 0x86b : return 358;
      break;

    case 0x86c :
    case 0x86d :
    case 0x86e :
    case 0x86f : return 359;
      break;

    case 0x870 :
    case 0x871 :
    case 0x872 :
    case 0x873 : return 360;
      break;

    case 0x874 :
    case 0x875 :
    case 0x876 :
    case 0x877 : return 361;
      break;

    case 0x878 :
    case 0x879 :
    case 0x87a :
    case 0x87b : return 362;
      break;

    case 0x87c :
    case 0x87d :
    case 0x87e :
    case 0x87f : return 363;
      break;

    default : 
      return -1;
      break;
  }
}

//Get clover number
int GetCloverID(int label)
{
  return GetCsIChannel(label)/4;
}

//Get clover segement number
//0 -- a
//1 -- b
//2 -- c
//3 -- d
int GetSegementID(int label)
{
  int sege_id = (label&0x000F) % 4;
  return sege_id; 
}

//Get qdc number
//from 0 to 3
int GetQDCID(int label)
{
  return label%4;
}

