#include "Assemble.h"

#include <iostream>

//
Assemble::Assemble(const std::string &filename_in, const std::string &filename_out)
{
  benchmark = new TBenchmark;
  nevt = 0;
  nevt_abandon = 0;

  file_in = TFile::Open(filename_in.c_str());
  tr_in = (TTree*)file_in->Get("tr");
  tr_in->SetBranchAddress("label", &label_in);
  tr_in->SetBranchAddress("adc_qdc", &adc_qdc_in);
  tr_in->SetBranchAddress("timestamp", &timestamp_in);

  total_nevt = tr_in->GetEntries();
  std::cout << "total_nevt " << total_nevt << std::endl;

  file_out = TFile::Open(filename_out.c_str(), "recreate");
  tr = new TTree("tr", "tmp rootfile 2");

  tr->Branch("label", &label, "label/s");
  tr->Branch("ch", &ch, "ch/s");
  tr->Branch("adc", &adc, "adc/s");
  tr->Branch("qdc", qdc, "qdc[4]/s");
  tr->Branch("timestamp", &timestamp, "timestamp/i");

  label = 0;
  ch = 0;
  adc = 0;
  for(int i=0;i<4;i++){
    qdc[i] = 0;
  }
  timestamp = 0;

  //
  for(int i=0;i<CLOVERCHANNELS;i++){
    tr_Clover[i] = new TTree(TString::Format("tr_Clover_ch%02d",i).Data(), "clover channel info");
  	tr_Clover[i]->Branch("ch", &ch, "ch/s");
  	tr_Clover[i]->Branch("adc", &adc, "adc/s");
  	tr_Clover[i]->Branch("timestamp", &timestamp, "timestamp/i");

	nevt_Clover[i] = 0;
  }

  for(int i=0;i<CSICHANNELS;i++){
    tr_CsI[i] = new TTree(TString::Format("tr_CsI_ch%02d",i).Data(), "csi channel info");
  	tr_CsI[i]->Branch("ch", &ch, "ch/s");
  	tr_CsI[i]->Branch("qdc", qdc, "qdc[4]/s");
  	tr_CsI[i]->Branch("timestamp", &timestamp, "timestamp/i");

	nevt_CsI[i] = 0;
  }
}

//
Assemble::~Assemble()
{
  delete benchmark;
}

//
bool Assemble::ProcessEntryClover()
{
#ifdef DEBUGASSEMBLE
  std::cout << "label " << label_in << std::endl;
  std::cout << "adc_qdc " << adc_qdc_in << std::endl;
  std::cout << "timestamp " << timestamp_in << std::endl;
#endif

  //clover, lips
  label = label_in;
  ch = GetCloverChannel(label_in);
  adc = adc_qdc_in;
  for(int i=0;i<4;i++)  qdc[i] = 0;
  timestamp = timestamp_in;

  return true;
}

//
bool Assemble::ProcessEntryCsI()
{
  UShort_t la[4];
  for(int i=0;i<4;i++){
    tr_in->GetEntry(nevt+i);
    la[i] = label_in;
    qdc[i] = adc_qdc_in;
  }

  for(int i=0;i<4;i++){
    if(GetLabelType(la[i])!=2){
      nevt += i;
      nevt_abandon++;
#ifdef DEBUGASSEMBLE
	  std::cout << "1. nevt " << nevt << std::endl;
#endif
      return false;
    }
  }

  UShort_t c = GetCsIChannel(la[0]);
  for(int i=1;i<4;i++){
    if(GetCsIChannel(la[i]) != c){
      nevt += 1;
      nevt_abandon++;
#ifdef DEBUGASSEMBLE
	  std::cout << "2. nevt " << nevt << std::endl;
#endif
      return false;
    }
  }

  for(int i=0;i<4;i++){
    if(GetQDCID(la[i]) != i){
      nevt += 1;
      nevt_abandon++;
#ifdef DEBUGASSEMBLE
	  std::cout << "3. nevt " << nevt << std::endl;
#endif
      return false;
    }
  }

  label = la[0];
  ch = GetCsIChannel(label);
  adc = qdc[0];
  timestamp = timestamp_in;

  return true;
}

//
void Assemble::Process()
{
  benchmark->Start("assemble"); 

  while(true){
    if(nevt >= total_nevt) break;

    if(nevt%100000==0){
      std::cout << "nevt " << nevt << std::endl;
    }

    tr_in->GetEntry(nevt);
    if(GetLabelType(label_in)==1){
      if(ProcessEntryClover()){
  		tr->Fill();
  		tr_Clover[ch]->Fill();
		nevt_Clover[ch]++;

		nevt ++;
      }
    }
    if(GetLabelType(label_in)==2){
      if(ProcessEntryCsI()){
        tr->Fill();
		tr_CsI[ch-300]->Fill();
		nevt_CsI[ch-300]++;

		nevt += 4;
      }
    }
  }
  
  file_out->cd();
  tr->Write();
  for(int i=0;i<CLOVERCHANNELS;i++){
  	if(nevt_Clover[i]>0){
	  tr_Clover[i]->Write();
	}
  }

  for(int i=0;i<CSICHANNELS;i++){
  	if(nevt_CsI[i]>0){
	  tr_CsI[i]->Write();
	}
  }
  file_out->Close();

  file_in->Close();

  std::cout << "nevt " << nevt << std::endl;
  std::cout << "nevt_abandon " << nevt_abandon << std::endl;
  benchmark->Show("assemble"); 
}

