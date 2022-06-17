#define MAXTRIGGERNUM 2
#define MAXLENGTH 32768

//
Double_t Fun1s(Double_t *i, Double_t *p);
// Double_t Fun2s(Double_t *i, Double_t *p);
// Double_t Fun3s(Double_t *i, Double_t *p);

Double_t FunA(Double_t *i, Double_t *p);
Double_t FunB(Double_t *i, Double_t *p);
Double_t FunC(Double_t *i, Double_t *p);
Double_t FunD(Double_t *i, Double_t *p);

//
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

  // TF1 *tfa = new TF1("tfa", FunA, 0, 10000, 5);
  // tfa->SetParameter(0, 0);
  // tfa->SetParameter(1, 600);
  // tfa->SetParameter(2, 400);
  // tfa->SetParameter(3, 400);
  // tfa->SetParameter(4, 1000);
  // h1->Fit("tfa", "RW");

  // TF1 *tfb = new TF1("tfb", FunB, 0, 10000, 4);
  // tfb->SetParameter(0, 0);
  // tfb->SetParameter(1, 600);
  // tfb->SetParameter(2, 400);
  // tfb->SetParameter(3, 400);
  // h1->Fit("tfb", "RW");

  // TF1 *tfc = new TF1("tfc", FunC, 0, 10000, 3);
  // tfc->SetParameter(0, 0);
  // tfc->SetParameter(1, 600);
  // tfc->SetParameter(2, 1000); 
  // h1->Fit("tfc", "RW");

  TF1 *tfd = new TF1("tfd", FunD, 0, 10000, 9);
  tfd->SetParameter(0, 0);
  tfd->SetParameter(1, 600);
  tfd->SetParameter(2, 4000);
  tfd->SetParameter(3, 50);
  tfd->SetParameter(4, 1000);
  tfd->SetParameter(5, 500);
  tfd->SetParameter(6, 1000);
  tfd->SetParameter(7, 2500);
  tfd->SetParameter(8, 1000);
  h1->Fit("tfd", "RW");

  
  return ;
}

//
Double_t FunA(Double_t *i, Double_t *p)
{
  Double_t s = p[0];
  Double_t x = i[0] - p[1];
  Double_t e = exp(-x/p[2]);

  if(x<0) return s;
  else{
    s += p[4]*(1-exp(-x/p[3]))*e;
    return s;
  }
}

//
Double_t FunB(Double_t *i, Double_t *p)
{
  Double_t s = p[0];
  Double_t x = i[0] - p[1];

  if(x<0) return s;
  else{
    s += (p[1] + p[2]*x)*exp(-x/p[3]);
    return s;
  }
}

//
Double_t FunC(Double_t *i, Double_t *p)
{
  Double_t s = p[0];
  Double_t x = i[0] - p[1];

  if(x<0) return s;
  else{
    s += p[2]*exp(-(log(x*x)));
    return s;
  }
}

//
// p[0]: baseline
// p[1]: t0 (in points)
// p[2]: tau rc (in points)
// p[3]: tau 1 (in points)
// p[4]: am 1
// p[5]: tau 2 (in points)
// p[6]: am 2
// p[7]: tau 3 (in points)
// p[8]: am 3
Double_t FunD(Double_t *i, Double_t *p)
{
  Double_t s = p[0];
  Double_t x = i[0] - p[1];
  Double_t e = exp(-x/p[2]);

  if(x<0) return s;
  else{
    s += p[4]*(1-exp(-x/p[3]))*e; 
    s += p[6]*(1-exp(-x/p[5]))*e;
    s += p[8]*(1-exp(-x/p[7]))*e;
  return s;
  }
}

//
// p[0]: baseline
// p[1]: t0 (in points)
// p[2]: tau rc (in points)
// p[3]: tau fast (in points)
// p[4]: tau slow (in points)
// p[5]: am fast
// p[6]: am slow
Double_t Fun1s(Double_t *i, Double_t *p)
{
  Double_t s = p[0];
  Double_t x = i[0] - p[1];
  Double_t e = exp(-x/p[2]);

  if(x<0) return s;
  else{
    s += p[5]*(1-exp(-x/p[3]))*e; 
    s += p[6]*(1-exp(-x/p[4]))*e;
  return s;
  }
}

// //
// // p[0]: baseline
// // p[1]: 0 --> t0 (in points)
// // p[2]: tau rc (in points)
// // p[3]: 0 --> tau fast (in points)
// // p[4]: 0 --> tau slow (in points)
// // p[5]: 0 --> am fast
// // p[6]: 0 --> am slow
// Double_t Fun2s(Double_t *i, Double_t *p)
// {
//   Double_t s = p[0];
//   Double_t x = i[0] - p[1];
//   Double_t e = exp(-x/p[2]);

//   if(x<0) return s;
//   else if(){
//     s += p[5]*(1-exp(-x/p[3]))*e; 
//     s += p[6]*(1-exp(-x/p[4]))*e;
//   return s;
//   }
// }