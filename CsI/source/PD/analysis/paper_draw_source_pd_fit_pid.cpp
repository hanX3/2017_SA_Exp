
void paper_draw_source_pd_fit_pid()
{
  gROOT->SetStyle("ATLAS");

  // real code
  TFile *file_in = TFile::Open("../../../../../data/CsI/source/data_R0039/data_R0039_ana_all_7.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  TTree *tr = (TTree*)file_in->Get("tr");
  
  double theta = 25.;

  double sinn = TMath::Sin(-theta/180.*3.14159);
  double coss = TMath::Cos(-theta/180.*3.14159);
  TString str_draw = TString::Format("am_fast*%f+am_slow*%f+20.:am_fast*%f-am_slow*%f-10.>>h", sinn, coss, coss, sinn);

  //
  TH2D *h = new TH2D("h", "", 900,0,300,600,0,200);
  // tr->Draw(str_draw.Data(),"am_fast>0.","col");
  tr->Draw(str_draw.Data(),"am_fast>0.","col",200000);
 
  TCanvas *cav = new TCanvas("cav", "", 0, 0, 520, 360);
  cav->SetLogz();
  cav->cd();

  h->GetXaxis()->SetLabelOffset(0.02);
  h->GetYaxis()->SetLabelOffset(0.02);

  h->GetXaxis()->SetTitleOffset(1.2);
  h->GetYaxis()->SetTitleOffset(1.2);

  h->GetXaxis()->SetTitle("Fast Amplitude");
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitle("Slow Amplitude");
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetXaxis()->SetRangeUser(0, 150);
  h->GetYaxis()->SetRangeUser(0, 120);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();

  h->SetMinimum(2);
  h->Draw("col");

  TLatex *tex_p = new TLatex(28, 82, "#gamma #rightarrow CsI");
  TLatex *tex_a = new TLatex(42, 45, "#alpha #rightarrow CsI");
  TLatex *tex_g = new TLatex(90, 25, "#gamma #rightarrow PD");

  tex_p->SetTextSize(0.06);
  tex_a->SetTextSize(0.06);
  tex_g->SetTextSize(0.06);

  tex_p->Draw();
  tex_a->Draw();
  tex_g->Draw();

  TLatex *tex = new TLatex(10,100,"(b)");
  tex->SetTextSize(0.06);
  tex->Draw();

  cav->SaveAs("source_pd_fit_pid.pdf");

  // file_in->Close();
}
