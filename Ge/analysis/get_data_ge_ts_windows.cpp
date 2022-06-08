
void get_data_ge_ts_windows()
{
  gROOT->SetBatch(0);

  TFile *file_in = TFile::Open("../../../data/Ge/root/ts_win.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  //
  TH1D *h1 = (TH1D*)file_in->Get("h_clover_ch00_clover_ch01");
  TH1D *h2 = (TH1D*)file_in->Get("h_clover_ch00_clover_ch08");

  ofstream file_out1, file_out2;
  file_out1.open("ts_win_same_clover.txt");
  file_out2.open("ts_win_diff_clover.txt");

  //
  for(int i=0;i<200;i++){
    file_out1 << -1000.+10.*i << " " << h1->GetBinContent(100000-100+i) << endl;
    file_out2 << -1000.+10.*i << " " << h2->GetBinContent(100000-100+i) << endl;
  }

  file_out1.close();
  file_out2.close();
}