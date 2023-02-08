
void paper_draw_source_pd_qdc_pid()
{
  gROOT->SetStyle("ATLAS");

  // real code
  TFile *file_in = TFile::Open("../../../../../data/CsI/source/data_R0039/data_R0039_ana_all_7.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  TTree *tr = (TTree*)file_in->Get("tr");
  
  //
  TH2D *h = new TH2D("h", "", 1000,0,100,2000,0,200);
  tr->Draw("qdc_long:qdc_short/5>>h","","col");
 
  TCanvas *cav = new TCanvas("cav", "", 0, 0, 520, 360);
  cav->SetLogz();
  cav->cd();

  h->GetXaxis()->SetLabelOffset(0.02);
  h->GetYaxis()->SetLabelOffset(0.02);

  h->GetXaxis()->SetTitleOffset(1.2);
  h->GetYaxis()->SetTitleOffset(1.2);

  h->GetXaxis()->SetTitle("Short Gate");
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitle("Long Gate");
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetXaxis()->SetRangeUser(0, 30);
  h->GetYaxis()->SetRangeUser(0, 80);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();

  h->Draw("col");

  TLatex *tex_p = new TLatex(2.4,34,"#gamma #rightarrow CsI");
  TLatex *tex_a = new TLatex(11,48,"#alpha #rightarrow CsI");
  TLatex *tex_g = new TLatex(20,48,"#gamma #rightarrow PD");
  
  tex_p->SetTextSize(0.06);
  tex_a->SetTextSize(0.06);
  tex_g->SetTextSize(0.06);

  tex_p->Draw();
  tex_a->Draw();
  tex_g->Draw();

  TLatex *tex = new TLatex(2,68,"(a)");
  tex->SetTextSize(0.06);
  tex->Draw();

  cav->SaveAs("source_pd_qdc_pid.pdf");

  //file_in->Close();
}
