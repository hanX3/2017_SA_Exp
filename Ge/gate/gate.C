#define gate_cxx
#include "gate.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void gate::Loop()
{
//   In a ROOT session, you can do:
//      root> .L gate.C
//      root> gate t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}

//
void gate::gate_gamma(Double_t e1, Double_t e2)
{
  TH1D *h1 = new TH1D("h1", "", 2048, 0, 2048);

  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;

  bool b_gamma = 0;
  int i_gamma = 0;
  for(Long64_t i=0; i<nentries;i++) {
    Long64_t j = LoadTree(i);
    if (j < 0) break;
    nb = fChain->GetEntry(i);   nbytes += nb;
    
    if(clover_cut_hit<2) continue;
    for(int k=0;k<clover_cut_hit;k++){
      if(clover_cut_energy[k]>e1 && clover_cut_energy[k]<e2){
        b_gamma = 1;
        i_gamma = k;
        break;
      }
    }

    if(b_gamma){
      for(int k=0;k<clover_cut_hit;k++){
        if(k==i_gamma) continue;
        h1->Fill(clover_cut_energy[k]);
      }
    }else continue;

    b_gamma = 0;
    i_gamma = 0;
  }

  h1->Draw();
}

//
void gate::gate_alpha_gamma(Double_t e1, Double_t e2)
{
  TH1D *h1 = new TH1D("h1", "", 2048, 0, 2048);

  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;

  bool b_gamma = 0;
  int i_gamma = 0;
  for(Long64_t i=0; i<nentries;i++) {
    Long64_t j = LoadTree(i);
    if (j < 0) break;
    nb = fChain->GetEntry(i);   nbytes += nb;
    
    if(clover_cut_hit<2) continue;
    if(alpha_num!=1) continue;
    for(int k=0;k<clover_cut_hit;k++){
      if(clover_cut_energy[k]>e1 && clover_cut_energy[k]<e2){
        b_gamma = 1;
        i_gamma = k;
        break;
      }
    }

    if(b_gamma){
      for(int k=0;k<clover_cut_hit;k++){
        if(k==i_gamma) continue;
        h1->Fill(clover_cut_energy[k]);
      }
    }else continue;

    b_gamma = 0;
    i_gamma = 0;
  }

  h1->Draw();
  
  ofstream fo;
  fo.open("gate_1agg_862.txt");

  for(int i=0;i<2048;i++){
    fo << i << "  " << h1->GetBinContent(i) << endl;
  }

  fo.close();
}