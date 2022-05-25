#include "WaveAnalysis.h"

//
WaveAnalysis::WaveAnalysis(const std::string &filename_in, const std::string &filename_out)
{
  file_in = TFile::Open(filename_in.c_str());
  if(file_in->IsZombie()){
    std::cout << "can not open " << filename_in << std::endl;
  }
  tr_in = (TTree*)file_in->Get("tree");
  tr_in->SetBranchAddress("ltra", &ltra);
  tr_in->SetBranchAddress("data", data);

  memset(&cfd_par, 0, sizeof(CFD_PAR));

  cfd_par.pre_trigger = CFDPARPRITRIGGER;
  cfd_par.smooth_length = CFDPARSMOOTHLENGTH;
  cfd_par.rise_length = CFDPARRISELENGTH;
  cfd_par.threshold = CFDPARTHRESHOLD;
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

  return true;
}

//
int WaveAnalysis::CFD(Long64_t n)
{

  return 0;
}

//
int WaveAnalysis::RCCR2(Long64_t n)
{
  GetWave(n);



}

//
void WaveAnalysis::DrawDraw(Long64_t n)
{
  GetWave(n);

  TCanvas *c1 = new TCanvas("c1", "", 0, 0, 480, 360);
  TH1D *h1 = new TH1D("h1", "h1", ltra, 0, ltra);
  for(int i=0;i<ltra;i++){
    h1->SetBinContent(i+1, (Double_t)data[i]);
  }

  c1->cd();
  h1->Draw();
}
