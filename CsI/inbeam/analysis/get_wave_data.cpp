#define MAXLENGTH 32768

void get_wave_data(Int_t n)
{
	TFile *file_in = TFile::Open("../../../../data/CsI/inbeam/mod2-DDAS-2017-12-08-14-44-59/data_ch00_300lsb.root");
	if(file_in->IsZombie()){
		cout << "wrong open the file" << endl;\
		return;
	}

	TTree *tr = (TTree*)file_in->Get("tr");

	UShort_t ltra;
	UShort_t data[MAXLENGTH];
	UShort_t dt[MAXLENGTH];

	tr->SetBranchAddress("ltra", &ltra);
	tr->SetBranchAddress("data", data);
	tr->SetBranchAddress("dt", dt);

	tr->GetEntry(n);

	ofstream file_out;
	file_out.open(TString::Format("../../../../data/CsI/inbeam/papar_draw/wave_id%06d.txt",n).Data());

	for(int i=0;i<ltra;i++){
		file_out << i << "  " << data[i] << endl;
	}

	file_out.close();
}
