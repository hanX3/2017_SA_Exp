#include "Cut.h"

//
Cut::Cut(const std::string &filename_in, const std::string &filename_out)
{
  benchmark = new TBenchmark;

  file_in = TFile::Open(filename_in.c_str());
  if(file_in->IsZombie()){
    std::cout << "cannot open " << filename_in << std::endl;
  }
  tr_in = (TTree*)file_in->Get("tr");
  tr_in->SetBranchAddress("clover_hit", &clover_hit);
  tr_in->SetBranchAddress("clover_id", &clover_id);
  tr_in->SetBranchAddress("clover_energy", &clover_energy);

  tr_in->SetBranchAddress("csi_hit", &csi_hit);
  tr_in->SetBranchAddress("csi_id", &csi_id);
  tr_in->SetBranchAddress("csi_qdc1", &csi_qdc1);
  tr_in->SetBranchAddress("csi_qdc2", &csi_qdc2);
  tr_in->SetBranchAddress("csi_qdc3", &csi_qdc3);
  tr_in->SetBranchAddress("csi_qdc4", &csi_qdc4);

  total_entry = tr_in->GetEntries();
  Clear();

  memset(ax_proton, 0, sizeof(ax_proton));
  memset(ay_proton, 0, sizeof(ay_proton));
  memset(bx_proton, 0, sizeof(bx_proton));
  memset(by_proton, 0, sizeof(by_proton));
  memset(cx_proton, 0, sizeof(cx_proton));
  memset(cy_proton, 0, sizeof(cy_proton));
  memset(dx_proton, 0, sizeof(dx_proton));
  memset(dy_proton, 0, sizeof(dy_proton));

  memset(ax_alpha, 0, sizeof(ax_alpha));
  memset(ay_alpha, 0, sizeof(ay_alpha));
  memset(bx_alpha, 0, sizeof(bx_alpha));
  memset(by_alpha, 0, sizeof(by_alpha));
  memset(cx_alpha, 0, sizeof(cx_alpha));
  memset(cy_alpha, 0, sizeof(cy_alpha));
  memset(dx_alpha, 0, sizeof(dx_alpha));
  memset(dy_alpha, 0, sizeof(dy_alpha));

  for(int i=0;i<CSINUMBER;i++){
    std::ifstream fi;
    fi.open(TString::Format("./dat/csi%02d_proton.txt",i).Data());
    if(!fi){
      continue;
    }

    fi >> ax_proton[i] >> ay_proton[i];
    fi >> bx_proton[i] >> by_proton[i];
    fi >> cx_proton[i] >> cy_proton[i];
    fi >> dx_proton[i] >> dy_proton[i];

    fi.close();
  }

  for(int i=0;i<CSINUMBER;i++){
    std::ifstream fi;
    //fi.open(TString::Format("./dat/csi%02d_alpha.txt",i).Data());
    fi.open(TString::Format("./dat/csi%02d_alpha_stringent.txt",i).Data());
    if(!fi) continue;

    fi >> ax_alpha[i] >> ay_alpha[i];
    fi >> bx_alpha[i] >> by_alpha[i];
    fi >> cx_alpha[i] >> cy_alpha[i];
    fi >> dx_alpha[i] >> dy_alpha[i];

    fi.close();
  }

#ifdef DEBUGCUT
  for(int i=0;i<CSINUMBER;i++){
    std::cout << i << " " << ax_proton[i] << " " << ay_proton[i] << std::endl;
    std::cout << i << " " << bx_proton[i] << " " << by_proton[i] << std::endl;
    std::cout << i << " " << cx_proton[i] << " " << cy_proton[i] << std::endl;
    std::cout << i << " " << dx_proton[i] << " " << dy_proton[i] << std::endl;
  }
  for(int i=0;i<CSINUMBER;i++){
    std::cout << i << " " << ax_alpha[i] << " " << ay_alpha[i] << std::endl;
    std::cout << i << " " << bx_alpha[i] << " " << by_alpha[i] << std::endl;
    std::cout << i << " " << cx_alpha[i] << " " << cy_alpha[i] << std::endl;
    std::cout << i << " " << dx_alpha[i] << " " << dy_alpha[i] << std::endl;
  }
#endif

  nevt = 0;

  file_out = TFile::Open(filename_out.c_str(), "recreate");
  tr_out = new TTree("tr", "cut info");
  tr_out->Branch("clover_cut_hit", &clover_cut_hit, "clover_cut_hit/I");
  tr_out->Branch("clover_cut_id", clover_cut_id, "clover_id[clover_cut_hit]/I");
  tr_out->Branch("clover_cut_energy", clover_cut_energy, "clover_energy[clover_cut_hit]/D");

  tr_out->Branch("proton_num", &proton_num, "proton_num/I");
  tr_out->Branch("alpha_num", &alpha_num, "alpha_num/I");

  tr_out->Branch("csi_cut_hit", &csi_cut_hit, "csi_cut_hit/I");
  tr_out->Branch("csi_cut_id", csi_cut_id, "csi_cut_id[csi_cut_hit]/I");
  tr_out->Branch("csi_cut_type", csi_cut_type, "csi_cut_type[csi_cut_hit]/I");
  tr_out->Branch("csi_cut_qdc1", csi_cut_qdc1, "csi_cut_qdc1[csi_cut_hit]/D");
  tr_out->Branch("csi_cut_qdc2", csi_cut_qdc2, "csi_cut_qdc2[csi_cut_hit]/D");
  tr_out->Branch("csi_cut_qdc3", csi_cut_qdc3, "csi_cut_qdc3[csi_cut_hit]/D");
  tr_out->Branch("csi_cut_qdc4", csi_cut_qdc4, "csi_cut_qdc4[csi_cut_hit]/D");

  clover_cut_hit = 0;
  memset(clover_cut_id, 0, sizeof(clover_cut_id));
  memset(clover_cut_energy, 0, sizeof(clover_cut_energy));

  proton_num = 0;
  alpha_num = 0;

  csi_cut_hit = 0;
  memset(csi_cut_id, 0, sizeof(csi_cut_id));
  memset(csi_cut_type, 0, sizeof(csi_cut_type));
  memset(csi_cut_qdc1, 0, sizeof(csi_cut_qdc1));
  memset(csi_cut_qdc2, 0, sizeof(csi_cut_qdc2));
  memset(csi_cut_qdc3, 0, sizeof(csi_cut_qdc3));
  memset(csi_cut_qdc4, 0, sizeof(csi_cut_qdc4));
}

