#include "WaveAnalysis.h"

//
WaveAnalysis::WaveAnalysis(const std::string &filename_in, const std::string &filename_out, int a, int b)
{
  benchmark = new TBenchmark;

  entry_start = a;
  entry_stop = b;

  file_in = TFile::Open(filename_in.c_str());
  if(file_in->IsZombie()){
    std::cout << "can not open " << filename_in << std::endl;
  }
  tr_in = (TTree*)file_in->Get("tr");
  tr_in->SetBranchAddress("ltra", &ltra);
  tr_in->SetBranchAddress("data", data);
  total_entry = tr_in->GetEntries();

  memset(&cfd_par, 0, sizeof(CFD_PAR));
  memset(&trapz_par, 0, sizeof(TRAPZ_PAR));

  memset(data, 0, MAXLENGTH*sizeof(UShort_t));
  memset(data_bl, 0, MAXLENGTH*sizeof(Double_t));
  memset(data_rccr2, 0, MAXLENGTH*sizeof(Double_t));
  memset(data_trapz, 0, MAXLENGTH*sizeof(Double_t));

  cfd_par.pre_trigger = CFDPARPRITRIGGER;
  cfd_par.smooth_length = CFDPARSMOOTHLENGTH;
  cfd_par.rise_length = CFDPARRISELENGTH;

  trapz_par.decay_length = TRAPPARDECAYLENGTH;
  trapz_par.rise_length = TRAPPARRISELENGTH;
  trapz_par.falt_top = TRAPPARRISELENGTH;
  trapz_par.pre_trigger = TRAPPARPRITRIGGER;
  trapz_par.peak_delay = TRAPPARPEAKDELAY;
  trapz_par.peak_mean = TRAPPARPEAKMEAN;

  //PID
  memset(&qdc_par, 0, sizeof(QDC_PAR));
  qdc_par.q1_start = Q1START;
  qdc_par.q1_stop = Q1STOP;
  qdc_par.q2_start = Q2START;
  qdc_par.q2_stop = Q2STOP;

  memset(&fit_par, 0, sizeof(FIT_PAR));
  fit_par.fit_start = FITSTART;
  fit_par.fit_stop = FITSTOP;
  fit_par.par[0] = BASELINE; 
  fit_par.par[1] = T0;
  fit_par.par[2] = TAURC; 
  fit_par.par[3] = TAUFAST;
  fit_par.par[4] = TAUSLOW;
  fit_par.par[5] = AMFAST;
  fit_par.par[6] = AMSLOW;

  fit_par.par_min[0] = BASELINEMIN; 
  fit_par.par_min[1] = T0MIN;
  fit_par.par_min[2] = TAURCMIN; 
  fit_par.par_min[3] = TAUFASTMIN;
  fit_par.par_min[4] = TAUSLOWMIN;
  fit_par.par_min[5] = AMFASTMIN;
  fit_par.par_min[6] = AMSLOWMIN;

  fit_par.par_max[0] = BASELINEMAX; 
  fit_par.par_max[1] = T0MAX;
  fit_par.par_max[2] = TAURCMAX; 
  fit_par.par_max[3] = TAUFASTMAX;
  fit_par.par_max[4] = TAUSLOWMAX;
  fit_par.par_max[5] = AMFASTMAX;
  fit_par.par_max[6] = AMSLOWMAX;

  file_out = new TFile(filename_out.c_str(), "recreate");
  tr_out = new TTree("tr", "wave analysis info");
  TString leaf1 = TString::Format("energy[%d]/D:time_tag[%d]/D:trigger_num/s", PILEUPMAX, PILEUPMAX);
  TString leaf2 = TString::Format("qdc_short/D:qdc_long/D");
  TString leaf3 = TString::Format("tau_fast/D:tau_slow/D:am_fast/D:am_slow/D");
  tr_out->Branch("trapz_result", &trapz_result, leaf1.Data());
  tr_out->Branch("qdc_result", &qdc_result, leaf2.Data());
  tr_out->Branch("fit_result", &fit_result, leaf3.Data());
  

#ifdef DEBUGDRAWOPTION
  cav1 = new TCanvas("cav1", "", 0, 0, 480, 360);
  cav2 = new TCanvas("cav2", "", 0, 0, 480, 360);
#endif
}

