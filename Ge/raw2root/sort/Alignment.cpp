#include "Alignment.h"

#include <vector>
#include "math.h"

//
Alignment::Alignment(const std::string &filename_in, const std::string &filename_out)
{
  benchmark = new TBenchmark;

  file_in = TFile::Open(filename_in.c_str());
  for(int i=0;i<CLOVERCHANNELS;i++){
    tr_Clover[i] = (TTree*)file_in->Get(TString::Format("tr_Clover_ch%02d",i).Data());
    if(!tr_Clover[i]){
      continue;
    }
    tr_Clover[i]->SetBranchAddress("ch", &ch_Clover);
    tr_Clover[i]->SetBranchAddress("timestamp", &timestamp_Clover);
  }

  for(int i=0;i<CSICHANNELS;i++){
    tr_CsI[i] = (TTree*)file_in->Get(TString::Format("tr_CsI_ch%02d",i).Data());
    if(!tr_CsI[i]){
      continue;
    }
    tr_CsI[i]->SetBranchAddress("ch", &ch_CsI);
    tr_CsI[i]->SetBranchAddress("qdc", qdc_CsI);
    tr_CsI[i]->SetBranchAddress("timestamp", &timestamp_CsI);
  }

  ch_Clover = 0;
  timestamp_Clover = 0;
  ch_CsI = 0;
  timestamp_CsI = 0;

  file_out = TFile::Open(filename_out.c_str(), "recreate");
  for(int i=0;i<CLOVERCHANNELS;i++){
    if(tr_Clover[i]){
      id_clover = i;
       break;
    }else continue;
  }
  for(int i=0;i<CSICHANNELS;i++){
    if(tr_CsI[i]){
      id_csi = i;
      break;
    }else continue;
  }

#ifdef DEBUGALIGNMENT
  std::cout << "id_clover " << id_clover << std::endl;
  std::cout << "id_csi " << id_csi << std::endl;
#endif

  for(int i=id_clover+1;i<CLOVERCHANNELS;i++){
    h1[i-id_clover-1] = new TH1D(TString::Format("h_clover_ch%02d_clover_ch%02d",id_clover,i).Data(), TString::Format("h_clover_ch%02d_clover_ch%02d",id_clover,i).Data(), 2*ALIGNMENTWINDOW, -ALIGNMENTWINDOW, ALIGNMENTWINDOW);
  }
  for(int i=id_csi+1;i<CSICHANNELS;i++){
    h2[i-id_csi-1] = new TH1D(TString::Format("h_csi_ch%02d_csi_ch%02d",id_csi,i).Data(), TString::Format("h_csi_ch%02d_csi_ch%02d",id_csi,i).Data(), 2*ALIGNMENTWINDOW, -ALIGNMENTWINDOW, ALIGNMENTWINDOW);
  }
  for(int i=id_csi;i<CSICHANNELS;i++){
    hh[i-id_csi] = new TH1D(TString::Format("h_clover_ch%02d_csi_ch%02d",id_clover,i).Data(), TString::Format("h_clover_ch%02d_csi_ch%02d",id_clover,i).Data(), 2*ALIGNMENTWINDOW, -ALIGNMENTWINDOW, ALIGNMENTWINDOW);
  }
}

//
Alignment::~Alignment()
{
  file_in->Close();
  file_out->Close();
}

//
void Alignment::GetCloverTimestampVector(TTree *tr, std::vector<Long64_t> &vec)
{
  for(Long64_t i=0;i<tr->GetEntries();i++){
    tr->GetEntry(i);
#ifdef DEBUGALIGNMENT
    std::cout << "ch " << ch_Clover << std::endl;
    std::cout << "timestamp " << timestamp_Clover << std::endl;
    std::cout << "ts " << ts << std::endl;
#endif

    vec.push_back(timestamp_Clover);
  }
}

