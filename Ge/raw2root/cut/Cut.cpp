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

  memset(ax, 0, sizeof(ax));
  memset(ay, 0, sizeof(ay));
  memset(bx, 0, sizeof(bx));
  memset(by, 0, sizeof(by));
  memset(cx, 0, sizeof(cx));
  memset(cy, 0, sizeof(cy));
  memset(dx, 0, sizeof(dx));
  memset(dy, 0, sizeof(dy));

  for(int i=0;i<CSINUMBER;i++){
    std::ifstream fi;
    //fi.open(TString::Format("./dat/csi%02d_proton.txt",i).Data());
    fi.open(TString::Format("./dat/csi%02d_alpha.txt",i).Data());
    if(!fi){
      continue;
    }

    fi >> ax[i] >> ay[i];
    fi >> bx[i] >> by[i];
    fi >> cx[i] >> cy[i];
    fi >> dx[i] >> dy[i];

    fi.close();
  }

  for(int i=0;i<CSINUMBER;i++){
    std::cout << i << " " << ax[i] << " " << ay[i] << std::endl;
    std::cout << i << " " << bx[i] << " " << by[i] << std::endl;
    std::cout << i << " " << cx[i] << " " << cy[i] << std::endl;
    std::cout << i << " " << dx[i] << " " << dy[i] << std::endl;
  }

  nevt = 0;

  file_out = TFile::Open(filename_out.c_str(), "recreate");
  tr_out = new TTree("tr", "cut info");
  tr_out->Branch("clover_cut_hit", &clover_cut_hit, "clover_cut_hit/I");
  tr_out->Branch("clover_cut_id", clover_cut_id, "clover_id[clover_cut_hit]/I");
  tr_out->Branch("clover_cut_energy", clover_cut_energy, "clover_energy[clover_cut_hit]/D");

  tr_out->Branch("csi_cut_hit", &csi_cut_hit, "csi_cut_hit/I");
  tr_out->Branch("csi_cut_id", csi_cut_id, "csi_cut_id[csi_cut_hit]/I");
  tr_out->Branch("csi_cut_qdc1", csi_cut_qdc1, "csi_cut_qdc1[csi_cut_hit]/D");
  tr_out->Branch("csi_cut_qdc2", csi_cut_qdc2, "csi_cut_qdc2[csi_cut_hit]/D");
  tr_out->Branch("csi_cut_qdc3", csi_cut_qdc3, "csi_cut_qdc3[csi_cut_hit]/D");
  tr_out->Branch("csi_cut_qdc4", csi_cut_qdc4, "csi_cut_qdc4[csi_cut_hit]/D");

  clover_cut_hit = 0;
  memset(clover_cut_id, 0, sizeof(clover_cut_id));
  memset(clover_cut_energy, 0, sizeof(clover_cut_energy));

  csi_cut_hit = 0;
  memset(csi_cut_id, 0, sizeof(csi_cut_id));
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
bool Cut::IsInside(Int_t id, Double_t x, Double_t y)
{
  Double_t pax = ax[id]-x;
  Double_t pay = ay[id]-y;
  Double_t pbx = bx[id]-x;
  Double_t pby = by[id]-y;
  Double_t pcx = cx[id]-x;
  Double_t pcy = cy[id]-y;
  Double_t pdx = dx[id]-x;
  Double_t pdy = dy[id]-y;

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
  }else if(papb<=0 && pbpc<=0 && pcpd<=0 && pdpa<=0){
    return 1;
  }else{
    return 0;
  }
}

//
void Cut::ProcessEntry(Long64_t n)
{
  Clear();
  tr_in->GetEntry(n);
  if(n<10){
    std::cout << "clover_hit " << clover_hit << std::endl;
    std::cout << "clover_id " << clover_id[0] << std::endl;
    std::cout << "clover_energy " << clover_energy[0] << std::endl;
  }

  if(csi_hit==0){
    return;
  }

  Double_t xx, yy;
  Int_t k[csi_hit];
  for(int i=0;i<csi_hit;i++){
    k[i] = 0;
  }
  for(int i=0;i<csi_hit;i++){
    if(csi_qdc1[i]>10000 || csi_qdc2[i]<6000 || csi_qdc2[i]>9000 || csi_qdc3[i]<3500 || csi_qdc3[i]>5000 || csi_qdc4[i]<10000){
      continue;
    }
    xx = 2.*csi_qdc4[i]-2.*csi_qdc1[i];
    yy = csi_qdc2[i]-2./3.*csi_qdc1[i];
    if(IsInside(csi_id[i], xx, yy)){
      //std::cout << csi_id[i] << " " << xx << " " << yy << std::endl;
      k[i] = 1;
    }
  }

  Int_t kk = 0;
  for(int i=0;i<csi_hit;i++){
    kk += k[i];
  }
  if(kk==0){
    return; 
  }

  clover_cut_hit = clover_hit;
  for(int i=0;i<clover_hit;i++){
    clover_cut_id[i] = clover_id[i];
    clover_cut_energy[i] = clover_energy[i];
  }
  Int_t j=0;
  for(int i=0;i<csi_hit;i++){
    if(k[i]==0){
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

  csi_cut_hit = 0;
  memset(csi_cut_id, 0, sizeof(csi_cut_id));
  memset(csi_cut_qdc1, 0, sizeof(csi_cut_qdc1));
  memset(csi_cut_qdc2, 0, sizeof(csi_cut_qdc2));
  memset(csi_cut_qdc3, 0, sizeof(csi_cut_qdc3));
  memset(csi_cut_qdc4, 0, sizeof(csi_cut_qdc4));
}

//
void Cut::Process()
{
  for(Long64_t i=0;i<total_entry;i++){
  //for(Long64_t i=0;i<1000;i++){
    if(i%100000==0){
      std::cout << i << std::endl;
    }  
    ProcessEntry(i);
  }

  file_in->Close();
  file_out->cd();
  tr_out->Write();
  file_out->Close();
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