//
WaveAnalysis::~WaveAnalysis()
{

}

//
bool WaveAnalysis::GetWave(Long64_t n)
{
  ltra = 0;
  memset(data, 0, sizeof(UShort_t)*MAXLENGTH);
  tr_in->GetEntry(n);
  for(UInt_t i=0;i<ltra;i++){
    data[i] *= POLARITY;
  }

  cfd_par.threshold = CFDPARTHRESHOLD;
  memset(&trapz_result, 0, sizeof(TRAPZ_RESULT));

  return true;
}

//
void WaveAnalysis::Process()
{
  benchmark->Start("analysis");

  for(Long64_t i=entry_start;i<entry_stop;i++){
	if(i==total_entry) break;
    if(i%10000==0){
	  std::cout << i << "/" << total_entry << std::endl;
	}

    ProcessEntry(i);
    tr_out->Fill();
  }

  file_out->cd();
  tr_out->Write();
  file_out->Close();

  file_in->Close();

  benchmark->Show("analysis");
}

//
bool WaveAnalysis::ProcessEntry(Long64_t n)
{
  GetWave(n);

  // Energy
  RCCR2(n);
  cfd_par.threshold = CFDPARTHRESHOLD; 
  Bool_t trigger_armed = 0;
  UInt_t ii, jj = 0;
  UShort_t hit_num;
  Double_t ee1 = 0., ee2 =0.;
  while(1){
    ii = 0;
    hit_num = 0;
    trigger_armed = 0;
    for(UShort_t i=10;i<(ltra-10);i++){
      if(trigger_armed==0 && data_rccr2[i]>=cfd_par.threshold){
        trigger_armed = 1;
        ii = i-1;
        ee1 = (data[ii-3]+data[ii-2]+data[ii-1])/3.;
      }
      if(trigger_armed==1 && data_rccr2[i]>=0 && data_rccr2[i+1]<0){
        trigger_armed = 0;
        ee2 = (data[i+1]+data[i+2]+data[i+3])/3.;
        if(i-ii>=5 && ee2-ee1>=cfd_par.threshold){
          if(hit_num<PILEUPMAX){
            trapz_result.time_tag[hit_num] = (Double_t)i + (Double_t)data_rccr2[i]/((Double_t)data_rccr2[i]-(Double_t)data_rccr2[i+1]);
            hit_num++;
	      }
	    }
      }
    }

    if(hit_num==0)
      cfd_par.threshold = cfd_par.threshold-1;
    else if(hit_num>PILEUPMAX)
      cfd_par.threshold = cfd_par.threshold+1;
    else if(hit_num>0 && hit_num<=PILEUPMAX)
      break;

    jj++;
    if(jj>0xffff || cfd_par.threshold<=0x0000 || cfd_par.threshold>=0xffff){
      hit_num = 0;
      break;
    }
  }

  if(trapz_par.peak_delay==0){
    trapz_par.peak_delay = trapz_par.rise_length + trapz_par.falt_top/2;
  }
  if(trapz_par.peak_mean==0){
    trapz_par.peak_mean = trapz_par.falt_top/4;
  }
  trapz_par.pre_trigger = (trapz_result.time_tag[0]>20)?(trapz_result.time_tag[0]-20):20;

#ifdef DEBUGWAVEANALYSIS
  std::cout << "peak_delay " << trapz_par.peak_delay << std::endl;
  std::cout << "peak_mean " << trapz_par.peak_mean << std::endl;
  std::cout << "pre_trigger " << trapz_par.pre_trigger << std::endl;
#endif

  Double_t e_sum = 0;
  UInt_t e_start = 0;
  if(hit_num>0){
    Trapezoid(n);  
  }
  trapz_result.trigger_num = hit_num;
  for(UInt_t i=0;i<hit_num;i++){
    e_sum = 0;
    e_start = (UInt_t)(trapz_result.time_tag[i]) + trapz_par.peak_delay;
    if(e_start>=ltra) break;
    if((e_start+trapz_par.peak_mean)<ltra){
      for(UInt_t j=e_start;j<(e_start+trapz_par.peak_mean);j++){
        e_sum += data_trapz[j];
      }
      trapz_result.energy[i] = (e_sum>0 ? e_sum/trapz_par.peak_mean*4.0 : 0);
    }else{
      for(UInt_t j=e_start;j<ltra;j++){
        e_sum += data_trapz[j];
      }
      trapz_result.energy[i] = (e_sum>0 ? e_sum/trapz_par.peak_mean*4.0 : 0);
    }
  }

#ifdef DEBUGWAVEANALYSIS
  std::cout << "n " << n << std::endl;
  std::cout << "hit_num " << hit_num << std::endl;
  std::cout << "time_tag " << trapz_result.time_tag[0] << std::endl;
  std::cout << "time_tag " << trapz_result.time_tag[1] << std::endl;
  std::cout << "energy " << trapz_result.energy[0] << std::endl;
  std::cout << "energy " << trapz_result.energy[1] << std::endl;
#endif

  //PID
  CaliQDC(n);   
  FitWave(n);

  return true;
}

