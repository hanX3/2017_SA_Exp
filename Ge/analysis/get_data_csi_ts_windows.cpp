
void get_data_csi_ts_windows()
{
  gROOT->SetBatch(0);

  // TFile *file_in = TFile::Open("../../../data/Ge/root/R0050_3_sort_1.root");
  TFile *file_in = TFile::Open("../../../data/Ge/root/R0048_0000_sort_1.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  //
  TH1D *h1 = (TH1D*)file_in->Get("h_csi_ch16_csi_ch18");
  TH1D *h2 = (TH1D*)file_in->Get("h_clover_ch00_csi_ch16");

  ofstream file_out1, file_out2;
  file_out1.open("ts_win_csi_csi_wrong.txt");
  file_out2.open("ts_win_csi_clover_wrong.txt");

  h1->Rebin(10);
  h2->Rebin(10);

  //
  for(int i=0;i<200;i++){
    file_out1 << h1->GetBinCenter(i+1) << " " << h1->GetBinContent(i+1) << endl;
    file_out2 << h2->GetBinCenter(i+1) << " " << h2->GetBinContent(i+1) << endl;
  }

  file_out1.close();
  file_out2.close();
}