//
void Alignment::GetCsITimestampVector(TTree *tr, std::vector<Long64_t> &vec)
{
  for(Long64_t i=0;i<tr->GetEntries();i++){
    tr->GetEntry(i);
#ifdef DEBUGALIGNMENT
    std::cout << "ch " << ch_CsI << std::endl;
    std::cout << "timestamp " << timestamp_CsI << std::endl;
    std::cout << "ts " << ts << std::endl;
#endif

    // if(qdc_CsI[0]>10000 || qdc_CsI[1]<6000 || qdc_CsI[1]>9000 || qdc_CsI[2]<3500 || qdc_CsI[2]>5000 || qdc_CsI[3]<10000){
    // if(qdc_CsI[0]<9000){
    //   continue;
    // }
    vec.push_back(timestamp_CsI);
  }
}

//
void Alignment::Process()
{
  std::vector<Long64_t> v1;
  std::vector<Long64_t> v2;
  
  v1.clear();
  v2.clear();

  Long64_t ts1, ts2;
  Long64_t j = 0;
  Long64_t k = 0;

  //clover vs. clover
  std::cout << "clover vs. clover" << std::endl;
  GetCloverTimestampVector(tr_Clover[id_clover], v1);
  for(int i=id_clover+1;i<CLOVERCHANNELS;i++){
    v2.clear();
    j = 0;
    k = 0;

    if(!tr_Clover[i]) continue;
    GetCloverTimestampVector(tr_Clover[i], v2);
    while(true){
      if(j==(Long64_t)v1.size() || k==(Long64_t)v2.size())  break;
      ts1 = v1[j];
      while(true){
        if(k==(Long64_t)v2.size())  break;
        ts2 = v2[k];
#ifdef DEBUGALIGNMENT
  std::cout << j << " ts1 " << ts1 << std::endl;
  std::cout << k << " ts2 " << ts2 << std::endl;
  std::cout << "ts1-ts2 " << ts1-ts2 << std::endl;
#endif
        if(abs(ts1-ts2)<ALIGNMENTWINDOW){
#ifdef DEBUGALIGNMENT
  std::cout << "1" << std::endl;
#endif
          h1[i-id_clover-1]->Fill(ts1-ts2);
          k++;
        }
        else if((ts1-ts2)>ALIGNMENTWINDOW){
#ifdef DEBUGALIGNMENT
  std::cout << "2" << std::endl;
#endif
          k++;
        }
        else{
#ifdef DEBUGALIGNMENT
    std::cout << "3" << std::endl;
#endif
          j++;
          break;
        }
      }
    }
  }

  //csi vs. csi
  std::cout << "csi vs. csi" << std::endl;
  GetCsITimestampVector(tr_CsI[id_csi], v1);
  for(int i=id_csi+1;i<CSICHANNELS;i++){
    v2.clear();
    j = 0;
    k = 0;

    if(!tr_CsI[i]) continue;
    GetCsITimestampVector(tr_CsI[i], v2);
    while(true){
      if(j==(Long64_t)v1.size() || k==(Long64_t)v2.size())  break;
      ts1 = v1[j];
      while(true){
        if(k==(Long64_t)v2.size())  break;
        ts2 = v2[k];
#ifdef DEBUGALIGNMENT
  std::cout << j << " ts1 " << ts1 << std::endl;
  std::cout << k << " ts2 " << ts2 << std::endl;
  std::cout << "ts1-ts2 " << ts1-ts2 << std::endl;
#endif
        if(abs(ts1-ts2)<ALIGNMENTWINDOW){
#ifdef DEBUGALIGNMENT
  std::cout << "1" << std::endl;
#endif
          h2[i-id_csi-1]->Fill(ts1-ts2);
          k++;
        }
        else if((ts1-ts2)>ALIGNMENTWINDOW){
#ifdef DEBUGALIGNMENT
  std::cout << "2" << std::endl;
#endif
          k++;
        }
        else{
#ifdef DEBUGALIGNMENT
  std::cout << "3" << std::endl;
#endif
          j++;
          break;
        }
      }
    }
  }
  
  //clover vs. csi
  std::cout << "clover vs. csi" << std::endl;
  GetCloverTimestampVector(tr_Clover[id_clover], v1);
  for(int i=id_csi;i<CSICHANNELS;i++){
    v2.clear();
    j = 0;
    k = 0;

    if(!tr_CsI[i]) continue;
    GetCsITimestampVector(tr_CsI[i], v2);
    while(true){
    if(j==(Long64_t)v1.size() || k==(Long64_t)v2.size())  break;
    ts1 = v1[j];
      while(true){
        if(k==(Long64_t)v2.size())  break;
        ts2 = v2[k];
#ifdef DEBUGALIGNMENT
  std::cout << j << " ts1 " << ts1 << std::endl;
  std::cout << k << " ts2 " << ts2 << std::endl;
  std::cout << "ts1-ts2 " << ts1-ts2 << std::endl;
#endif 
        if(abs(ts1-ts2)<ALIGNMENTWINDOW){
#ifdef DEBUGALIGNMENT
  std::cout << "1" << std::endl;
#endif
          hh[i-id_csi]->Fill(ts1-ts2);
          k++;
        }
        else if((ts1-ts2)>ALIGNMENTWINDOW){
#ifdef DEBUGALIGNMENT
  std::cout << "2" << std::endl;
#endif
          k++;
        }
        else{
#ifdef DEBUGALIGNMENT
  std::cout << "3" << std::endl;
#endif
          j++;
          break;
        }
      }
    }
  }

  file_out->cd();
  for(int i=id_clover+1;i<CLOVERCHANNELS;i++){
    if(!tr_Clover[i]) continue;
    h1[i-id_clover-1]->Write();
  }
  for(int i=id_csi+1;i<CSICHANNELS;i++){
    if(!tr_CsI[i]) continue;
    h2[i-id_csi-1]->Write();
  }
  for(int i=id_csi;i<CSICHANNELS;i++){
    if(!tr_CsI[i]) continue;
    hh[i-id_csi]->Write();
  }
}