//
Cut::~Cut()
{

}

//
bool Cut::IsInsideProton(Int_t id, Double_t x, Double_t y)
{
  Double_t pax = ax_proton[id]-x;
  Double_t pay = ay_proton[id]-y;
  Double_t pbx = bx_proton[id]-x;
  Double_t pby = by_proton[id]-y;
  Double_t pcx = cx_proton[id]-x;
  Double_t pcy = cy_proton[id]-y;
  Double_t pdx = dx_proton[id]-x;
  Double_t pdy = dy_proton[id]-y;

  Double_t papb = pax*pby - pay*pbx;
  Double_t pbpc = pbx*pcy - pby*pcx;
  Double_t pcpd = pcx*pdy - pcy*pdx;
  Double_t pdpa = pdx*pay - pdy*pax;
 
  /*
  std::cout << id << " " << ax[id] << std::endl;
  std::cout << id << " " << ay[id] << std::endl;
  std::cout << id << " " << bx[id] << std::endl;
  std::cout << id << " " << by[id] << std::endl;
  std::cout << id << " " << cx[id] << std::endl;
  std::cout << id << " " << cy[id] << std::endl;
  std::cout << id << " " << dx[id] << std::endl;
  std::cout << id << " " << dy[id] << std::endl;

  std::cout << "pax " << pax << std::endl;
  std::cout << "pay " << pay << std::endl;
  std::cout << "pbx " << pbx << std::endl;
  std::cout << "pby " << pby << std::endl;
  std::cout << "pcx " << pcx << std::endl;
  std::cout << "pcy " << pcy << std::endl;
  std::cout << "pdx " << pdx << std::endl;
  std::cout << "pdy " << pdy << std::endl;

  std::cout << "papb " << papb << std::endl;
  std::cout << "pbpc " << pbpc << std::endl;
  std::cout << "pcpd " << pcpd << std::endl;
  std::cout << "pdpa " << pdpa << std::endl;
  */

  if(papb>=0 && pbpc>=0 && pcpd>=0 && pdpa>=0){
    return 1;
  }
  else if(papb<=0 && pbpc<=0 && pcpd<=0 && pdpa<=0){
    return 1;
  }
  else{
    return 0;
  }
}


//
bool Cut::IsInsideAlpha(Int_t id, Double_t x, Double_t y)
{
  Double_t pax = ax_alpha[id]-x;
  Double_t pay = ay_alpha[id]-y;
  Double_t pbx = bx_alpha[id]-x;
  Double_t pby = by_alpha[id]-y;
  Double_t pcx = cx_alpha[id]-x;
  Double_t pcy = cy_alpha[id]-y;
  Double_t pdx = dx_alpha[id]-x;
  Double_t pdy = dy_alpha[id]-y;

  Double_t papb = pax*pby - pay*pbx;
  Double_t pbpc = pbx*pcy - pby*pcx;
  Double_t pcpd = pcx*pdy - pcy*pdx;
  Double_t pdpa = pdx*pay - pdy*pax;
 
  /*
  std::cout << id << " " << ax[id] << std::endl;
  std::cout << id << " " << ay[id] << std::endl;
  std::cout << id << " " << bx[id] << std::endl;
  std::cout << id << " " << by[id] << std::endl;
  std::cout << id << " " << cx[id] << std::endl;
  std::cout << id << " " << cy[id] << std::endl;
  std::cout << id << " " << dx[id] << std::endl;
  std::cout << id << " " << dy[id] << std::endl;

  std::cout << "pax " << pax << std::endl;
  std::cout << "pay " << pay << std::endl;
  std::cout << "pbx " << pbx << std::endl;
  std::cout << "pby " << pby << std::endl;
  std::cout << "pcx " << pcx << std::endl;
  std::cout << "pcy " << pcy << std::endl;
  std::cout << "pdx " << pdx << std::endl;
  std::cout << "pdy " << pdy << std::endl;

  std::cout << "papb " << papb << std::endl;
  std::cout << "pbpc " << pbpc << std::endl;
  std::cout << "pcpd " << pcpd << std::endl;
  std::cout << "pdpa " << pdpa << std::endl;
  */

  if(papb>=0 && pbpc>=0 && pcpd>=0 && pdpa>=0){
    return 1;
  }
  else if(papb<=0 && pbpc<=0 && pcpd<=0 && pdpa<=0){
    return 1;
  }
  else{
    return 0;
  }
}

