TFile *file_in = TFile::Open("../../../../../data/CsI/source/data_R0039/data_R0039_ana_all_7.root");

void fom(TH1D *hy, double a, double b, double c, double d);

void rotate_qdc(double theta)
{
  TTree *tr = (TTree*)file_in->Get("tr");

  TH2D *hh1 = new TH2D("hh1", "hh1", 1000, 0, 100, 200, -60, 20); 

  TString str_draw;
  double sinn = TMath::Sin(-theta/180.*3.14159);
  double coss = TMath::Cos(-theta/180.*3.14159);
  str_draw = TString::Format("qdc_short*%f+qdc_long*%f:qdc_short*%f-qdc_long*%f>>hh1", sinn, coss, coss, sinn);

  TCanvas *cc1 = new TCanvas("cc1", "cc1");
  cc1->cd();
  tr->Draw(str_draw.Data(), "", "col");

  TH1D *hy1 = hh1->ProjectionY("hy1");
  TCanvas *cc2 = new TCanvas("cc2", "cc2");
  cc2->cd();
  hy1->Draw();

  //TH1D *h_new = new TH1D("h_new", "", 1000, 0, 1000);
  //tr->Draw("100.*qdc_long/qdc_short>>h_new");
  //TCanvas *cc3 = new TCanvas("cc3", "cc3");
  //cc3->cd();
  //h_new->Draw();

  fom(hy1, -3.5, 0.5, -12, -6);
}


void rotate_fit(double theta)
{
  TTree *tr = (TTree*)file_in->Get("tr");

  TH2D *hh2 = new TH2D("hh2", "hh2", 300, 0, 300, 240, -100, 20); 

  TString str_draw;
  double sinn = TMath::Sin(-theta/180.*3.14159);
  double coss = TMath::Cos(-theta/180.*3.14159);
  str_draw = TString::Format("am_fast*%f+am_slow*%f:am_fast*%f-am_slow*%f>>hh2", sinn, coss, coss, sinn);

  TCanvas *cc3 = new TCanvas("cc3", "cc3");
  cc3->cd();
  tr->Draw(str_draw.Data(), "", "col");

  TH1D *hy2 = hh2->ProjectionY("hy2");
  TCanvas *cc4 = new TCanvas("cc4", "cc4");
  cc4->cd();
  hy2->Draw();

  fom(hy2, -20, -6, -66, -33);
}

//
void fom(TH1D *hy, double a, double b, double c, double d)
{
  TF1 *tf2 = new TF1("tf2", "gaus", a, b);
  hy->Fit("tf2", "RN0");
  TF1 *tf3 = new TF1("tf3", "gaus", c, d);
  hy->Fit("tf3", "RN0");

  tf2->SetLineColor(2);
  tf3->SetLineColor(3);
  tf2->Draw("same");
  tf3->Draw("same");

  cout << "gamma " << tf2->GetParameter(1) << endl;
  cout << "alpha " << tf3->GetParameter(1) << endl;

  double fom = TMath::Abs((tf3->GetParameter(1)-tf2->GetParameter(1)))/(tf2->GetParameter(2)+tf3->GetParameter(2));
  cout << "fom " << fom << endl;
}
