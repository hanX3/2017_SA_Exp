
void get_gamma_th1()
{
  TFile *file_in = TFile::Open("../../../draw_th1.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  TH1D *h0 = (TH1D*)file_in->Get("h0");
  TH1D *h1 = (TH1D*)file_in->Get("h1");
  TH1D *h2 = (TH1D*)file_in->Get("h2");
  
  ofstream file_out0, file_out1, file_out2;
  file_out0.open("../../../gamma_gt0.txt");
  file_out1.open("../../../gamma_gt1.txt");
  file_out2.open("../../../gamma_gt2.txt");

  for(int i=0;i<2048;i++){
  	file_out0 << i << "  " << h0->GetBinContent(i) << endl;
  	file_out1 << i << "  " << h1->GetBinContent(i) << endl;
  	file_out2 << i << "  " << h2->GetBinContent(i) << endl;
  }

  file_out0.close();
  file_out1.close();
  file_out2.close();

  file_in->Close();

  return ;
}