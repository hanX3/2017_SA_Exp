#include <iostream>
#include <fstream>
#include <map>
#include <unistd.h>
#include "TFile.h"
#include "TTree.h"
#include "TVirtualFitter.h"
#include "TSpectrum.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TString.h"
#include "TGraph.h"
#include "TMath.h"
#include "TH1.h"
#include "TF1.h"

using namespace std;

//for standard source data
const Int_t PEAK_NUM_152Eu = 11;
Float_t STD_152Eu_E[PEAK_NUM_152Eu] = {121.8, 244.7, 344.3, 411.1, 444.0, 778.9, 867.4, 964.1, 1085.8, 1112.0, 1408.0};
Float_t STD_152Eu_Int[PEAK_NUM_152Eu] = {28.37, 7.53, 26.57, 2.23, 3.12, 12.97, 4.214, 14.63, 10.13, 13.54, 20.85};

const Int_t PEAK_NUM_133Ba = 5;
Float_t STD_133Ba_E[PEAK_NUM_133Ba] = {80.9, 276.4, 302.8, 356.0, 383.8};
Float_t STD_133Ba_Int[PEAK_NUM_133Ba] = {34.11, 7.147, 18.30, 61.94, 8.905};

Float_t STD_60Co_E[2] = {1173.2, 1332.5};
Float_t STD_60Co_Int[2] = {100, 100};

Float_t Intensity_152Eu = 1.0;
Float_t Intensity_133Ba = 0;
Float_t Intensity_60Co = 0;

//for calibration
Float_t threshold = 0.05; //search peak value;
Float_t sigma = 3.0; //search peak sigma
Float_t CHI_LIMIT = 20.0;
double debug=1;

