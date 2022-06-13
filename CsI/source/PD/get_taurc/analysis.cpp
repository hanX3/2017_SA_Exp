
void analysis()
{
  int t[] = {1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000};

  TGraph *gr = new TGraph();
  ofstream fo;
  fo.open("get_taurc.txt");

  std::vector<Double_t> v;

  int size = sizeof(t)/sizeof(t[0]);
  for(int i=0;i<size;i++){
    TFile *fi = TFile::Open(TString::Format("taurc_%d.root",t[i]).Data());
    if(fi->IsZombie()){
      cout << "wrong open the file" << TString::Format("taurc_%d",t[i]) << endl;
      return;
    }

    TTree *tr = (TTree*)fi->Get("tr");
    Double_t chi2;
    tr->SetBranchAddress("chi2", &chi2);

    cout << "nentries " << tr->GetEntries() << endl;
    
    v.clear();
    for(int j=0;j<tr->GetEntries();j++){
      tr->GetEntry(j);
      v.push_back(chi2);
    }

    sort(v.begin(), v.end());
    Double_t mean = 0;
    for(int j=0;j<10000;j++){
      mean += v[j];
    }

    mean /= 10000.;

    cout << "mean " << mean << endl;
    gr->SetPoint(i, t[i]/100., mean);
    fo << t[i]/100. << " " << mean << endl;

    delete tr;
    fi->Close();
  }

  gr->Draw("AP*");

}