
void paper_draw_qdc_pid_230215()
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
  TFile *file_in = TFile::Open("../../../draw_th1.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  TH2D *h = (TH2D*)file_in->Get("h_csi_ch16");
  TH2D *hh = new TH2D("hh", "", 20000, 0, 6000, 6000, 0, 600);

  double k = 0;
  for(int i=0;i<20000;i++){
    for(int j=0;j<4000;j++){
      k = h->GetBinContent(i, j);
      hh->SetBinContent(j, i, k);
    }
  }

  TCanvas *cav = new TCanvas("cav", "", 0, 0, 520, 360);
  cav->cd();

  hh->SetMinimum(2);
  hh->GetXaxis()->SetTitle("Short Gate");
  hh->GetXaxis()->SetTitleSize(0.06);
  hh->GetYaxis()->SetTitle("Long Gate");
  hh->GetYaxis()->SetTitleSize(0.06);
  hh->GetXaxis()->SetRangeUser(10, 200);
  hh->GetYaxis()->SetRangeUser(10, 500);
  hh->GetXaxis()->CenterTitle();
  hh->GetYaxis()->CenterTitle();

  hh->Draw("col");

  TLatex *tex_p = new TLatex(145,420,"p");
  TLatex *tex_a = new TLatex(170,320,"#alpha");
  TLatex *tex_g = new TLatex(120,100,"#gamma + PD");
  TLatex *tex_label = new TLatex(40,430,"(a)");

  tex_p->Draw();
  tex_a->Draw();
  tex_g->Draw();
  tex_label->Draw();

  cav->SaveAs("qdc_pid.pdf");

  //file_in->Close();
}
