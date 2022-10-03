
void paper_draw_source_pd_qdc_pid()
{
  gROOT->SetStyle("ATLAS");

  // real code
  TFile *file_in = TFile::Open("../../../../../data/CsI/source/data_R0039/data_R0039_ana.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  TTree *tr = (TTree*)file_in->Get("tr");
  
  //
  TH2D *h = new TH2D("h", "", 2000,0,200,1000,0,100);
  tr->Draw("qdc_short/5.:qdc_long>>h","","col");
 
  TCanvas *cav = new TCanvas("cav", "", 0, 0, 520, 360);
  cav->SetLogz();
  cav->cd();

  h->GetXaxis()->SetLabelOffset(0.02);
  h->GetYaxis()->SetLabelOffset(0.02);

  h->GetXaxis()->SetTitleOffset(1.2);
  h->GetYaxis()->SetTitleOffset(1.2);

  h->GetXaxis()->SetTitle("Slow Component");
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitle("Fast Component");
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetXaxis()->SetRangeUser(0, 80);
  h->GetYaxis()->SetRangeUser(0, 30);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();

  h->Draw("col");

  TLatex *tex_p = new TLatex(34,5.8,"#gamma #rightarrow CsI");
  TLatex *tex_a = new TLatex(48,13,"#alpha #rightarrow CsI");
  TLatex *tex_g = new TLatex(50,22,"#gamma #rightarrow PD");
  
  tex_p->SetTextSize(0.06);
  tex_a->SetTextSize(0.06);
  tex_g->SetTextSize(0.06);

  tex_p->Draw();
  tex_a->Draw();
  tex_g->Draw();

  TLatex *tex = new TLatex(8,25,"(a)");
  tex->SetTextSize(0.06);
  tex->Draw();

  cav->SaveAs("source_pd_qdc_pid.pdf");

  //file_in->Close();
}
