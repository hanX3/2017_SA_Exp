
//
void fom(TH1D *hy, double a, double b, double c, double d);

//
TCanvas *cc1 = new TCanvas("cc1", "cc1");
TCanvas *cc2 = new TCanvas("cc2", "cc2");
void rotate_qdc(int q1_stop, double theta)
{
  TFile *file_in = TFile::Open(TString::Format("./run_R0040_qdc_%d.root", q1_stop).Data());

  TTree *tr = (TTree*)file_in->Get("tr");

  TH2D *hh1 = new TH2D("hh1", "hh1", 1000, 0, 1000, 400, -200, 200); 

  TString str_draw;
  double sinn = TMath::Sin(-theta/180.*3.14159);
  double coss = TMath::Cos(-theta/180.*3.14159);
  str_draw = TString::Format("qdc_short*%f+qdc_long*%f:qdc_short*%f-qdc_long*%f>>hh1", sinn, coss, coss, sinn);

  cc1->cd();
  // tr->Draw(str_draw.Data(), "(qdc_long>180&&qdc_long<280) || (qdc_long>400&&qdc_long<600)", "col");
  tr->Draw(str_draw.Data(), "", "col");

  TH1D *hy1 = hh1->ProjectionY("hy1");
  cc2->cd();
  hy1->Draw();
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
