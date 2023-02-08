#define CSINUMBER 64

// after draw_ge()
void draw_csi()
{
  gROOT->SetBatch(1);

  TChain *cha = new TChain("tr");
  cha->Add("../../../R0050*build*.root");

  Int_t csi_hit;
  Int_t csi_id[CSINUMBER];
  Double_t csi_qdc1[CSINUMBER];
  Double_t csi_qdc2[CSINUMBER];
  Double_t csi_qdc3[CSINUMBER];
  Double_t csi_qdc4[CSINUMBER];

  cha->SetBranchAddress("csi_hit", &csi_hit);
  cha->SetBranchAddress("csi_id", csi_id);
  cha->SetBranchAddress("csi_qdc1", csi_qdc1);
  cha->SetBranchAddress("csi_qdc2", csi_qdc2);
  cha->SetBranchAddress("csi_qdc3", csi_qdc3);
  cha->SetBranchAddress("csi_qdc4", csi_qdc4);

  cout << cha->GetEntries() << endl;
  cha->Show(0);

  Long64_t nentries = cha->GetEntries();
  TH2D *h[CSINUMBER];

  for(int i=0;i<CSINUMBER;i++){
    h[i] = new TH2D(TString::Format("h_csi_ch%02d",i).Data(), "", 20000, 0, 40000, 4000, 0, 8000);
  }

  Double_t xx, yy;
  for(Long64_t i=0;i<nentries;i++){
    if(i%10000000==0) cout << i << "/" << nentries << endl;
    cha->GetEntry(i);

    if(csi_hit==0){
      continue;
    }

    for(int j=0;j<csi_hit;j++){
      if(csi_qdc1[j]>10000 || csi_qdc2[j]<6000 || csi_qdc2[j]>9000 || csi_qdc3[j]<3500 || csi_qdc3[j]>5000 || csi_qdc4[j]<10000){
        continue;
      }
      xx = 2.*csi_qdc4[j]-2.*csi_qdc1[j];
      yy = csi_qdc2[j]-2./3.*csi_qdc1[j];

      // h[csi_id[j]]->Fill(xx, yy);
      h[csi_id[j]]->Fill(yy, xx);
    }
  }
  
  TFile *ff = new TFile("draw_th1_230129.root", "update");
  ff->cd();
  for(int i=0;i<CSINUMBER;i++){
    //cout << i << " " << h[i]->GetSum() << endl;
    if(h[i]->GetSum()==0) continue;
    h[i]->Write();
  }

  ff->Close();
}
