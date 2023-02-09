#define CLOVERNUMBER 10

// before draw_csi()
void draw_ge_230209()
{
  gROOT->SetBatch(1);

  TChain *cha = new TChain("tr");
  cha->Add("../../../R00*build*.root");

  Int_t clover_hit;
  Int_t clover_id[CLOVERNUMBER];
  Double_t clover_energy[CLOVERNUMBER];

  cha->SetBranchAddress("clover_hit", &clover_hit);
  cha->SetBranchAddress("clover_id", clover_id);
  cha->SetBranchAddress("clover_energy", clover_energy);

  cout << cha->GetEntries() << endl;

  Long64_t nentries = cha->GetEntries();
  TH1D *h[CLOVERNUMBER];//hit>0, hit>1, hit>2

  h[0] = new TH1D("h0", "", 2048, 0, 2048);// clover_hit>0
  h[1] = new TH1D("h1", "", 2048, 0, 2048);// clover_hit>1
  h[2] = new TH1D("h2", "", 2048, 0, 2048);// clover_hit>2

  for(Long64_t i=0;i<nentries;i++){
    if(i%1000000==0) cout << i << "/" << nentries << endl;
    cha->GetEntry(i);

    if(clover_hit>0){
      for(int j=0;j<clover_hit;j++)
        h[0]->Fill(clover_energy[j]);
    }

    if(clover_hit>1){
      for(int j=0;j<clover_hit;j++)
        h[1]->Fill(clover_energy[j]);
    }

    if(clover_hit>2){
      for(int j=0;j<clover_hit;j++)
        h[2]->Fill(clover_energy[j]);
    }
  }

  /*
  auto c0 = new TCanvas();
  c0->cd();
  h[0]->Draw();

  auto c1 = new TCanvas();
  c1->cd();
  h[1]->Draw();

  auto c2 = new TCanvas();
  c2->cd();
  h[2]->Draw();
  */

  ofstream file_out[3];
  //h[0]
  file_out[0].open("hit_gt0.txt");
  for(int i=0;i<2048;i++){
    //cout << i << "  " << h[0]->GetBinContent(i) << endl;
    file_out[0] << i << "  " << h[0]->GetBinContent(i) << endl;
  }
  
  //h[1]
  file_out[1].open("hit_gt1.txt");
  for(int i=0;i<2048;i++){
    //cout << i << "  " << h[1]->GetBinContent(i) << endl;
    file_out[1] << i << "  " << h[1]->GetBinContent(i) << endl;
  }
  
  //h[0]
  file_out[2].open("hit_gt2.txt");
  for(int i=0;i<2048;i++){
    //cout << i << "  " << h[0]->GetBinContent(i) << endl;
    file_out[2] << i << "  " << h[2]->GetBinContent(i) << endl;
  }
  
  for(int i=0;i<3;i++){
    file_out[i].close();
  }

  TFile *ff = new TFile("draw_th1.root", "recreate");
  ff->cd();
  h[0]->Write();
  h[1]->Write();
  h[2]->Write();
  ff->Close();
}
