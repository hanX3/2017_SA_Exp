#include "Build.h"

//
Build::Build(const std::string &filename_in1, const std::string &filename_in2, const std::string &filename_out)
{
  benchmark = new TBenchmark;

  file_in1 = TFile::Open(filename_in1.c_str());
  file_in2 = TFile::Open(filename_in2.c_str());
  if(file_in1->IsZombie()){
    std::cout << "cannot open " << filename_in1 << std::endl;
  }
  if(file_in2->IsZombie()){
    std::cout << "cannot open " << filename_in2 << std::endl;
  }

  total_entry = 0;
  for(int i=0;i<CLOVERNUMBER;i++){
    tr_clover[i] = (TTree*)file_in2->Get(TString::Format("tr_Clover%02d_cali_addback",i).Data());
	if(tr_clover[i]){
      tr_clover[i]->SetBranchAddress("id_cali_addback", &id_clover);
      tr_clover[i]->SetBranchAddress("adc_cali_addback", &energy_clover);
      tr_clover[i]->SetBranchAddress("timestamp_cali_addback", &timestamp_clover);

	  total_entry += tr_clover[i]->GetEntries();
	  std::cout << i << " entry " << tr_clover[i]->GetEntries() << std::endl;
	}
  }

  for(int i=0;i<CSINUMBER;i++){
    tr_csi[i] = (TTree*)file_in1->Get(TString::Format("tr_CsI_ch%02d",i).Data());
	if(tr_csi[i]){
      tr_csi[i]->SetBranchAddress("ch", &id_csi);
      tr_csi[i]->SetBranchAddress("qdc", qdc_csi);
      tr_csi[i]->SetBranchAddress("timestamp", &timestamp_csi);

	  total_entry += tr_csi[i]->GetEntries();
	  std::cout << i << " entry " << tr_csi[i]->GetEntries() << std::endl;
	}
  }

  std::cout << "total_entry " << total_entry << std::endl;

  ts = (Long64_t*)malloc(total_entry*sizeof(Long64_t));
  ts_id = (Int_t*)malloc(total_entry*sizeof(Int_t));

  file_out = TFile::Open(filename_out.c_str(), "recreate");
  tr_out = new TTree("tr", "event info");
  tr_out->Branch("clover_hit", &clover_hit, "clover_hit/I");
  tr_out->Branch("clover_id", clover_id, "clover_id[clover_hit]/I");
  tr_out->Branch("clover_energy", clover_energy, "clover_energy[clover_hit]/D");
  tr_out->Branch("clover_timestamp", clover_timestamp, "clover_timestamp[clover_hit]/L");

  tr_out->Branch("csi_hit", &csi_hit, "csi_hit/I");
  tr_out->Branch("csi_id", csi_id, "csi_id[csi_hit]/I");
  tr_out->Branch("csi_qdc1", csi_qdc1, "csi_qdc1[csi_hit]/D");
  tr_out->Branch("csi_qdc2", csi_qdc2, "csi_qdc2[csi_hit]/D");
  tr_out->Branch("csi_qdc3", csi_qdc3, "csi_qdc3[csi_hit]/D");
  tr_out->Branch("csi_qdc4", csi_qdc4, "csi_qdc4[csi_hit]/D");
  tr_out->Branch("csi_timestamp", csi_timestamp, "csi_timestamp[csi_hit]/L");

}

//
Build::~Build()
{
  file_in1->Close();
  file_in2->Close();
}