//
void WaveAnalysis::RCCR2(Long64_t n)
{
  memset(data_bl, 0, ltra*sizeof(Double_t));
  memset(data_rccr2, 0, ltra*sizeof(Double_t));

  baseline = 0.;
  for(UInt_t i=0;i<cfd_par.pre_trigger;i++){
    baseline += (Double_t)data[i];
  }
  baseline /= (Double_t)cfd_par.pre_trigger;

  for(UShort_t i=0;i<ltra;i++){
    data_bl[i] = (Double_t)data[i]-baseline; 
  }

  for(UShort_t i=(2*cfd_par.rise_length+cfd_par.smooth_length);i<ltra;i++){
    data_rccr2[i] = RCCR2XX(data_bl, i, cfd_par.smooth_length, cfd_par.rise_length) - RCCR2XX(data_bl, i-cfd_par.rise_length, cfd_par.smooth_length, cfd_par.rise_length);
  }
}

//
void WaveAnalysis::Trapezoid(Long64_t n)
{
  Double_t *d = (Double_t*)malloc(ltra*sizeof(Double_t));
  Double_t *q = (Double_t*)malloc(ltra*sizeof(Double_t));
  Double_t *ss = (Double_t*)malloc(ltra*sizeof(Double_t));
  
  memset(data_trapz, 0, ltra*sizeof(Double_t));

  Double_t M = trapz_par.decay_length;
  UInt_t k = trapz_par.rise_length;
  UInt_t m = trapz_par.falt_top;
  UInt_t l = k+m;

  for(UShort_t i=0;i<ltra;i++){
    if(i==0){
      d[i]=data_bl[i];
      q[i]=d[i];
      ss[i]=q[i]+M*d[i];
    }else if(i>0 && i<k){
      d[i]=data_bl[i];
      q[i]=q[i-1]+d[i];
      ss[i]=ss[i-1]+q[i]+M*d[i];
    }else if(i>=k && i<l){
      d[i]=data_bl[i]-data_bl[i-k];
      q[i]=q[i-1]+d[i];
      ss[i]=ss[i-1]+q[i]+M*d[i];
    }else if(i>=l && i<l+k){
      d[i]=data_bl[i]-data_bl[i-k]-data_bl[i-l];
      q[i]=q[i-1]+d[i];
      ss[i]=ss[i-1]+q[i]+M*d[i];
    }else if(i>=l+k){
      d[i]=data_bl[i]-data_bl[i-k]-data_bl[i-l]+data_bl[i-k-l];
      q[i]=q[i-1]+d[i];
      ss[i]=ss[i-1]+q[i]+M*d[i];
    }
    data_trapz[i] = ss[i]/M/(Double_t)k;
  }

  Double_t bl = 0.;
  for(UInt_t i=0;i<trapz_par.pre_trigger;i++){
    bl += data_trapz[i];
  }
  bl /= (Double_t)trapz_par.pre_trigger;
  for(UInt_t i=0;i<ltra;i++){
    data_trapz[i] -= bl;
  }

  free(d);
  free(q);
  free(ss);
}

//
void WaveAnalysis::CaliQDC(Long64_t n)
{
  memset(&qdc_result, 0, sizeof(QDC_RESULT));
  
  qdc_result.qdc_short = Integral(data_bl, qdc_par.q1_start, qdc_par.q1_stop);
  qdc_result.qdc_long = Integral(data_bl, qdc_par.q2_start, qdc_par.q2_stop);
}

