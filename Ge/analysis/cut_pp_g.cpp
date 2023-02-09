#define CLOVERNUMBER 10
#define CSINUMBER 64


void cut_pp_g()
{
  gROOT->SetBatch(1);

  TFile *file_in = TFile::Open("../../../cut.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;\
    return;
  }

  TTree *tr = (TTree*)file_in->Get("tr");
  TH1D *h1 = new TH1D("h1", "", 2048, 0, 2048);

  tr->Draw("clover_cut_energy>>h1", "proton_num>1");

  ofstream file_out;
  file_out.open("pp_g.txt");
  for(int i=0;i<2048;i++){
    file_out << i << "  " << h1->GetBinContent(i) << endl;
  }

  file_out.close();
  file_in->Close();
}
