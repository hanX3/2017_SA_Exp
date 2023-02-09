#include "WaveAnalysis.h"

//
WaveAnalysis::WaveAnalysis(const std::string &filename_in, const std::string &filename_out, int a, int b)
{
  benchmark = new TBenchmark;

  file_in = TFile::Open(filename_in.c_str());
  if(file_in->IsZombie()){
    std::cout << "can not open " << filename_in << std::endl;
  }
  tr_in = (TTree*)file_in->Get("tree");
  tr_in->SetBranchAddress("size", &ltra);
  tr_in->SetBranchAddress("data", data);
  total_entry = tr_in->GetEntries();

  memset(data, 0, MAXLENGTH*sizeof(UShort_t));
  memset(data_bl, 0, MAXLENGTH*sizeof(Double_t));

  //PID
  memset(&qdc_par, 0, sizeof(QDC_PAR));
  qdc_par.q1_start = a;
  qdc_par.q1_stop = b;
  qdc_par.q2_start = Q2START;
  qdc_par.q2_stop = Q2STOP;

  file_out = new TFile(filename_out.c_str(), "recreate");
  tr_out = new TTree("tr", "wave analysis info");

  TString leaf = TString::Format("qdc_short/D:qdc_long/D");
  tr_out->Branch("qdc_result", &qdc_result, leaf.Data());
}

//
WaveAnalysis::~WaveAnalysis()
{

}

//
bool WaveAnalysis::GetWave(Long64_t n)
{
  ltra = 0;
  memset(data, 0, sizeof(UShort_t)*MAXLENGTH);
  tr_in->GetEntry(n);
  for(UInt_t i=0;i<ltra;i++){
    data_pol[i] = data[i]*POLARITY;
  }

  return true;
}

//
void WaveAnalysis::Process()
{
  benchmark->Start("analysis");

  for(Long64_t i=0;i<total_entry;i++){
    if(i%10000==0){
    std::cout << i << "/" << total_entry << std::endl;
  }

    ProcessEntry(i);
    tr_out->Fill();
  }

  file_out->cd();
  tr_out->Write();
  file_out->Close();

  file_in->Close();

  benchmark->Show("analysis");
}

//
bool WaveAnalysis::ProcessEntry(Long64_t n)
{
  GetWave(n);

  baseline = 0.;
  for(UInt_t i=0;i<PRETRIGGER;i++){
    baseline += data_pol[i];
  }
  baseline /= (Double_t)PRETRIGGER;

  for(UShort_t i=0;i<ltra;i++){
    data_bl[i] = data_pol[i]-baseline; 
  }

  //PID
  CaliQDC(n);   

  return true;
}

//
void WaveAnalysis::CaliQDC(Long64_t n)
{
  memset(&qdc_result, 0, sizeof(QDC_RESULT));
  
  qdc_result.qdc_short = Integral(data_bl, qdc_par.q1_start, qdc_par.q1_stop);
  qdc_result.qdc_long = Integral(data_bl, qdc_par.q2_start, qdc_par.q2_stop);
}

//
Double_t Integral(Double_t *v, UInt_t i, UInt_t j)
{
  Double_t s = 0.;
  for(UInt_t k=i;k<j;k++){
    s += v[k];
  }

  s /= (Double_t)(j-i);

  return s;
}
