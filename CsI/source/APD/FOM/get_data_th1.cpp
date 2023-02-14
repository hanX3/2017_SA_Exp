
//
TCanvas *cc1 = new TCanvas("cc1", "cc1");
TCanvas *cc2 = new TCanvas("cc2", "cc2");
void get_data_th1(int q1_stop, double theta)
{
  TFile *file_in = TFile::Open(TString::Format("./run_R0040_qdc_%d.root", q1_stop).Data());

  TTree *tr = (TTree*)file_in->Get("tr");

  TH2D *hh1 = new TH2D("hh1", "hh1", 1000, 0, 1000, 400, -200, 200); 

  TString str_draw;
  double sinn = TMath::Sin(-theta/180.*3.14159);
  double coss = TMath::Cos(-theta/180.*3.14159);
  str_draw = TString::Format("qdc_short*%f+qdc_long*%f:qdc_short*%f-qdc_long*%f>>hh1", sinn, coss, coss, sinn);

  cc1->cd();
  tr->Draw(str_draw.Data(), "(qdc_long>180&&qdc_long<280) || (qdc_long>400&&qdc_long<600)", "col");
  // tr->Draw(str_draw.Data(), "", "col");

  TH1D *hy1 = hh1->ProjectionY("hy1");
  cc2->cd();
  hy1->Draw();

  ofstream fo;
  fo.open(TString::Format("qdc_%d.txt", q1_stop));

  for(int i=0;i<hy1->GetNbinsX();i++){
    fo << i << " " << hy1->GetBinContent(i) << endl;
  }

  fo.close();
}

