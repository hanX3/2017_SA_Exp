
void get_apd_tau_fast_data()
{
  gROOT->SetBatch(1);

  TFile *file_in = TFile::Open("../../../../../data/CsI/source/run_R0040/run_R0040_ana_all.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  TTree *tr = (TTree*)file_in->Get("tr");
  
  //
  TH1D *h = new TH1D("h", "", 100, 0.6, 1.6);
  tr->Draw("tau_fast>>h","","");

  ofstream file_out;
  file_out.open("../../../../../data/CsI/source/paper_draw/apd_tau_fast.txt");

  for(int i=0;i<100;i++){
    file_out << h->GetBinCenter(i+1) << "  " << h->GetBinContent(i+1) << endl;
  }

  file_out.close();
}