//
void Build::Sort()
{
  Long64_t nentries[CLOVERNUMBER+CSINUMBER];//
  memset(nentries, 0, sizeof(nentries));

  total_entry = 0;
  for(int i=0;i<CLOVERNUMBER;i++){
    if(!tr_clover[i]) continue;
	nentries[i] = tr_clover[i]->GetEntries();
	for(int j=0;j<tr_clover[i]->GetEntries();j++){
	  if(total_entry%10000000==0) std::cout << "clover " << total_entry << std::endl;
      tr_clover[i]->GetEntry(j);
	  ts[total_entry] = timestamp_clover;
	  total_entry++;
	}
  } 

  for(int i=0;i<CSINUMBER;i++){
    if(!tr_csi[i]) continue;
	nentries[CLOVERNUMBER+i] = tr_csi[i]->GetEntries();
	for(int j=0;j<tr_csi[i]->GetEntries();j++){
	  if(total_entry%10000000==0) std::cout << "csi " << total_entry << std::endl;
	  tr_csi[i]->GetEntry(j);
	  ts[total_entry] = timestamp_csi;
	  total_entry++;
	}
  }

#ifdef DEBUGMERGE
  for(int i=0;i<(CLOVERNUMBER+CSINUMBER);i++){
    std::cout << i << " nentries " << nentries[i] << std::endl;
  } 
#endif

  std::cout << "total_entry " << total_entry << std::endl;
  TMath::Sort((Int_t)total_entry, (Long64_t*)ts, (Int_t*)ts_id, kFALSE);
#ifdef DEBUGMERGE
  for(int i=0;i<10;i++){
    std::cout << ts[i] << " " << ts_id[i] << std::endl;
  }
#endif

  memset(min_tag, 0, sizeof(min_tag));
  memset(max_tag, 0, sizeof(max_tag));
  for(int i=0;i<(CLOVERNUMBER+CSINUMBER);i++){
    for(int j=0;j<i;j++) min_tag[i] += nentries[j];
	for(int j=0;j<=i;j++)  max_tag[i] += nentries[j];
  }

#ifdef DEBUGMERGE
  for(int i=0;i<(CLOVERNUMBER+CSINUMBER);i++){
    std::cout << i << " min_tag " << min_tag[i] << std::endl;
    std::cout << i << " max_tag " << max_tag[i] << std::endl;
  }
#endif

  Int_t tr_id = 0;
  Long64_t tr_entry = 0;
  Long64_t timestamp_now;
  Long64_t i_previous, i_next;

  for(Long64_t i=0;i<total_entry;i++){
  //for(int i=0;i<10;i++){
#ifdef DEBUGMERGE
    std::cout << "ts " << ts[i] << " ts_id " << ts_id[i] << std::endl;
#endif
	if(total_entry%100000==0) std::cout << "entry " << total_entry << std::endl;
    for(int j=0;j<(CLOVERNUMBER+CSINUMBER);j++){
	  if(ts_id[i]>=min_tag[j] && ts_id[i]<max_tag[j]){
	    tr_id = j;
		tr_entry = ts_id[i]-min_tag[j];
		break;
	  }
	}
#ifdef DEBUGMERGE
    std::cout << "tr_id " << tr_id << std::endl;
    std::cout << "tr_entry " << tr_entry << std::endl;
#endif

    if(tr_id>(CLOVERNUMBER-1)){
	  continue;
    }

    tr_clover[tr_id]->GetEntry(tr_entry);
    clover_id[clover_hit] = id_clover;
    clover_energy[clover_hit] = energy_clover;
    clover_timestamp[clover_hit] = timestamp_clover;
    clover_hit++;

    timestamp_now = timestamp_clover;
    i_previous = i;
    i_next = i;

    //search foreward
    while(1){
	  i_previous--;
	  if(i_previous<0){
	    break;
	  }
      for(int j=0;j<(CLOVERNUMBER+CSINUMBER);j++){
	    if(ts_id[i_previous]>=min_tag[j] && ts_id[i_previous]<max_tag[j]){
	      tr_id = j;
		  tr_entry = ts_id[i_previous]-min_tag[j];
		  break;
	    }
	  }
    
	  if(tr_id<CLOVERNUMBER){
	    tr_clover[tr_id]->GetEntry(tr_entry);
	    if(abs(timestamp_clover-timestamp_now)>COINWINDOWCLOVER) break;

	    clover_id[clover_hit] = id_clover;
        clover_energy[clover_hit] = energy_clover;
        clover_timestamp[clover_hit] = timestamp_clover;
	    clover_hit++;
	  }else{

	    tr_csi[tr_id-CLOVERNUMBER]->GetEntry(tr_entry);
	    if(abs(timestamp_csi-timestamp_now)>COINWINDOWCSI){
		  break;
		}

	    csi_id[csi_hit] = tr_id-CLOVERNUMBER;
	    csi_qdc1[csi_hit] = qdc_csi[0];
	    csi_qdc2[csi_hit] = qdc_csi[1];
	    csi_qdc3[csi_hit] = qdc_csi[2];
	    csi_qdc4[csi_hit] = qdc_csi[3];
	    csi_timestamp[csi_hit] = timestamp_csi;
	    csi_hit++;
	  }

    }

    //search backward
    while(1){
      i_next++;
	  if(i_next>=(total_entry-1)){
	    break;
	  }
	
      for(int j=0;j<(CLOVERNUMBER+CSINUMBER);j++){
	    if(ts_id[i_next]>=min_tag[j] && ts_id[i_next]<max_tag[j]){
	      tr_id = j;
		  tr_entry = ts_id[i_next]-min_tag[j];
		  break;
	    }
	  }

	  if(tr_id<CLOVERNUMBER){
	    tr_clover[tr_id]->GetEntry(tr_entry);
	    if(abs(timestamp_clover-timestamp_now)>COINWINDOWCLOVER) break;

	    clover_id[clover_hit] = id_clover;
        clover_energy[clover_hit] = energy_clover;
        clover_timestamp[clover_hit] = timestamp_clover;
	    clover_hit++;

		i = i_next+1;
	  }else{

	    tr_csi[tr_id-CLOVERNUMBER]->GetEntry(tr_entry);
	    if(abs(timestamp_csi-timestamp_now)>COINWINDOWCSI){
          break;
	    }

	    csi_id[csi_hit] = tr_id-CLOVERNUMBER;
	    csi_qdc1[csi_hit] = qdc_csi[0];
	    csi_qdc2[csi_hit] = qdc_csi[1];
	    csi_qdc3[csi_hit] = qdc_csi[2];
	    csi_qdc4[csi_hit] = qdc_csi[3];
	    csi_timestamp[csi_hit] = timestamp_csi;
	    csi_hit++;
	  }
    }

    tr_out->Fill();
    Clear();
  }
}

//
void Build::Process()
{
  Sort();
  file_out->cd();
  tr_out->Write();
  file_out->Close();
}

//
void Build::Clear()
{
  clover_hit = 0;
  memset(clover_id, 0, sizeof(clover_id));
  memset(clover_energy, 0, sizeof(clover_energy));
  memset(clover_timestamp, 0, sizeof(clover_timestamp));

  csi_hit = 0;
  memset(csi_id, 0, sizeof(csi_id));
  memset(csi_qdc1, 0, sizeof(csi_qdc1));
  memset(csi_qdc2, 0, sizeof(csi_qdc2));
  memset(csi_qdc3, 0, sizeof(csi_qdc3));
  memset(csi_qdc4, 0, sizeof(csi_qdc4));
  memset(csi_timestamp, 0, sizeof(csi_timestamp));
}
