
void get_data_csi_qdc()
{
  gROOT->SetBatch(1);

  TFile *file_in = TFile::Open("../../../data/Ge/root/R0050_3_decode_2.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  TTree *tr1 = (TTree*)file_in->Get("tr_CsI_ch16");

  TH1D *h0 = new TH1D("h0", "", 4096, 0, 16384);
  TH1D *h1 = new TH1D("h1", "", 4096, 0, 16384);
  TH1D *h2 = new TH1D("h2", "", 4096, 0, 16384);
  TH1D *h3 = new TH1D("h3", "", 4096, 0, 16384);

  tr1->Draw("qdc[0]>>h0");
  tr1->Draw("qdc[1]>>h1");
  tr1->Draw("qdc[2]>>h2");
  tr1->Draw("qdc[3]>>h3");
  
  //
  ofstream file_out0, file_out1, file_out2, file_out3;
  file_out0.open("./csi16_qdc0.txt");
  file_out1.open("./csi16_qdc1.txt");
  file_out2.open("./csi16_qdc2.txt");
  file_out3.open("./csi16_qdc3.txt");


  for(int i=0;i<4096;i++){
    file_out0 << 4*i << "  " << h0->GetBinContent(i) << endl;
    file_out1 << 4*i << "  " << h1->GetBinContent(i) << endl;
    file_out2 << 4*i << "  " << h2->GetBinContent(i) << endl;
    file_out3 << 4*i << "  " << h3->GetBinContent(i) << endl;
  }

  file_out0.close();
  file_out1.close();
  file_out2.close();
  file_out3.close();

  //
  //
  TTree *tr2 = (TTree*)file_in->Get("tr_CsI_ch18");

  TH1D *h4 = new TH1D("h4", "", 4096, 0, 16384);
  TH1D *h5 = new TH1D("h5", "", 4096, 0, 16384);
  TH1D *h6 = new TH1D("h6", "", 4096, 0, 16384);
  TH1D *h7 = new TH1D("h7", "", 4096, 0, 16384);

  tr2->Draw("qdc[0]>>h4");
  tr2->Draw("qdc[1]>>h5");
  tr2->Draw("qdc[2]>>h6");
  tr2->Draw("qdc[3]>>h7");
  
  //
  ofstream file_out4, file_out5, file_out6, file_out7;
  file_out4.open("./csi18_qdc0.txt");
  file_out5.open("./csi18_qdc1.txt");
  file_out6.open("./csi18_qdc2.txt");
  file_out7.open("./csi18_qdc3.txt");


  for(int i=0;i<4096;i++){
    file_out4 << 4*i << "  " << h4->GetBinContent(i) << endl;
    file_out5 << 4*i << "  " << h5->GetBinContent(i) << endl;
    file_out6 << 4*i << "  " << h6->GetBinContent(i) << endl;
    file_out7 << 4*i << "  " << h7->GetBinContent(i) << endl;
  }

  file_out4.close();
  file_out5.close();
  file_out6.close();
  file_out7.close();
}