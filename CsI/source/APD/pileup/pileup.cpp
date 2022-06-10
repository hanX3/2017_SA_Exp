#define MAXTRIGGERNUM 2
#define MAXLENGTH 32768



void pileup(Long64_t n)
{
  TFile *fi1 = TFile::Open("../../../../../data/CsI/source/run_R0040/run_R0040_000000000_000001000_ana.root");
  if(fi1->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  Double_t trapz_result_data[MAXTRIGGERNUM+MAXTRIGGERNUM];

  TTree *tr1 = (TTree*)fi1->Get("tr");
  tr1->Show(n);
  
  tr1->SetBranchAddress("trapz_result", trapz_result_data);

  tr1->GetEntry(n);
  cout << "trapz_result_energy " << trapz_result_data[0] << endl;
  cout << "trapz_result_energy " << trapz_result_data[1] << endl;
  cout << "trapz_result_time_tag " << trapz_result_data[2] << endl;
  cout << "trapz_result_time_tag " << trapz_result_data[3] << endl;

  //
  TFile *fi2 = TFile::Open("../../../../../data/CsI/source/run_R0040.root");
  if(fi2->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }


  UShort_t ltra;
  UShort_t dt[MAXLENGTH];
  UShort_t data[MAXLENGTH];

  TTree *tr2 = (TTree*)fi2->Get("tree");
  tr2->SetBranchAddress("size", &ltra);
  tr2->SetBranchAddress("dt", dt);
  tr2->SetBranchAddress("data", data);

  tr2->GetEntry(n);

  Double_t dtt[ltra];
  Double_t dataa[ltra];

  Double_t baseline = 0;
  for(int i=0;i<300;i++){
    baseline += (Double_t)data[i];
  }
  baseline /= 300.;

  for(int i=0;i<ltra;i++){
    dtt[i] = (Double_t)dt[i];
    dataa[i] = (Double_t)baseline - (Double_t)data[i];
  }

  TCanvas *c1 = new TCanvas("c1", "", 0, 0, 480, 360);
  c1->cd();

  TH1D *h1 = new TH1D("h1", "", ltra, 0, ltra);
  for(int i=0;i<ltra;i++){
    h1->SetBinContent(i+1, dataa[i]);
  }

  h1->Draw();

  //expo fit
  




  return ;
}