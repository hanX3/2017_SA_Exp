#define CLOVERNUMBER 10
#define CSINUMBER 64


void cut_proton_gg()
{
  gROOT->SetBatch(1);

	TFile *file_in = TFile::Open("../../../cut.root");
	if(file_in->IsZombie()){
		cout << "wrong open the file" << endl;
		return;
	}

	TTree *tr = (TTree*)file_in->Get("tr");
  TH1D *h_pgg = new TH1D("h_pgg", "", 2048, 0, 2048);

  tr->Draw("clover_cut_energy>>h_pgg", "proton_num>0 && clover_cut_hit>1");

  TFile *file_out = new TFile("../../../draw_th1.root", "update");
  file_out->cd();
  h_pgg->Write();

  file_in->Close();
  file_out->Close();
}