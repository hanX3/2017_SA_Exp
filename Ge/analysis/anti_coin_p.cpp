
void anti_coin_p()
{
	TFile *file_in = TFile::Open("../../../draw_th1.root");
	if(file_in->IsZombie()){
		cout << "wrong open the file" << endl;\
		return;
	}

	TH1D *h1 = (TH1D*)file_in->Get("h1");
	TH1D *h_pgg = (TH1D*)file_in->Get("h_pgg");

	TH1D *hh = new TH1D("hh", "", 2048, 0, 2048);

	for(int i=0;i<2048;i++){
		hh->SetBinContent(i, h1->GetBinContent(i)-h_pgg->GetBinContent(i));
	}

	auto c1 = new TCanvas();
	c1->cd();
	hh->Draw();
}