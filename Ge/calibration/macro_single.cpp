{
  gROOT->ProcessLine("TFile *f = TFile::Open(\"../../../data/Ge/root/R0032_0000_decode_2.root\")");
  gROOT->ProcessLine("TH1D *h = new TH1D(\"h\",\"\",8192,0,8192)");
  gROOT->ProcessLine("tr_Clover_ch04->Draw(\"adc>>h\")");
  gROOT->ProcessLine("Float_t par[4]");
  gROOT->ProcessLine(".L auto_cali.cpp");
  gROOT->ProcessLine("auto_cali(h, par)");
}
