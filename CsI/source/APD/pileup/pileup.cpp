#define MAXTRIGGERNUM 2
#define MAXLENGTH 32768

//
Double_t Fun1s(Double_t *i, Double_t *p);
Double_t Fun2s(Double_t *i, Double_t *p);
Double_t Fun3s(Double_t *i, Double_t *p);

Double_t FunA(Double_t *i, Double_t *p);
Double_t FunB(Double_t *i, Double_t *p);
Double_t FunC(Double_t *i, Double_t *p);
Double_t FunD(Double_t *i, Double_t *p);
Double_t FunE(Double_t *i, Double_t *p);

//
Double_t LightOut(Double_t *i, Double_t *p);
Double_t PreAmp(Double_t *i, Double_t *p);

//
void pileup(Long64_t n)
{
  TFile *fi1 = TFile::Open("../../../../../data/CsI/source/run_R0040/run_R0040_ana_all.root");
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

  TF1 *tf_1s = new TF1("tf_1s", Fun1s, 0, 5000, 7);
  tf_1s->SetParameter(0, 0);
  tf_1s->SetParameter(1, 4500);
  tf_1s->SetParameter(2, 500);
  tf_1s->SetParameter(3, 200);
  tf_1s->SetParameter(4, 2000);
  tf_1s->SetParameter(5, 1000);
  tf_1s->SetParameter(6, 1000);
  h1->Fit("tf_1s", "RL");


  TCanvas *c2 = new TCanvas("c2", "", 0, 0, 480, 360);
  c2->cd();

  TH1D *h2 = new TH1D("h2", "", ltra, 0, ltra);
  for(int i=0;i<ltra;i++){
    h2->SetBinContent(i+1, dataa[i]-tf_1s->Eval(i));
  }

  h2->Draw();

  TF1 *tf_1s_2 = new TF1("tf_1s_2", Fun1s, 5000, 10000, 7);
  tf_1s_2->SetParameter(0, 0);
  tf_1s_2->SetParameter(1, 4500);
  tf_1s_2->SetParameter(2, 6000);
  tf_1s_2->SetParameter(3, 200);
  // tf_1s_2->SetParLimits(3, 50, 1000);
  tf_1s_2->SetParameter(4, 2000);
  tf_1s_2->SetParameter(5, 1000);
  tf_1s_2->SetParameter(6, 1000);
  for(int i=0;i<6;i++)
    h2->Fit("tf_1s_2", "RLM");

  TCanvas *c3 = new TCanvas("c3", "", 0, 0, 480, 360);
  c3->cd();

  TH1D *h3 = new TH1D("h3", "", ltra, 0, ltra);
  for(int i=0;i<ltra;i++){
    h3->SetBinContent(i+1, dataa[i]);
  }

  h3->Draw();

  TF1 *tf_2s = new TF1("tf_2s", Fun2s, 0, 10000, 13);
  tf_2s->FixParameter(0, 0);
  tf_2s->FixParameter(1, tf_1s->GetParameter(1));
  tf_2s->FixParameter(2, tf_1s->GetParameter(2));
  tf_2s->SetParameter(3, tf_1s->GetParameter(3));
  tf_2s->SetParameter(4, tf_1s->GetParameter(4));
  tf_2s->SetParameter(5, tf_1s->GetParameter(5));
  tf_2s->SetParameter(6, tf_1s->GetParameter(6));
  tf_2s->FixParameter(7, tf_1s_2->GetParameter(1));
  tf_2s->FixParameter(8, tf_1s_2->GetParameter(2));
  tf_2s->SetParameter(9, tf_1s_2->GetParameter(3));
  tf_2s->SetParameter(10, tf_1s_2->GetParameter(4));
  tf_2s->SetParameter(11, tf_1s_2->GetParameter(5));
  tf_2s->SetParameter(12, tf_1s_2->GetParameter(6));

  tf_2s->Draw();
  h3->Fit("tf_2s", "RL");

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

  // TF1 *tfd = new TF1("tfd", FunD, 0, 10000, 9);
  // tfd->SetParameter(0, 0);
  // tfd->SetParameter(1, 600);
  // tfd->SetParameter(2, 4000);
  // tfd->SetParameter(3, 50);
  // tfd->SetParameter(4, 1000);
  // tfd->SetParameter(5, 500);
  // tfd->SetParameter(6, 1000);
  // tfd->SetParameter(7, 2500);
  // tfd->SetParameter(8, 1000);
  // h1->Fit("tfd", "RW");

  // TF1 *tfe = new TF1("tfe", FunE, 0, 10000, 11);
  // tfe->SetParameter(0, 0);
  // tfe->SetParameter(1, 600);
  // tfe->SetParameter(2, 4000);
  // tfe->SetParameter(3, 10);
  // tfe->SetParameter(4, 1000);
  // tfe->SetParameter(5, 100);
  // tfe->SetParameter(6, 1000);
  // tfe->SetParameter(7, 500);
  // tfe->SetParameter(8, 1000);
  // tfe->SetParameter(9, 2000);
  // tfe->SetParameter(10, 1000);
  // h1->Fit("tfe", "RW");

  // TF1 *tf1s = new TF1("tf1s", Fun1s, 0, 10000, 7);
  // tf1s1->SetParameter(0, 0);
  // tf1s->SetParameter(1, 600);
  // tf1s->SetParameter(2, 4000);
  // tf1s->SetParameter(3, 100);
  // tf1s->SetParameter(4, 1000);
  // tf1s->SetParameter(5, 500);
  // tf1s->SetParameter(6, 1000);
  // h1->Fit("tf1s", "RW");

  // TVirtualFitter::SetDefaultFitter("Minuit2");
  // ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(10000);
  // ROOT::Math::MinimizerOptions::SetDefaultTolerance(100);

  // TF1 *tf2s = new TF1("tf2s", Fun2s, 0, 10000, 13);
  // tf2s->SetParameter(0, 0);
  // tf2s->SetParLimits(0, -1, 1);
  // tf2s->SetParameter(1, 4500);
  // tf2s->SetParameter(2, 400);
  // // tf2s->SetParLimits(2, 200, 600);
  // tf2s->SetParameter(3, 100);
  // // tf2s->SetParLimits(3, 50, 600);
  // tf2s->SetParameter(4, 1000);
  // // tf2s->SetParLimits(4, 10, 3000);
  // tf2s->SetParameter(5, 1800);
  // // tf2s->SetParLimits(5, 1000, 6000);
  // tf2s->SetParameter(6, 1000);
  // // tf2s->SetParLimits(6, 10, 3000);
  // tf2s->SetParameter(7, 8000);
  // // tf2s->SetParLimits(7, 7000, 9000);
  // tf2s->SetParameter(8, 8000);
  // tf2s->SetParameter(9, 100);
  // // tf2s->SetParLimits(8, 50, 600);
  // tf2s->SetParameter(10, 1000);
  // // tf2s->SetParLimits(9, 10, 3000);
  // tf2s->SetParameter(11, 1800);
  // // tf2s->SetParLimits(10, 1000, 6000);
  // tf2s->SetParameter(12, 1000);
  // // tf2s->SetParLimits(11, 10, 3000);

  // h1->Fit("tf2s", "RML");

  // TF1 *tf_light_out = new TF1("tf_light_out", LightOut, 0, 10000, 5);
  // TF1 *tf_pre_amp = new TF1("tf_pre_amp", PreAmp, 0, 10000, 1);

  // TF1Convolution *tf_conv = new TF1Convolution("tf_light_out", "tf_pre_amp", -10000, 10000, true);
  // tf_conv->SetRange(-10000, 10000);
  // tf_conv->SetNofPointsFFT(100000);
  // TF1 *tf = new TF1("tf", *tf_conv, 0, 10000, tf_conv->GetNpar());
  // cout << "tf_conv->GetNpar() " << tf_conv->GetNpar() << endl;
  // tf->SetParameter(0, 0);
  // tf->SetParameter(1, 200);
  // tf->SetParameter(2, 2000);
  // tf->SetParameter(3, 1000);
  // tf->SetParameter(4, 1000);
  // tf->SetParameter(5, 4000);

  // tf->Draw();
  // h1->Fit("tf", "R");
  
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
// p[3]: tau 1 (in points)
// p[4]: am 1
// p[5]: tau 2 (in points)
// p[6]: am 2
// p[7]: tau 3 (in points)
// p[8]: am 3
// p[9]: tau 4 (in points)
// p[10]: am 4
Double_t FunE(Double_t *i, Double_t *p)
{
  Double_t s = p[0];
  Double_t x = i[0] - p[1];
  Double_t e = exp(-x/p[2]);

  if(x<0) return s;
  else{
    s += p[4]*(1-exp(-x/p[3]))*e; 
    s += p[6]*(1-exp(-x/p[5]))*e;
    s += p[8]*(1-exp(-x/p[7]))*e;
    s += p[10]*(1-exp(-x/p[9]))*e;
    return s;
  }
}

//
// p[0]: baseline
// p[1]: tau rc (in points)
// p[2]: t0 (in points)
// p[3]: tau fast (in points)
// p[4]: am fast
// p[5]: tau slow (in points)
// p[6]: am slow
Double_t Fun1s(Double_t *i, Double_t *p)
{
  Double_t s = p[0];
  Double_t x = i[0] - p[2];
  Double_t e = exp(-x/p[1]);

  if(x<0) return s;
  else{
    s += p[4]*(1-exp(-x/p[3]))*e; 
    s += p[6]*(1-exp(-x/p[5]))*e;
    return s;
  }
}

//
// p[0]: baseline
// p[1]: 0 --> tau rc (in points)
// p[2]: 0 --> t0 (in points)
// p[3]: 0 --> tau fast (in points)
// p[4]: 0 --> am fast
// p[5]: 0 --> tau slow (in points)
// p[6]: 0 --> am slow
// p[7]: 1 --> tau rc (in points)
// p[8]: 1 --> t0 (in points)
// p[9]: 1 --> tau fast (in points)
// p[10]: 1 --> am fast
// p[11]: 1 --> tau slow (in points)
// p[12]: 1 --> am slow
Double_t Fun2s(Double_t *i, Double_t *p)
{
  Double_t s = 0;
  Double_t x = 0;
  Double_t e1 = 0;
  Double_t e2 = 0;

  if((i[0]-p[2])<0) return p[0];
  else if((i[0]-p[2])>=0 && (i[0]-p[8])<0){
    x = i[0]-p[2];
    e1 = exp(-x/p[1]);
    s += p[0];
    s += p[4]*(1-exp(-x/p[3]))*e1;
    s += p[6]*(1-exp(-x/p[5]))*e1;
    return s;
  }else{
    x = i[0]-p[8];
    e1 = exp(-x/p[1]);
    e2 = exp(-x/p[7]);
    s += p[0];
    s += p[4]*(1-exp(-x/p[3]))*e1;
    s += p[6]*(1-exp(-x/p[5]))*e1;
    s += p[10]*(1-exp(-x/p[9]))*e2;
    s += p[12]*(1-exp(-x/p[11]))*e2;
    return s;
  }
}

//
// p[0]: tf
// p[1]: ts
// p[2]: nf
// p[3]: ns
Double_t LightOut(Double_t *i, Double_t *p)
{
  Double_t s = 0.;
  Double_t x = i[0] - p[0];

  s += p[3]/p[1]*exp(-x/p[1]);
  s += p[4]/p[2]*exp(-x/p[2]);
  return s;
}

//
// p[0]: tp
Double_t PreAmp(Double_t *i, Double_t *p)
{
  Double_t s = 0.;
  Double_t x = i[0];

  s += exp(-x/p[0]);

  return s;
}