//
void WaveAnalysis::FitWave(Long64_t n)
{
  TH1D *h = new TH1D(TString::Format("h%09lld",n).Data(), "", ltra, 0, ltra);
  for(UInt_t i=0;i<ltra;i++){
    h->SetBinContent(i+1, data_bl[i]);
  }

  TF1 *tf = new TF1(TString::Format("tf%09lld",n).Data(), Fittf, fit_par.fit_start, fit_par.fit_stop, 7);
  for(int i=0;i<7;i++){
    tf->SetParameter(i, fit_par.par[i]);
  }
  tf->SetParLimits(3, fit_par.par_min[3], fit_par.par_max[3]);
  //tf->SetParLimits(4, fit_par.par_min[4], fit_par.par_max[4]);

  //tf->FixParameter(0, fit_par.par[0]);
  tf->FixParameter(2, fit_par.par[2]);
  tf->FixParameter(4, fit_par.par[4]);

#ifdef DEBUGDRAWOPTION  
  cav2->cd();
  h->Draw();
  h->Fit(tf, "WR");
#else
  h->Fit(tf, "QWN0RS");
  fit_result.tau_fast = tf->GetParameter(3);
  fit_result.tau_slow = tf->GetParameter(4);
  fit_result.am_fast = tf->GetParameter(5);
  fit_result.am_slow = tf->GetParameter(6);

  delete h;
  delete tf;
#endif
}

#ifdef DEBUGDRAWOPTION
//
void WaveAnalysis::DrawEntry(Long64_t n)
{
  GetWave(n);
  RCCR2(n);
  Trapezoid(n);

  TGraph *g1 = new TGraph(ltra);
  for(int i=0;i<ltra;i++){
    g1->SetPoint(i, (Double_t)i, (Double_t)data_bl[i]);
  }

  cav1->cd();
  g1->Draw();

  //
  TGraph *g2 = new TGraph(ltra);
  for(int i=0;i<ltra;i++){
    g2->SetPoint(i, (Double_t)i, (Double_t)data_rccr2[i]);
  }

  cav1->cd();
  g2->SetLineColor(2);
  g2->Draw("same");

  //
  TGraph *g3 = new TGraph(ltra);
  for(int i=0;i<ltra;i++){
    g3->SetPoint(i, (Double_t)i, (Double_t)data_trapz[i]);
  }

  cav1->cd();
  g3->SetLineColor(4);
  g3->Draw("same");

  //
  FitWave(n);
}
#endif

#ifdef DEBUGDRAWOPTION
//
void WaveAnalysis::DrawMultiRCCR2()
{
  TMultiGraph *mg = new TMultiGraph();
  TGraph *g[1000];
  for(Long64_t i=0;i<1000;i++){
    GetWave(i);
    RCCR2(i);
    g[i] = new TGraph(ltra);
    for(int j=0;j<ltra;j++)
      g[i]->SetPoint(j, (Double_t)j, (Double_t)data_rccr2[j]);
    mg->Add(g[i]);
  }

  cav1->cd();
  mg->Draw("ap");
}
#endif

//
Double_t RCCR2XX(Double_t *v, UInt_t i, UInt_t window, UInt_t risetime)
{
  Double_t x = 0.;
  x = RCCR2YY(v, i, window) - RCCR2YY(v, i-risetime, window);

  return x;
}

//
Double_t RCCR2YY(Double_t *v, UInt_t i, UInt_t window)
{
  Double_t y = 0.;
  for(UInt_t j=window;j>0;j--){
    y += v[i-j];
  }
  y = y/(Double_t)window*2.;

  return y;
}

//
Double_t Integral(Double_t *v, UInt_t i, UInt_t j)
{
  Double_t s = 0.;
  for(UInt_t k=i;k<j;k++){
    s += v[k];
  }

  s /= (Double_t)(j-i);

  return s;
}

//
// p[0]: baseline
// p[1]: t0 (in points)
// p[2]: tau rc (in points)
// p[3]: tau fast (in points)
// p[4]: tau slow (in points)
// p[5]: am fast
// p[6]: am slow
Double_t Fittf(Double_t *i, Double_t *p)
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