//
int auto_cali(TH1D *h, Float_t *par)
{
  int i = 0;
  int npeaks = 20;
  if(!h){
    cout << "get hist error, check!" << endl;
    return -1;
  }

  //standard source data
  map<double,double> map_std_source;
  if(Intensity_152Eu != 0.0){
    for(i=0;i<PEAK_NUM_152Eu;i++){
      map_std_source.insert(pair<double,double>(STD_152Eu_E[i], STD_152Eu_Int[i]));
    }
  }
  if(Intensity_133Ba != 0.0){
    for(i=0;i<PEAK_NUM_133Ba;i++){
      map_std_source.insert(pair<double,double>(STD_133Ba_E[i], STD_133Ba_Int[i]));
    }
  }
  if(Intensity_60Co != 0.0){
    for(i=0;i<2;i++){
      map_std_source.insert(pair<double,double>(STD_60Co_E[i], STD_60Co_Int[i]));
    }
  }

  //Use TSpectrum to find the peak candidates
  TCanvas *c1 = (TCanvas*)gROOT->GetListOfCanvases()->FindObject("c1");
  if(!c1) c1 = new TCanvas("c1", "c1", 0, 0, 480, 360);
  TSpectrum *s = new TSpectrum(2*npeaks);
  Int_t nfound = s->Search(h, sigma, "", threshold);
  Double_t *xpos = s->GetPositionX();
  Double_t *ypos = s->GetPositionY();
  if(nfound>map_std_source.size()+10){
    cout << endl;
    cout << "peak found too much, please check the search parameters!" << endl;
    c1->WaitPrimitive();
    return -1;
  }
  if(nfound<2){
    cout << endl;
    cout << "peak found too less, please check the search parameters!" << endl;
    c1->WaitPrimitive();
    return -1;
  }
  if(debug==1){
    cout << "\n print peaks found :" << endl;
    s->Print();
    cout << endl;
  }

  map<double,double> map_peak_spectrum;
  TF1 *f1 = new TF1("f1", "gaus(0)+pol1(3)", 100, 8100);
  for(i=0;i<nfound;i++){
    f1->SetParameters(ypos[i], xpos[i], 2*2, 0, 0);
    h->Fit(f1, "Q", "", 0.98*xpos[i], 1.02*xpos[i]);
    xpos[i] = f1->GetParameter(1);
    ypos[i] = TMath::Sqrt(2.0*TMath::Pi())*f1->GetParameter(2)*f1->GetParameter(0);
    map_peak_spectrum.insert(pair<double,double>(xpos[i],ypos[i]));
    c1->Update();
  }

  ///cal
  map<double,double>::iterator iter_source;
  map<double,double>::iterator iter_peak;
  iter_source = map_std_source.end();
  iter_peak = map_peak_spectrum.end();
  iter_source--;
  iter_peak--;
  double cal_x[50];
  double cal_y[50];
  i = 0;
  Double_t chi = 0.;
  while(1){
    cal_y[i] = iter_source->first;
    cal_x[i] = iter_peak->first;
    if(debug == 1){
      cout << "curr i vcalue: " << i << ", x[i]: " << cal_x[i] << ", source: " << cal_y[i] << endl;
    }
    if(i>0){
      while(1){
        TGraph* gr = new TGraph(i+1, cal_x, cal_y);
        gr->Fit("pol1","Q");
        TF1 *f1 = (TF1*)gr->GetFunction("pol1");
        chi = f1->GetChisquare();
        if(debug==1)
          cout << i << ":" << chi << " " << f1->Eval(cal_x[i]) << " " << cal_y[i] << ", chi: " << chi << endl;
          if(chi>CHI_LIMIT){
            if(iter_source==map_std_source.begin()) break;
            // if the peak is not the source peak, discard this peak
            if(cal_y[i]<f1->Eval(cal_x[i])){
              if(debug==1){
                cout << "try next peak" << endl;
              }
              i--;
              iter_source++; // retry with next peak,
              break;
            }
            else{
              if(debug==1) cout << "discard source" << endl;
              iter_source--;
              cal_y[i] = iter_source->first;
            }
          }
          else break;
      }
    }
    if(i>=nfound) break;
    if(iter_source==map_std_source.begin()) break;
    if(iter_peak==map_peak_spectrum.begin()) break;

    iter_source--;
    iter_peak--;
    i++;
  }
  if(chi>CHI_LIMIT)  i--;
  if(i<2){
    cout << "can not find enough peaks for calibration." << endl;
    c1->WaitPrimitive();
    return -1;
  }
  if(debug==1){
    for(int j=0;j<i+1;j++){
      cout<<"curr ical point:"<<j<<":"<<cal_x[j]<<", "<<cal_y[j]<<endl;
    }
  }

  TCanvas *c2 = (TCanvas*)gROOT->GetListOfCanvases()->FindObject("c2");
  if(!c2) c2 = new TCanvas("c2", "c2", 490, 0, 480, 360);
  c2->cd();
  TGraph *gr = new TGraph(i+1, cal_x, cal_y);
  gr->SetTitle(h->GetTitle());
  gr->Draw("AP*");
  gr->Fit("pol2", "");
  TF1 *f2 = (TF1*)gr->GetFunction("pol2");
  par[0] = f2->GetParameter(0);
  par[1] = f2->GetParameter(1);
  par[2] = f2->GetParameter(2);
  par[3]=f2->GetChisquare();
  
  TPaveLabel *pp = new TPaveLabel(0.05,0.8,0.65,0.95, Form("%g  %g  %g",par[0],par[1],par[2]), "brNDC");
  pp->SetTextColor(kGreen+3);
  pp->SetTextSize(0.28);
  pp->Draw();
  c2->Update();

  //show peaks used
  TCanvas *c3 = (TCanvas*)gROOT->GetListOfCanvases()->FindObject("c3");
  if(!c3) c3 = new TCanvas("c3", "c3", 980, 0, 520, 600);
  c3->cd();
  c3->SetTitle(h->GetTitle());
  c3->Range(-1,0,19,30);
  stringstream ss;

  //152Eu
  TPaveText *pt0 = new TPaveText(0, 26, 8, 28);
  TText *t0  = pt0->AddText("152Eu:");
  pt0->Draw();
  TPaveText *pt1 = new TPaveText(0, 5, 8, 25);
  TText *t1[PEAK_NUM_152Eu];
  for(int k=0;k<PEAK_NUM_152Eu;k++){
    ss.str("");
    ss << STD_152Eu_E[k] << " keV";
    t1[k] = pt1->AddText(ss.str().c_str());
    for(int l=0;l<i;l++){
      if(cal_y[l]==STD_152Eu_E[k]){
        t1[k]->SetTextColor(2);
      }
    }
  }
  pt1->Draw();

  //133Ba
  TPaveText *pt2 = new TPaveText(9, 26, 17, 28);
  TText *t2  = pt2->AddText("133Ba:");
  pt2->Draw();
  TPaveText *pt3 = new TPaveText(9, 16, 17, 25);
  TText *t3[PEAK_NUM_133Ba];
  for(int k=0;k<PEAK_NUM_133Ba;k++){
    ss.str("");
    ss << STD_133Ba_E[k] << " keV";
    t3[k] = pt3->AddText(ss.str().c_str());
    for(int l=0;l<i;l++){
      if(cal_y[l]==STD_133Ba_E[k]){
        t3[k]->SetTextColor(2);
      }
    }
  }
  pt3->Draw();
  c3->Update();
  	
  return 0;
}
