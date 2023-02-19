
void paper_draw_source_apd_qdc_pid()
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
  gStyle->SetTextSize(0.1);
  // gStyle->SetTextFont(22);

  //gROOT->SetBatch(1);

  // real code
  TFile *file_in = TFile::Open("../../../../../data/CsI/source/run_R0040/run_R0040_ana_all.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  TTree *tr = (TTree*)file_in->Get("tr");
  
  //
  TH2D *h = new TH2D("h", "", 500,0,500,800,0,800);
  tr->Draw("qdc_long:qdc_short>>h","","col");
  // tr->Draw("qdc_long:qdc_short>>h","","col",200000);
 
  TCanvas *cav = new TCanvas("cav", "", 0, 0, 520, 360);
  cav->SetLogz();
  cav->cd();

  h->GetXaxis()->SetLabelOffset(0.02);
  h->GetYaxis()->SetLabelOffset(0.02);

  h->GetXaxis()->SetTitleOffset(1.2);
  h->GetYaxis()->SetTitleOffset(1.2);

  h->GetXaxis()->SetLabelSize(0.06);
  h->GetYaxis()->SetLabelSize(0.06);

  h->GetXaxis()->SetTitle("Short Gate");
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitle("Long Gate");
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetXaxis()->SetRangeUser(0, 500);
  h->GetYaxis()->SetRangeUser(0, 800);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();


  h->SetMinimum(2);
  h->Draw("col");

  TLatex *tex_g = new TLatex(156,429,"#gamma #rightarrow CsI");
  TLatex *tex_a = new TLatex(357,432,"#alpha #rightarrow CsI");

  tex_a->SetTextSize(0.08);
  tex_g->SetTextSize(0.08);

  tex_g->Draw();
  tex_a->Draw();

  TLatex *tex = new TLatex(36,679,"(c)");
  tex->SetTextSize(0.08);
  tex->Draw();
 

  cav->SaveAs("source_apd_qdc_pid.pdf");

  //file_in->Close();
}
