#define MAXLENGTH 32768

//
// p[0]: tau rc (in points)
// p[1]: am
Double_t Fittaurc(Double_t *i, Double_t *p)
{
  Double_t x = i[0];
  Double_t s = p[1]*exp(-x/p[0]);

  return s;
}

void get_taurc(Int_t trc)
{
  gROOT->Macro("cut_gamma_pd.C");
  TCutG *cutg = (TCutG*)gROOT->GetListOfSpecials()->FindObject("cut_gamma_pd");

  TFile *fi1 = TFile::Open("../../../../../data/CsI/source/data_R0039.root");
  if(fi1->IsZombie()){
    cout << "wrong open the fi1" << endl;
    return;
  }
  TTree *tr1 = (TTree*)fi1->Get("tree");

  UShort_t ltra;
  UShort_t data[MAXLENGTH];
  UShort_t dt[MAXLENGTH];
  tr1->SetBranchAddress("ltra", &ltra);
  tr1->SetBranchAddress("data", data);
  tr1->SetBranchAddress("dt", dt);

  TFile *fi2 = TFile::Open("../../../../../data/CsI/source/data_R0039/data_R0039_ana_all.root");
  if(fi2->IsZombie()){
    cout << "wrong open the fi2" << endl;
    return;
  }

  Double_t qdc_result_data[2];

  TTree *tr2 = (TTree*)fi2->Get("tr");
  tr2->SetBranchAddress("qdc_result", qdc_result_data);

  tr2->GetEntry(0);
  cout << "qdc_result_energy " << qdc_result_data[0] << endl;
  cout << "qdc_result_energy " << qdc_result_data[1] << endl;

  TFile *fo = new TFile(TString::Format("taurc_%d.root",trc).Data(), "recreate");
  TTree *tr_out = new TTree("tr", "get taurc");
  Double_t chi2;
  Long64_t nevt;
  tr_out->Branch("chi2", &chi2, "chi2/D");
  tr_out->Branch("nevt", &nevt, "nevt/L");

  bool b_cut = 0;
  for(Long64_t i=0;i<tr1->GetEntries();i++){
    if(i%10000==0){
      cout << i << "/" << tr1->GetEntries() << endl;
    }

    tr2->GetEntry(i);
    b_cut = cutg->IsInside(qdc_result_data[1], qdc_result_data[0]);
    // cout << "b_cut " << b_cut << endl;

    if(b_cut){
      tr1->GetEntry(i);
      TH1D *h = new TH1D(TString::Format("h%09lld",i).Data(), "", ltra, 0, ltra);
      Double_t baseline = 0;
      for(int j=0;j<500;j++){
        baseline += data[j];
      }
      baseline /= 500.;

      for(int j=0;j<ltra;j++){
        h->SetBinContent(j+1, data[j]-baseline);
      }
      TF1 *tf = new TF1(TString::Format("tf%09lld",i).Data(), Fittaurc, ltra-1000, ltra, 2);
      tf->FixParameter(0, trc);
      tf->SetParameter(1, 100);
      h->Fit(tf, "QWN0RS");

      chi2 = tf->GetChisquare();
      nevt = i;
      // cout << "chi2 " << chi2 << endl;

      tr_out->Fill();

      b_cut = 0;
      chi2 = 0;
      nevt = 0;
      delete h;
      delete tf;
    }
  }

  fo->cd();
  tr_out->Write();
  fo->Close();
}