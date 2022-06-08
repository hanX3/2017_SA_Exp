
void get_data_ge_addback()
{
  gROOT->SetBatch(0);

  TFile *file_in = TFile::Open("../../../data/Ge/root/R0058_0000_sort_2.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  TTree *tr1 = (TTree*)file_in->Get("tr_Clover00_cali");
  TTree *tr2 = (TTree*)file_in->Get("tr_Clover00_cali_addback");

  TH1D *h1 = new TH1D("h1", "", 2048, 0, 2048);
  TH1D *h2 = new TH1D("h2", "", 2048, 0, 2048);

  tr1->Draw("adc_cali>>h1");
  tr2->Draw("adc_cali_addback>>h2");

  //
  ofstream file_out1, file_out2;
  file_out1.open("./ge_no_addback.txt");
  file_out2.open("./ge_addback.txt");

  for(int i=0;i<2048;i++){
    file_out1 << i << "  " << h1->GetBinContent(i) << endl;
    file_out2 << i << "  " << h2->GetBinContent(i) << endl;
  }

  file_out1.close();
  file_out2.close();
}