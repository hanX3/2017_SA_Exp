
void get_data_time_interval()
{
  gROOT->SetBatch(0);

  TFile *file_in = TFile::Open("../../../data/Ge/root/R0050_0000_decode_2.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  //clover
  Long64_t ts1;
  TTree *tr1 = (TTree*)file_in->Get("tr_Clover_ch00");
  tr1->SetBranchAddress("timestamp", &ts1);

  vector<Long64_t> v_ts_interval1;

  Long64_t ts;
  tr1->GetEntry(0);
  ts = ts1;

  for(int i=1;i<tr1->GetEntries();i++){
    tr1->GetEntry(i);

    if(i<10)
      cout << ts1 << endl;

    v_ts_interval1.push_back(ts1-ts);
    ts = ts1;
  }

  //array
  Int_t n = v_ts_interval1.size();
  Long64_t *ts_interval1;
  Int_t *ts_interval1_index;

  ts_interval1 = (Long64_t*)malloc(n*sizeof(Long64_t));
  ts_interval1_index = (Int_t*)malloc(n*sizeof(Int_t));
  for(int i=0;i<n;i++){
    ts_interval1[i] = v_ts_interval1[i];
    if(i<10){
      cout << ts_interval1[i] << endl;
    }
  }

  TMath::Sort((Int_t)n, (Long64_t*)ts_interval1, (Int_t*)ts_interval1_index, kTRUE);
  for(int i=0;i<10;i++){
    cout << ts_interval1_index[i] << " " << ts_interval1[ts_interval1_index[i]] << endl;
  }

  TH1I *h1 = new TH1I("h1", "h1", 300000, 0, 3000000000);
  TH1I *h1_new = new TH1I("h1_new", "h1_new", 10000, 0, 1000000);
  for(int i=0;i<n;i++){
    h1->Fill(ts_interval1[i]);
    h1_new->Fill(ts_interval1[i]);
  }

  TCanvas *c1 = new TCanvas("c1", "c1", 0, 0, 500, 300);
  TCanvas *c1_new = new TCanvas("c1_new", "c1_new", 600, 0, 500, 300);
  c1->cd();
  h1->Draw();

  c1_new->cd();
  h1_new->Draw(); 

  //
  //
  // csi
  Long64_t ts2;
  TTree *tr2 = (TTree*)file_in->Get("tr_CsI_ch16");
  tr2->SetBranchAddress("timestamp", &ts2);

  vector<Long64_t> v_ts_interval2;

  tr2->GetEntry(0);
  ts = ts2;

  for(int i=1;i<tr2->GetEntries();i++){
    tr2->GetEntry(i);

    if(i<10)
      cout << ts2 << endl;

    v_ts_interval2.push_back(ts2-ts);
    ts = ts2;
  }

  //array
  n = v_ts_interval2.size();
  Long64_t *ts_interval2;
  Int_t *ts_interval2_index;

  ts_interval2 = (Long64_t*)malloc(n*sizeof(Long64_t));
  ts_interval2_index = (Int_t*)malloc(n*sizeof(Int_t));
  for(int i=0;i<n;i++){
    ts_interval2[i] = v_ts_interval2[i];
    if(i<10){
      cout << ts_interval2[i] << endl;
    }
  }

  TMath::Sort((Int_t)n, (Long64_t*)ts_interval2, (Int_t*)ts_interval2_index, kTRUE);
  for(int i=0;i<10;i++){
    cout << ts_interval2_index[i] << " " << ts_interval2[ts_interval2_index[i]] << endl;
  }

  TH1I *h2 = new TH1I("h2", "h2", 300000, 0, 3000000000);
  TH1I *h2_new = new TH1I("h2_new", "h2_new", 10000, 0, 1000000);
  for(int i=0;i<n;i++){
    h2->Fill(ts_interval2[i]);
    h2_new->Fill(ts_interval2[i]);
  }

  TCanvas *c2 = new TCanvas("c2", "c2", 0, 400, 500, 300);
  TCanvas *c2_new = new TCanvas("c2_new", "c2_new", 600, 400, 500, 300);
  c2->cd();
  h2->Draw();

  c2_new->cd();
  h2_new->Draw();

  //write
  ofstream file_out1, file_out2;
  file_out1.open("ts_interval_clover.txt");
  file_out2.open("ts_interval_csi.txt");

  for(int i=0;i<h1->GetNbinsX();i++){
    file_out1 << h1->GetBinCenter(i+1) << " " << h1->GetBinContent(i+1) << endl;
  }
  for(int i=0;i<h2->GetNbinsX();i++){
    file_out2 << h2->GetBinCenter(i+1) << " " << h2->GetBinContent(i+1) << endl;
  }
  file_out1.close();
  file_out2.close();

  //ts hist
  //time intrrval 1
  tr1->GetEntry(ts_interval1_index[0]);
  ts = ts1;
  cout << "ts1 " << ts1 << endl;
  TH1I *h3_clover1 = new TH1I("h3_clover1", "h3_clover1", 2000, ts/10000.-100000, ts/10000.+100000);
  for(int i=0;i<200000;i++){
    if((ts_interval1_index[0]-100000+i)<0){
      continue;
    }
    tr1->GetEntry(ts_interval1_index[0]-100000+i);
    h3_clover1->Fill(ts1/10000.);
    // cout << ts_interval1_index[0]-10000+i << " " << ts1/10000. << endl;
  }

  TCanvas *c3_clover1 = new TCanvas("c3_clover1", "c3_clover1", 1200, 0, 500, 300);
  c3_clover1->cd();
  h3_clover1->Draw();

  //
  tr2->GetEntry(ts_interval2_index[0]);
  ts = ts2;
  cout << "ts2 " << ts2 << endl;
  TH1I *h3_csi1 = new TH1I("h3_csi1", "h3_csi1", 2000, ts/10000.-100000, ts/10000.+100000);
  for(int i=0;i<200000;i++){
    if((ts_interval2_index[0]-100000+i)<0){
      continue;
    }
    tr2->GetEntry(ts_interval2_index[0]-100000+i);
    h3_csi1->Fill(ts2/10000.);
    // cout << ts_interval2_index[0]-10000+i << " " << ts2/10000. << endl;
  }

  TCanvas *c3_csi1 = new TCanvas("c3_csi1", "c3_csi1", 1200, 400, 500, 300);
  c3_csi1->cd();
  h3_csi1->Draw();

  //ts hist
  //time intrrval 2
  tr1->GetEntry(ts_interval1_index[1]);
  ts = ts1;
  cout << "ts1 " << ts1 << endl;
  TH1I *h3_clover2 = new TH1I("h3_clover2", "h3_clover2", 2000, 0, ts/10000.+100000);
  for(int i=0;i<200000;i++){
    if((ts_interval1_index[1]-100000+i)<0){
      continue;
    }
    tr1->GetEntry(ts_interval1_index[1]-100000+i);
    h3_clover2->Fill(ts1/10000.);
    // cout << ts_interval1_index[0]-10000+i << " " << ts1/10000. << endl;
  }

  TCanvas *c3_clover2 = new TCanvas("c3_clover2", "c3_clover2", 1800, 0, 500, 300);
  c3_clover2->cd();
  h3_clover2->Draw();

  //
  tr2->GetEntry(ts_interval2_index[1]);
  ts = ts2;
  cout << "ts2 " << ts2 << endl;
  TH1I *h3_csi2 = new TH1I("h3_csi2", "h3_csi2", 2000, 0, ts/10000.+100000);
  for(int i=0;i<200000;i++){
    if((ts_interval2_index[1]-100000+i)<0){
      continue;
    }
    tr2->GetEntry(ts_interval2_index[1]-100000+i);
    h3_csi2->Fill(ts2/10000.);
    // cout << ts_interval2_index[0]-10000+i << " " << ts2/10000. << endl;
  }

  TCanvas *c3_csi2 = new TCanvas("c3_csi2", "c3_csi2", 1800, 400, 500, 300);
  c3_csi2->cd();
  h3_csi2->Draw();

  //write
  ofstream file_out[4];
  file_out[0].open("ts_stream1_clover.txt");
  file_out[1].open("ts_stream1_csi.txt");
  file_out[2].open("ts_stream2_clover.txt");
  file_out[3].open("ts_stream2_csi.txt");

  for(int i=0;i<2000;i++){
    file_out[0] << h3_clover1->GetBinCenter(i+1) << " " << h3_clover1->GetBinContent(i+1) << endl;
    file_out[1] << h3_csi1->GetBinCenter(i+1) << " " << h3_csi1->GetBinContent(i+1) << endl;
    file_out[2] << h3_clover2->GetBinCenter(i+1) << " " << h3_clover2->GetBinContent(i+1) << endl;
    file_out[3] << h3_csi2->GetBinCenter(i+1) << " " << h3_csi2->GetBinContent(i+1) << endl;
  }
  
  for(int i=0;i<4;i++){
    file_out[i].close();
  }

}