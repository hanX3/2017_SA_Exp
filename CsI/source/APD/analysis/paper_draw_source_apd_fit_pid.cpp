
void paper_draw_source_apd_fit_pid()
{
  gROOT->SetStyle("ATLAS");

  // real code
  TFile *file_in = TFile::Open("../../../../../data/CsI/source/run_R0040/run_R0040_ana_all.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  TTree *tr = (TTree*)file_in->Get("tr");
  
  //
  TH2D *h = new TH2D("h", "", 1000,0,1000,800,0,800);
  tr->Draw("1.8*am_slow:0.9*am_fast>>h","","col");
 
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
  h->GetXaxis()->SetRangeUser(0, 1000);
  h->GetYaxis()->SetRangeUser(0, 800);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();

  h->Draw("col");

  TLatex *tex_g = new TLatex(423,495,"#gamma #rightarrow CsI");
  TLatex *tex_a = new TLatex(472,240,"#alpha #rightarrow CsI");
  
  tex_g->SetTextSize(0.06);
  tex_a->SetTextSize(0.06);

  tex_g->Draw();
  tex_a->Draw();

  TLatex *tex = new TLatex(66.6,666.6,"(d)");
  tex->SetTextSize(0.06);
  tex->Draw();

  cav->SaveAs("source_apd_fit_pid.pdf");

  //file_in->Close();
}
