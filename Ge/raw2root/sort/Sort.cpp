#include "Sort.h"
#include <fstream>

//
Sort::Sort(const std::string &filename_in, const std::string &filename_out)
{
  benchmark = new TBenchmark;

  file_in = TFile::Open(filename_in.c_str());
  if(file_in->IsZombie()){
    std::cout << "cannot open " << filename_in << std::endl;
  }

  file_out = TFile::Open(filename_out.c_str(), "recreate");

  memset(par, 0, sizeof(par)); 
  GetCloverCali(par);

#ifdef DEBUGSORT
  for(int i=0;i<CLOVERCHANNELS;i++){
    std::cout << par[i][0] << " " << par[i][1] << " " << par[i][2] << std::endl;
  }
#endif

}

//
Sort::~Sort()
{
  file_in->Close();
  file_out->Close();
}

//
void Sort::AddBack(Int_t c)
{
  UShort_t ch;
  UShort_t adc;
  Long64_t timestamp;

  Long64_t nentries[4];
  Long64_t total_entry = 0;
  TTree *tr[4];
  for(int i=0;i<4;i++){
    tr[i] = (TTree*)file_in->Get(TString::Format("tr_Clover_ch%02d",4*c+i).Data());
    if(!tr[i]){
#ifdef DEBUGSORT
      std::cout << "clover " << c << " empty tree" << std::endl;
#endif
      return;
    }

    total_entry += tr[i]->GetEntries();
    nentries[i] = tr[i]->GetEntries();
    tr[i]->SetBranchAddress("ch", &ch);
    tr[i]->SetBranchAddress("adc", &adc);
    tr[i]->SetBranchAddress("timestamp", &timestamp);
  }
#ifdef DEBUGSORT
  std::cout << "total_entry " << total_entry << std::endl;
#endif

  std::vector<CloverData> vec_cd;
  
  Long64_t *ts;
  ts = (Long64_t*)malloc(total_entry*sizeof(Long64_t)); 
  Int_t *ts_id;
  ts_id = (Int_t*)malloc(total_entry*sizeof(Int_t));
  
  total_entry = 0;
  for(int i=0;i<4;i++){
    for(Long64_t j=0;j<tr[i]->GetEntries();j++){
      tr[i]->GetEntry(j);
      ts[total_entry] = timestamp;
      total_entry++;
    }
  }

  TMath::Sort((Int_t)total_entry, (Long64_t*)ts, (Int_t*)ts_id, kFALSE);
#ifdef DEBUGSORT
  for(int i=0;i<10;i++){
    std::cout << ts[i] << " " << ts_id[i] << std::endl;
  }
#endif

  free(ts);
  
  Long64_t min_tag[4], max_tag[4];
  memset(min_tag, 0, sizeof(min_tag));
  memset(max_tag, 0, sizeof(max_tag));
  for(int i=0;i<4;i++){
    for(int j=0;j<i;j++)  min_tag[i] += nentries[j];
    for(int j=0;j<=i;j++)  max_tag[i] += nentries[j];
  }

  CloverData cda;
  Int_t tr_id = 0;
  Long64_t tr_entry = 0;

  memset(&cda, 0, sizeof(cda));
  for(Long64_t i=0;i<total_entry;i++){
    for(int j=0;j<4;j++){
      if(ts_id[i]>=min_tag[j] && ts_id[i]<max_tag[j]){
        tr_id = j;
        tr_entry = ts_id[i]-min_tag[j];
        break;
      }
    }
    tr[tr_id]->GetEntry(tr_entry);
    cda.id = (Int_t)ch;
    cda.energy = par[ch][0]+par[ch][1]*(Double_t)adc+par[ch][2]*(Double_t)(adc*adc);
    cda.timestamp = timestamp;

    vec_cd.push_back(cda);
  }
  free(ts_id);

#ifdef DEBUGSORT
  std::cout << "vec_cd size " << vec_cd.size() << std::endl;
  for(int i=0;i<10;i++){
    std::cout << vec_cd[i].id << " " << vec_cd[i].energy << " " << vec_cd[i].timestamp << std::endl;
  }
#endif

  Int_t id_cali;
  Double_t adc_cali;
  Long64_t timestamp_cali;
  TTree *tr_Clover_cali = new TTree(TString::Format("tr_Clover%02d_cali",c).Data(), "clover info");

  tr_Clover_cali->Branch("id_cali", &id_cali, "id_cali/I");
  tr_Clover_cali->Branch("adc_cali", &adc_cali, "adc_cali/D");
  tr_Clover_cali->Branch("timestamp_cali", &timestamp_cali, "timestamp_cali/L");

  for(Long64_t i=0;i<total_entry;i++){
    id_cali = vec_cd[i].id;
    adc_cali = vec_cd[i].energy;
    timestamp_cali = vec_cd[i].timestamp;

    tr_Clover_cali->Fill();
  }
  file_out->cd();
  tr_Clover_cali->Write();

  //addback
  std::vector<CloverData> vec_cd_addback;
  Int_t multi1, multi2, multi3, multi4, multi2_neig, multi2_diag;
  multi1 = 0; multi2 = 0; multi3 = 0; multi4 = 0;
  multi2_neig = 0; multi2_diag = 0;
  Int_t multi = 0;
  Int_t add_channel = 0;
  CloverData cda_previous;
  cda_previous.id = -1;
  cda_previous.energy = 0.;
  cda_previous.timestamp = 0;
  for(Long64_t i=0;i<(Long64_t)(vec_cd.size());i++){
    if((vec_cd[i].timestamp-cda_previous.timestamp)<ADDBACKCOINWINDOW){
      multi++;
      add_channel += (vec_cd[i].id)%4;

      cda_previous.energy += vec_cd[i].energy;
    }
    if((vec_cd[i].timestamp-cda_previous.timestamp)>=ADDBACKCOINWINDOW || i==(Long64_t)(vec_cd.size()-1)){
      if(i==0){
        cda_previous = vec_cd[i];
        continue;
      }
      multi++;
      cda_previous.id = c;
      vec_cd_addback.push_back(cda_previous);
      if(multi==1) multi1++;
      else if(multi==3) multi3++;
      else if(multi==4) multi4++;
      else{
        multi2++;
        if(add_channel==3) multi2_diag++;
        else multi2_neig++;
      }

      multi = 0;
      add_channel = 0;
      cda_previous = vec_cd[i];
    }
  }

#ifdef DEBUGSORT
  std::cout << "vec_cd_addback size " << vec_cd_addback.size() << std::endl;
  for(int i=0;i<10;i++){
    std::cout << vec_cd_addback[i].id << " " << vec_cd_addback[i].energy << " " << vec_cd_addback[i].timestamp << std::endl;
  }
#endif

#ifdef DEBUGSORT
  std::cout << "multi1 " << multi1 << std::endl;
  std::cout << "multi2 " << multi2 << std::endl;
  std::cout << "multi2_neig " << multi2_neig << std::endl;
  std::cout << "multi2_diag " << multi2_diag << std::endl;
  std::cout << "multi3 " << multi3 << std::endl;
  std::cout << "multi4 " << multi4 << std::endl;
#endif

  Int_t id_cali_addback;
  Double_t adc_cali_addback;
  Long64_t timestamp_cali_addback;
  TTree *tr_Clover_cali_addback = new TTree(TString::Format("tr_Clover%02d_cali_addback",c).Data(), "clover info addback");

  tr_Clover_cali_addback->Branch("id_cali_addback", &id_cali_addback, "id_cali_addback/I");
  tr_Clover_cali_addback->Branch("adc_cali_addback", &adc_cali_addback, "adc_cali_addback/D");
  tr_Clover_cali_addback->Branch("timestamp_cali_addback", &timestamp_cali_addback, "timestamp_cali_addback/L");

  for(Long64_t i=0;i<(Long64_t)(vec_cd_addback.size());i++){
    if(vec_cd_addback[i].energy<ENERGYMIN || vec_cd_addback[i].energy>ENERGYMAX){
      continue;
    }
    id_cali_addback = vec_cd_addback[i].id;
    adc_cali_addback = vec_cd_addback[i].energy;
    timestamp_cali_addback = vec_cd_addback[i].timestamp;

    tr_Clover_cali_addback->Fill();
  }
  file_out->cd();
  tr_Clover_cali_addback->Write();
}

//
void Sort::Process()
{
  benchmark->Start("sort");

  for(int i=0;i<CLOVERCHANNELS/4;i++){
    AddBack(i); 
  }

  benchmark->Show("sort");
}

//
void GetCloverCali(Double_t p[CLOVERCHANNELS][3])
{
  std::ifstream f;
  f.open(TString::Format("%s",CLOVERCALIDATA).Data());
  if(!f){
    std::cout << "can not open " << CLOVERCALIDATA << std::endl;
    return;
  }

  Int_t id;
  Double_t par0, par1, par2;
  Double_t chi2;

  while(true){
    f >> id >> par0 >> par1 >> par2 >> chi2;
    if(!f.good()) break;

    p[id][0] = par0;
    p[id][1] = par1;
    p[id][2] = par2;
  }
  f.close();
}
