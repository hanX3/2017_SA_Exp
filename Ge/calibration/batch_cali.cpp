#include "set.h"
#include "auto_cali.cpp"

//if want to check channel by channel
//commant gROOT->SetBatch(1) and umcommant c->WaitPrimitive()

void batch_cali()
{
  gROOT->SetBatch(1);

  TFile *file_in = TFile::Open("../../../data/Ge/root/R0032_0000_decode_2.root");
  if(file_in->IsZombie()){
    cout << "cannot open rootfile." << endl;
	return ;
  }

  TTree *tr[CLOVERCHANNELS];
  TH1D *h[CLOVERCHANNELS];
  TCanvas *c = new TCanvas("c");
  TString str_draw[CLOVERCHANNELS];
  Float_t par[4];

  ofstream file_out("clover_cali.dat");
  TString str_out;



  for(int i=0;i<CLOVERCHANNELS;i++){
    for(int j=0;j<4;j++) par[j]=0.;
    tr[i] = (TTree*)file_in->Get(TString::Format("tr_Clover_ch%02d",i));
    if(!tr[i]){
      cout << "no that tree" << endl;
      str_out = Form("%-d\t%-10.6f\t%-10.6f\t%-12.9f\t%-g", i, par[0], par[1], par[2], par[3]);
      cout << str_out << endl;
      file_out << str_out << endl;
       continue;
    }

    c->cd();
    h[i] = new TH1D(TString::Format("h%02d",i).Data(),TString::Format("c%02d",i).Data(),8192,0,8192);
    str_draw[i] = TString::Format("adc>>h%02d",i);
    tr[i]->Draw(str_draw[i].Data());
	
    auto_cali(h[i], par);
    str_out = Form("%-d\t%-10.6f\t%-10.6f\t%-12.9f\t%-g", i, par[0], par[1], par[2], par[3]);
    cout << str_out << endl;
    file_out << str_out << endl;
    //c->WaitPrimitive();
  }

  file_out.close();
  file_in->Close();
}