//
void Cut::ProcessEntry(Long64_t n)
{
  Clear();
  tr_in->GetEntry(n);

#ifdef DEBUGCUT
  if(n<10){
    std::cout << "clover_hit " << clover_hit << std::endl;
    std::cout << "clover_id " << clover_id[0] << std::endl;
    std::cout << "clover_energy " << clover_energy[0] << std::endl;
  }
#endif

  if(csi_hit==0){
    return;
  }

  Double_t xx, yy;
  for(int i=0;i<csi_hit;i++){
    if(csi_qdc1[i]>10000 || csi_qdc2[i]<6000 || csi_qdc2[i]>9000 || csi_qdc3[i]<3500 || csi_qdc3[i]>5000 || csi_qdc4[i]<10000){
      continue;
    }
    xx = 2.*csi_qdc4[i]-2.*csi_qdc1[i];
    yy = csi_qdc2[i]-2./3.*csi_qdc1[i];
    if(IsInsideProton(csi_id[i], xx, yy)){
      csi_cut_type[i] = 1;
      proton_num++;
    }
    else if(IsInsideAlpha(csi_id[i], xx, yy)){
      csi_cut_type[i] = 2;
      alpha_num++;
    }
  }

  Int_t kk = 0;
  for(int i=0;i<csi_hit;i++){
    kk += csi_cut_type[i];
  }
  if(kk==0){
    return; 
  }

  clover_cut_hit = clover_hit;
  for(int i=0;i<clover_hit;i++){
    clover_cut_id[i] = clover_id[i];
    clover_cut_energy[i] = clover_energy[i];
  }

  Int_t j = 0;
  for(int i=0;i<csi_hit;i++){
    if(csi_cut_type[i]==0){
      continue;
    }
    csi_cut_id[j] = csi_id[i];
    csi_cut_qdc1[j] = csi_qdc1[i];
    csi_cut_qdc2[j] = csi_qdc2[i];
    csi_cut_qdc3[j] = csi_qdc3[i];
    csi_cut_qdc4[j] = csi_qdc4[i];
    j++;
  }

  csi_cut_hit = j;
  tr_out->Fill();

  clover_cut_hit = 0;
  memset(clover_cut_id, 0, sizeof(clover_cut_id));
  memset(clover_cut_energy, 0, sizeof(clover_cut_energy));

  proton_num = 0;
  alpha_num = 0;

  csi_cut_hit = 0;
  memset(csi_cut_type, 0, sizeof(csi_cut_type));
  memset(csi_cut_qdc1, 0, sizeof(csi_cut_qdc1));
  memset(csi_cut_qdc2, 0, sizeof(csi_cut_qdc2));
  memset(csi_cut_qdc3, 0, sizeof(csi_cut_qdc3));
  memset(csi_cut_qdc4, 0, sizeof(csi_cut_qdc4));
}

//
void Cut::Process()
{
  benchmark->Start("cut");
  for(Long64_t i=0;i<total_entry;i++){
  //for(Long64_t i=0;i<1000;i++){
#ifdef DEBUGCUT
    if(i%100000==0){
      std::cout << i << std::endl;
    }  
#endif
    ProcessEntry(i);
  }

  file_in->Close();
  file_out->cd();
  tr_out->Write();
  file_out->Close();
  benchmark->Show("cut");
}

//
void Cut::Clear()
{
  clover_hit = 0;
  memset(clover_id, 0, sizeof(clover_id));
  memset(clover_energy, 0, sizeof(clover_energy));

  csi_hit = 0;
  memset(csi_id, 0, sizeof(csi_id));
  memset(csi_qdc1, 0, sizeof(csi_qdc1));
  memset(csi_qdc2, 0, sizeof(csi_qdc2));
  memset(csi_qdc3, 0, sizeof(csi_qdc3));
  memset(csi_qdc4, 0, sizeof(csi_qdc4));
}
