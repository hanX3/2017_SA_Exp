
void paper_draw_qdc_pid_projection()
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
  TFile *file_in = TFile::Open("draw_th1_all_230216.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  Double_t xx, yy;
  TTree *tr = (TTree*)file_in->Get("tr_csi_16");
  tr->Print();
  tr->SetBranchAddress("xx", &xx);
  tr->SetBranchAddress("yy", &yy);

  TH2D *h = new TH2D("h", "h", 600, 0, 10, 1000, 0, 500);
  tr->Draw("xx/36.:xx/36/(yy/14.-16.75)/1.6>>h", "yy>180&&(xx/(yy+20.)>3.)&&(xx/(yy+100)<20.)", "colz");

  TCanvas *cav = new TCanvas("cav", "", 0, 0, 520, 360);
  cav->cd();

  h->GetXaxis()->SetLabelSize(0.06);
  h->GetYaxis()->SetLabelSize(0.06);

  h->SetMinimum(2);
  h->GetXaxis()->SetTitle("PID");
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitle("Q_{long} [a.u.]");
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetRangeUser(10, 500);
  h->GetXaxis()->SetRangeUser(0.5, 3.5);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();

  h->Draw("col");

  TLatex *tex_p = new TLatex(2.3,400,"p");
  TLatex *tex_a = new TLatex(1.4,390,"#alpha");
  TLatex *tex_g = new TLatex(0.6,180,"#gamma + PD");
  TLatex *tex_label = new TLatex(1.0,400,"(b)");

  tex_p->Draw();
  tex_a->Draw();
  tex_g->Draw();
  tex_label->Draw();

  cav->SaveAs("qdc_pid_projection.pdf");

  // file_in->Close();
}
