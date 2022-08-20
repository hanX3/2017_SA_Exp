
void paper_draw_source_pd_qdc_pid()
{
  TStyle *han_style= new TStyle("han_style","");

  // use plain black on white colors
  han_style->SetCanvasBorderMode(0);
  han_style->SetCanvasColor(0);
  han_style->SetFrameBorderMode(0);
  han_style->SetFrameBorderSize(3);
  han_style->SetFrameLineStyle(1);
  han_style->SetFrameLineWidth(2);
  han_style->SetFrameLineColor(0);
  han_style->SetPadBorderMode(0);
  han_style->SetPadColor(0);
  han_style->SetStatColor(0);

  // set the paper & margin sizes
  han_style->SetPaperSize(20, 40);
  han_style->SetPadTopMargin(0.05);
  han_style->SetPadRightMargin(0.15);
  han_style->SetPadBottomMargin(0.15);
  han_style->SetPadLeftMargin(0.15);

  // do not display any of the standard histogram decorations
  han_style->SetOptTitle(0);
  han_style->SetOptStat(0);
  han_style->SetOptFit(0);

  // put tick marks on top and RHS of plots
  han_style->SetPadTickX(1);
  han_style->SetPadTickY(1);
  han_style->SetOptTitle(0);
  han_style->SetHistLineWidth(2);

  printf("Welcome to ROOT!\n");
  gROOT->SetStyle("han_style");
  gStyle->SetLineWidth(2);

  gStyle->SetOptLogz();
  gStyle->SetTextSize(0.08);
  gStyle->SetTextFont(22);

  //gROOT->SetBatch(1);

  // real code
  TFile *file_in = TFile::Open("../../../../../data/CsI/source/data_R0039/data_R0039_ana.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  TTree *tr = (TTree*)file_in->Get("tr");
  
  //
  TH2D *h = new TH2D("h", "", 2000,0,200,1000,0,100);
  tr->Draw("qdc_short/5.:qdc_long>>h","","colz");
 
  TCanvas *cav = new TCanvas("cav", "", 0, 0, 520, 360);
  cav->cd();

  h->GetXaxis()->SetTitle("Long QDC");
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitle("Short QDC");
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetXaxis()->SetRangeUser(1, 100);
  h->GetYaxis()->SetRangeUser(1, 40);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();

  h->Draw("colz");

  TLatex *tex_p = new TLatex(34,5.8,"#gamma + CsI");
  TLatex *tex_a = new TLatex(50,13,"#alpha + CsI");
  TLatex *tex_g = new TLatex(53,24,"#gamma + PD");

  tex_p->Draw();
  tex_a->Draw();
  tex_g->Draw();

  cav->SaveAs("source_pd_qdc_pid.pdf");

  //file_in->Close();
}