//
void Alignment::GetCloverTimeInterval()
{
  std::vector<Long64_t> v;

  Long64_t ti = 0;
  Long64_t ti_max = 0;
  Long64_t k = 0;
  Long64_t ts = 0;
  for(int i=id_clover;i<CLOVERCHANNELS;i++){
    if(!tr_Clover[i]) continue;
    v.clear();
    GetCloverTimestampVector(tr_Clover[i], v);

    hi_clover[i-id_clover] = new TH1D(TString::Format("hi_clover_ch%02d",i).Data(), TString::Format("hi_clover_ch%02d",i).Data(), 1000000, 0, 1000000000);

    ti = 0;
    ti_max = 0;
    k = 0;
    ts = 0;
    for(Long64_t j=1;j<(Long64_t)v.size();j++){
      ti = v[j]-v[j-1];
      hi_clover[i]->Fill(ti);
      if(ti > ti_max){
        ti_max = ti;
        k = j;
        ts = v[j];
      }
    }
    std::cout << i << " " << k << " " << ti_max << " " << ts << std::endl;
  }

  file_out->cd();
  for(int i=id_clover;i<CLOVERCHANNELS;i++){
    if(!tr_Clover[i]) continue;
      hi_clover[i-id_clover]->Write();
  }
}

//
void Alignment::GetCsITimeInterval()
{
  std::vector<Long64_t> v;

  Long64_t ti = 0;
  Long64_t ti_max = 0;
  Long64_t k = 0;
  Long64_t ts = 0;
  for(int i=id_csi;i<CSICHANNELS;i++){
    if(!tr_CsI[i]) continue;
    v.clear();
    GetCsITimestampVector(tr_CsI[i], v);

    hi_csi[i-id_csi] = new TH1D(TString::Format("hi_csi_ch%02d",i).Data(), TString::Format("hi_csi_ch%02d",i).Data(), 1000000, 0, 1000000000);

    ti = 0;
    ti_max = 0;
    k = 0;
    ts = 0;
    for(Long64_t j=1;j<(Long64_t)v.size();j++){
      ti = v[j]-v[j-1];
      hi_csi[i-id_csi]->Fill(ti);
      if(ti > ti_max){
        ti_max = ti;
        k = j;
        ts = v[j];
      }
    }
    std::cout << i << " " << k << " " << ti_max << " " << ts << std::endl;
  }

  for(int i=id_csi;i<CSICHANNELS;i++){
    if(!tr_CsI[i]) continue;
      hi_csi[i-id_csi]->Write();
  }
}
