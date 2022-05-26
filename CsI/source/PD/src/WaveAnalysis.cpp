#include "WaveAnalysis.h"

//
WaveAnalysis::WaveAnalysis(const std::string &filename_in, const std::string &filename_out)
{
  file_in = TFile::Open(filename_in.c_str());
  if(file_in->IsZombie()){
    std::cout << "can not open " << filename_in << std::endl;
  }
  tr_in = (TTree*)file_in->Get("tree");
  tr_in->SetBranchAddress("ltra", &ltra);
  tr_in->SetBranchAddress("data", data);

  memset(&cfd_par, 0, sizeof(CFD_PAR));

  cfd_par.pre_trigger = CFDPARPRITRIGGER;
  cfd_par.smooth_length = CFDPARSMOOTHLENGTH;
  cfd_par.rise_length = CFDPARRISELENGTH;
  cfd_par.threshold = CFDPARTHRESHOLD;

  trapz_par.decay_length = TRAPPARDECAYLENGTH;
  trapz_par.rise_length = TRAPPARRISELENGTH;
  trapz_par.falt_top = TRAPPARRISELENGTH;
  trapz_par.pre_trigger = TRAPPARPRITRIGGER;
  trapz_par.peak_delay = TRAPPARPEAKDELAY;
  trapz_par.peak_mean = TRAPPARPEAKMEAN;

  //
  cav = new TCanvas("cav", "", 0, 0, 480, 360);
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

  return true;
}

//
bool WaveAnalysis::ProcessEntry(Long64_t n)
{
  GetWave(n);
  RCCR2(n);

  Bool_t trigger_armed = 0;
  UInt_t ii;
  UShort_t hit_num;
  Float_t ee1 = 0., ee2 =0.;
  while(1){
    ii = 0;
	hit_num = 0;
	for(UInt_t i=10;i<(ltra-10);i++){
	  if(trigger_armed==0 && data_rccr2[i]>=cfd_par.threshold){
	    trigger_armed = 1;
		ii = i-1;
        ee1 = (data[ii-3]+data[ii-2]+data[ii-1])/3.;
	  }
	  if(trigger_armed==1 && data_rccr2[i]>0 && data_rccr2[i+1]<0){
	    trigger_armed = 0;
		ee2 = (data[i+1]+data[i+2]+data[i+3])/3.;
		if(i-ii>=5 && ee2-ee1>=cfd_par.threshold){
		  if(hit_num<4){
		    out//todo
		  }
		}
	  }
	}
  }
}

//
void WaveAnalysis::RCCR2(Long64_t n)
{
  Double_t *vv = (Double_t*)malloc(ltra*sizeof(Double_t));
  memset(data_rccr2, 0, ltra*sizeof(Short_t));

  baseline = 0.;
  for(UInt_t i=0;i<cfd_par.pre_trigger;i++){
    baseline += data[i];
  }
  baseline /= (Double_t)cfd_par.pre_trigger;

  std::cout << "baseline " << baseline << std::endl;

  for(UShort_t i=0;i<ltra;i++){
    vv[i] = data[i]-baseline; 
  }

  for(UShort_t i=(2*cfd_par.rise_length+cfd_par.smooth_length);i<ltra;i++){
    data_rccr2[i] = RCCR2XX(vv, i, cfd_par.smooth_length, cfd_par.rise_length) - RCCR2XX(vv, i-cfd_par.rise_length, cfd_par.smooth_length, cfd_par.rise_length);
  }

  free(vv);
}

//
void WaveAnalysis::Trapezoid(Long64_t n)
{
  Double_t *d = (Double_t*)malloc(ltra*sizeof(Double_t));
  Double_t *q = (Double_t*)malloc(ltra*sizeof(Double_t));
  Double_t *ss = (Double_t*)malloc(ltra*sizeof(Double_t));
  Double_t *vv = (Double_t*)malloc(ltra*sizeof(Double_t));
  
  memset(data_trapz, 0, ltra*sizeof(Short_t));

  Double_t M = trapz_par.decay_length;
  UInt_t k = trapz_par.rise_length;
  UInt_t m = trapz_par.falt_top;
  UInt_t l = k+m;

  for(UShort_t i=0;i<ltra;i++){
    vv[i] = data[i]-baseline;
  }
  
  for(UShort_t i=0;i<ltra;i++){
    if(i==0){
	  d[i]=vv[i];
	  q[i]=d[i];
	  ss[i]=q[i]+M*d[i];
	}else if(i>0 && i<k){
	  d[i]=vv[i];
	  q[i]=q[i-1]+d[i];
	  ss[i]=ss[i-1]+q[i]+M*d[i];
	}else if(i>=k && i<l){
	  d[i]=vv[i]-vv[i-k];
	  q[i]=q[i-1]+d[i];
	  ss[i]=ss[i-1]+q[i]+M*d[i];
	}else if(i>=l && i<l+k){
	  d[i]=vv[i]-vv[i-k]-vv[i-l];
	  q[i]=q[i-1]+d[i];
	  ss[i]=ss[i-1]+q[i]+M*d[i];
	}else if(i>=l+k){
	  d[i]=vv[i]-vv[i-k]-vv[i-l]+vv[i-k-l];
	  q[i]=q[i-1]+d[i];
	  ss[i]=ss[i-1]+q[i]+M*d[i];
	}
	data_trapz[i] = ss[i]/M/k;
  }

  baseline = 0.;
  for(UInt_t i=0;i<trapz_par.pre_trigger;i++){
    baseline += data_trapz[i];
  }
  baseline /= (Double_t)trapz_par.pre_trigger;
  for(UInt_t i=0;i<ltra;i++){
    data_trapz[i] -= baseline;
  }

  free(d);
  free(q);
  free(ss);
  free(vv);
}

//
void WaveAnalysis::DrawDraw(Long64_t n)
{
  GetWave(n);
  RCCR2(n);

  TGraph *g1 = new TGraph(ltra);
  for(int i=0;i<ltra;i++){
    g1->SetPoint(i, (Double_t)i, (Double_t)data[i]-baseline);
  }

  cav->cd();
  g1->Draw();

  //
  TGraph *g2 = new TGraph(ltra);
  for(int i=0;i<ltra;i++){
    g2->SetPoint(i, (Double_t)i, (Double_t)data_rccr2[i]);
  }

  cav->cd();
  g2->SetLineColor(2);
  g2->Draw("same");

  //
  Trapezoid(n);
  TGraph *g3 = new TGraph(ltra);
  for(int i=0;i<ltra;i++){
    g3->SetPoint(i, (Double_t)i, (Double_t)data_trapz[i]);
  }

  cav->cd();
  g3->SetLineColor(4);
  g3->Draw("same");
}

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

  cav->cd();
  mg->Draw("ap");
}

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
  y = y/window*2.;

  return y;
}

