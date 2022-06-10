
void get_data_energy_spec()
{
  gROOT->SetBatch(0);

  TFile *file_in = TFile::Open("../../../../../data/CsI/source/data_R0039/data_R0039_ana_all.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  TTree *tr = (TTree*)file_in->Get("tr");

  gROOT->Macro("cut_gamma_csi.C");
  gROOT->Macro("cut_alpha_csi.C");
  gROOT->Macro("cut_gamma_pd.C");

  //
  TH1D *h1 = new TH1D("h1", "", 512, 0, 1024);
  tr->Draw("energy>>h1", "cut_gamma_csi&&energy>0", "");

  TH1D *h2 = new TH1D("h2", "", 512, 0, 1024);
  tr->Draw("energy>>h2", "cut_alpha_csi&&energy>0", "");

  TH1D *h3 = new TH1D("h3", "", 512, 0, 1024);
  tr->Draw("energy>>h3", "cut_gamma_pd&&energy>0", "");

  ofstream file_out1, file_out2, file_out3;
  file_out1.open("gamma_csi.txt");
  file_out2.open("alpha_csi.txt");
  file_out3.open("gamma_pd.txt");

  for(int i=0;i<512;i++){
    file_out1 << i << " " << h1->GetBinContent(i) << endl;
    file_out2 << i << " " << h2->GetBinContent(i) << endl;
    file_out3 << i << " " << h3->GetBinContent(i) << endl;
  }

  file_out1.close();
  file_out2.close();
  file_out3.close();
}