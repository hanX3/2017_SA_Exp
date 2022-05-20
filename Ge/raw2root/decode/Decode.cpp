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
    std::cout << "end of raw file" << std::endl;
    return false;
  }

  nevt++;
  if(nevt<10){
    md->PrintInfo();
  }
  if(nevt%1000000==0){
    std::cout << std::dec << "nevt " << nevt << std::endl;
  }
#ifdef DEBUGDECODE
  if(GetLabelType(label)==2 && nevt<1000){
  	md->PrintInfo();
  }
#endif

  label = md->GetLabel();
  if((GetLabelType(label)==1||GetLabelType(label)==3||GetLabelType(label)==4) && GetADCorTDCFlag(label)){
  	return true;
  }
  adc_qdc = md->GetADC_TDC_QDC();
  timestamp = md->GetTimestamp();

  tr->Fill();

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

//Get clover number
int GetCloverID(int label)
{
  switch (label) {
    case 0x040 :
    case 0x041 :
    case 0x042 :
    case 0x043 :
    case 0x050 :
    case 0x051 :
    case 0x052 :
    case 0x053 : return 0;
      break;

    case 0x048 :
    case 0x049 :
    case 0x04a :
    case 0x04b :
    case 0x058 :
    case 0x059 :
    case 0x05a :
    case 0x05b : return 1;
      break;

    case 0x060 :
    case 0x061 :
    case 0x062 :
    case 0x063 :
    case 0x070 :
    case 0x071 :
    case 0x072 :
    case 0x073 : return 2;
      break;

    case 0x068 :
    case 0x069 :
    case 0x06a :
    case 0x06b :
    case 0x078 :
    case 0x079 :
    case 0x07a :
    case 0x07b : return 3;
      break;

    case 0x080 :
    case 0x081 :
    case 0x082 :
    case 0x083 :
    case 0x090 :
    case 0x091 :
    case 0x092 :
    case 0x093 : return 4;
      break;

    case 0x088 :
    case 0x089 :
    case 0x08a :
    case 0x08b :
    case 0x098 :
    case 0x099 :
    case 0x09a :
    case 0x09b : return 5;
      break;

    case 0x0a0 :
    case 0x0a1 :
    case 0x0a2 :
    case 0x0a3 :
    case 0x0b0 :
    case 0x0b1 :
    case 0x0b2 :
    case 0x0b3 : return 6; //this is a broken clover...
      break;

    case 0x0a8 :
    case 0x0a9 :
    case 0x0aa :
    case 0x0ab :
    case 0x0b8 :
    case 0x0b9 :
    case 0x0ba :
    case 0x0bb : return 7;
      break;

    case 0x0c0 :
    case 0x0c1 :
    case 0x0c2 :
    case 0x0c3 :
    case 0x0d0 :
    case 0x0d1 :
    case 0x0d2 :
    case 0x0d3 : return 8;
      break;

    case 0x0c8 :
    case 0x0c9 :
    case 0x0ca :
    case 0x0cb :
    case 0x0d8 :
    case 0x0d9 :
    case 0x0da :
    case 0x0db : return 9;//lips
      break;

    default : 
      return -1;
      break;
  }
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

//Get CsI number
//from 0 to 63
int GetCsIID(int label)
{
  switch(label) {
    case 0x780 :
    case 0x781 :
    case 0x782 :
    case 0x783 : return 0;
      break;

    case 0x784 :
    case 0x785 :
    case 0x786 :
    case 0x787 : return 1;
      break;

    case 0x788 :
    case 0x789 :
    case 0x78a :
    case 0x78b : return 2;
      break;

    case 0x78c :
    case 0x78d :
    case 0x78e :
    case 0x78f : return 3;
      break;

    case 0x790 :
    case 0x791 :
    case 0x792 :
    case 0x793 : return 4;
      break;

    case 0x794 :
    case 0x795 :
    case 0x796 :
    case 0x797 : return 5;
      break;

    case 0x798 :
    case 0x799 :
    case 0x79a :
    case 0x79b : return 6;
      break;

    case 0x79c :
    case 0x79d :
    case 0x79e :
    case 0x79f : return 7;
      break;

    case 0x7a0 :
    case 0x7a1 :
    case 0x7a2 :
    case 0x7a3 : return 8;
      break;

    case 0x7a4 :
    case 0x7a5 :
    case 0x7a6 :
    case 0x7a7 : return 9;
      break;

    case 0x7a8 :
    case 0x7a9 :
    case 0x7aa :
    case 0x7ab : return 10;
      break;

    case 0x7ac :
    case 0x7ad :
    case 0x7ae :
    case 0x7af : return 11;
      break;

    case 0x7b0 :
    case 0x7b1 :
    case 0x7b2 :
    case 0x7b3 : return 12;
      break;

    case 0x7b4 :
    case 0x7b5 :
    case 0x7b6 :
    case 0x7b7 : return 13;
      break;

    case 0x7b8 :
    case 0x7b9 :
    case 0x7ba :
    case 0x7bb : return 14;
      break;

    case 0x7bc :
    case 0x7bd :
    case 0x7be :
    case 0x7bf : return 15;
      break;

    case 0x7c0 :
    case 0x7c1 :
    case 0x7c2 :
    case 0x7c3 : return 16;
      break;

    case 0x7c4 :
    case 0x7c5 :
    case 0x7c6 :
    case 0x7c7 : return 17;
      break;

    case 0x7c8 :
    case 0x7c9 :
    case 0x7ca :
    case 0x7cb : return 18;
      break;

    case 0x7cc :
    case 0x7cd :
    case 0x7ce :
    case 0x7cf : return 19;
      break;

    case 0x7d0 :
    case 0x7d1 :
    case 0x7d2 :
    case 0x7d3 : return 20;
      break;

    case 0x7d4 :
    case 0x7d5 :
    case 0x7d6 :
    case 0x7d7 : return 21;
      break;

    case 0x7d8 :
    case 0x7d9 :
    case 0x7da :
    case 0x7db : return 22;
      break;

    case 0x7dc :
    case 0x7dd :
    case 0x7de :
    case 0x7df : return 23;
      break;

    case 0x7e0 :
    case 0x7e1 :
    case 0x7e2 :
    case 0x7e3 : return 24;
      break;

    case 0x7e4 :
    case 0x7e5 :
    case 0x7e6 :
    case 0x7e7 : return 25;
      break;

    case 0x7e8 :
    case 0x7e9 :
    case 0x7ea :
    case 0x7eb : return 26;
      break;

    case 0x7ec :
    case 0x7ed :
    case 0x7ee :
    case 0x7ef : return 27;
      break;

    case 0x7f0 :
    case 0x7f1 :
    case 0x7f2 :
    case 0x7f3 : return 28;
      break;

    case 0x7f4 :
    case 0x7f5 :
    case 0x7f6 :
    case 0x7f7 : return 29;
      break;

    case 0x7f8 :
    case 0x7f9 :
    case 0x7fa :
    case 0x7fb : return 30;
      break;

    case 0x7fc :
    case 0x7fd :
    case 0x7fe :
    case 0x7ff : return 31;
      break;

    case 0x800 :
    case 0x801 :
    case 0x802 :
    case 0x803 : return 32;
      break;

    case 0x804 :
    case 0x805 :
    case 0x806 :
    case 0x807 : return 33;
      break;

    case 0x808 :
    case 0x809 :
    case 0x80a :
    case 0x80b : return 34;
      break;

    case 0x80c :
    case 0x80d :
    case 0x80e :
    case 0x80f : return 35;
      break;

    case 0x810 :
    case 0x811 :
    case 0x812 :
    case 0x813 : return 36;
      break;

    case 0x814 :
    case 0x815 :
    case 0x816 :
    case 0x817 : return 37;
      break;

    case 0x818 :
    case 0x819 :
    case 0x81a :
    case 0x81b : return 38;
      break;

    case 0x81c :
    case 0x81d :
    case 0x81e :
    case 0x81f : return 39;
      break;

    case 0x820 :
    case 0x821 :
    case 0x822 :
    case 0x823 : return 40;
      break;

    case 0x824 :
    case 0x825 :
    case 0x826 :
    case 0x827 : return 41;
      break;

    case 0x828 :
    case 0x829 :
    case 0x82a :
    case 0x82b : return 42;
      break;

    case 0x82c :
    case 0x82d :
    case 0x82e :
    case 0x82f : return 43;
      break;

    case 0x830 :
    case 0x831 :
    case 0x832 :
    case 0x833 : return 44;
      break;

    case 0x834 :
    case 0x835 :
    case 0x836 :
    case 0x837 : return 45;
      break;

    case 0x838 :
    case 0x839 :
    case 0x83a :
    case 0x83b : return 46;
      break;

    case 0x83c :
    case 0x83d :
    case 0x83e :
    case 0x83f : return 47;
      break;

    case 0x840 :
    case 0x841 :
    case 0x842 :
    case 0x843 : return 48;
      break;

    case 0x844 :
    case 0x845 :
    case 0x846 :
    case 0x847 : return 49;
      break;

    case 0x848 :
    case 0x849 :
    case 0x84a :
    case 0x84b : return 50;
      break;

    case 0x84c :
    case 0x84d :
    case 0x84e :
    case 0x84f : return 51;
      break;

    case 0x850 :
    case 0x851 :
    case 0x852 :
    case 0x853 : return 52;
      break;

    case 0x854 :
    case 0x855 :
    case 0x856 :
    case 0x857 : return 53;
      break;

    case 0x858 :
    case 0x859 :
    case 0x85a :
    case 0x85b : return 54;
      break;

    case 0x85c :
    case 0x85d :
    case 0x85e :
    case 0x85f : return 55;
      break;

    case 0x860:
    case 0x861 :
    case 0x862 :
    case 0x863 : return 56;
      break;

    case 0x864 :
    case 0x865 :
    case 0x866 :
    case 0x867 : return 57;
      break;

    case 0x868 :
    case 0x869 :
    case 0x86a :
    case 0x86b : return 58;
      break;

    case 0x86c :
    case 0x86d :
    case 0x86e :
    case 0x86f : return 59;
      break;

    case 0x870 :
    case 0x871 :
    case 0x872 :
    case 0x873 : return 60;
      break;

    case 0x874 :
    case 0x875 :
    case 0x876 :
    case 0x877 : return 61;
      break;

    case 0x878 :
    case 0x879 :
    case 0x87a :
    case 0x87b : return 62;
      break;

    case 0x87c :
    case 0x87d :
    case 0x87e :
    case 0x87f : return 63;
      break;

    default : 
      return -1;
      break;
  }
}

//Get qdc number
//from 0 to 3
int GetQDCID(int label)
{
  return label%4;
}

