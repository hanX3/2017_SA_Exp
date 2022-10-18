
#define THETA_QDC 66.
#define THETA_FIT 40.


TFile *file_in = TFile::Open("../../../../../data/CsI/source/run_R0040/run_R0040_ana_all.root");
//
void get_qdc_fom_data()
{
  TTree *tr = (TTree*)file_in->Get("tr");

  TH2D *hh1 = new TH2D("hh1", "hh1", 1000, 0, 1000, 200, -200, 0); 

  TString str_draw;
  double sinn = TMath::Sin(-THETA_QDC/180.*3.14159);
  double coss = TMath::Cos(-THETA_QDC/180.*3.14159);
  str_draw = TString::Format("qdc_short*%f+qdc_long*%f:qdc_short*%f-qdc_long*%f>>hh1", sinn, coss, coss, sinn);

  TCanvas *cc1 = new TCanvas("cc1", "cc1");
  cc1->cd();
  tr->Draw(str_draw.Data(), "", "col");

  TH1D *hy1 = hh1->ProjectionY("hy1");
  TCanvas *cc2 = new TCanvas("cc2", "cc2");
  cc2->cd();
  hy1->Draw();

  ofstream file_out;
  file_out.open(TString::Format("fom_qdc.txt"));

  for(int i=0;i<hy1->GetNbinsX();i++){
    file_out << i << "  " << hy1->GetBinContent(i)+200. << endl;
  }

  file_out.close();
}


void get_fit_fom_data()
{
  TTree *tr = (TTree*)file_in->Get("tr");

  TH2D *hh2 = new TH2D("hh2", "hh2", 1000, 0, 1000, 300, -800, 100); 

  TString str_draw;
  double sinn = TMath::Sin(-THETA_FIT/180.*3.14159);
  double coss = TMath::Cos(-THETA_FIT/180.*3.14159);
  str_draw = TString::Format("am_fast*%f+am_slow*%f:am_fast*%f-am_slow*%f>>hh2", sinn, coss, coss, sinn);

  TCanvas *cc3 = new TCanvas("cc3", "cc3");
  cc3->cd();
  tr->Draw(str_draw.Data(), "", "col");

  TH1D *hy2 = hh2->ProjectionY("hy2");
  TCanvas *cc4 = new TCanvas("cc4", "cc4");
  cc4->cd();
  hy2->Draw();

  ofstream file_out;
  file_out.open(TString::Format("fom_fit.txt"));

  for(int i=0;i<hy2->GetNbinsX();i++){
    file_out << i << "  " << hy2->GetBinContent(i)+100. << endl;
  }

  file_out.close();
}