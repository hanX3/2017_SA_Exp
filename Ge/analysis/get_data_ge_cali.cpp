
void get_data_ge_cali()
{
  gROOT->SetBatch(0);

  TFile *file_in = TFile::Open("../../../data/Ge/root/R0032_0000_decode_2.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  TTree *tr = (TTree*)file_in->Get("tr_Clover_ch00");

  //
  TH1D *h = new TH1D("h", "", 2048, 0, 2048);
  tr->Draw("adc>>h");

  ofstream file_out1;
  file_out1.open("./ge_cali.txt");

  for(int i=0;i<2048;i++){
    file_out1 << i << "  " << h->GetBinContent(i) << endl;
  }

  file_out1.close();

  //
  TF1 *tf = new TF1("tf", "[0]+[1]*x+gaus(2)", 820, 850);
  tf->SetParameters(800, 0., 9000, 835., 0.2);
  h->Fit("tf", "R");

  ofstream file_out2;
  file_out2.open("./ge_cali_fit.txt");


  for(int i=0;i<3000;i++){
    file_out2 << 820+0.01*i << "  " << tf->Eval(820+0.01*i) << endl;
  }

  file_out2.close();
}
