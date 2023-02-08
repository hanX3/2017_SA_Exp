#define tree2m4b_cxx
#include "tree2m4b.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void tree2m4b::Loop()
{
//   In a ROOT session, you can do:
//      root> .L tree2m4b.C
//      root> tree2m4b t
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
void tree2m4b::tree2th2()
{
  Long64_t n_gg = 0;
  Long64_t n_1a_gg = 0;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for(Long64_t i=0; i<nentries;i++) {
    Long64_t j = LoadTree(i);
    if (j < 0) break;
    nb = fChain->GetEntry(i);   nbytes += nb;
    
    if(clover_cut_hit>1){
      n_gg++;
      for(int ii=0;ii<clover_cut_hit;ii++){
        for(int jj=0;jj<clover_cut_hit;jj++){
          if(ii==jj) continue;
          h2_gg->Fill(clover_cut_energy[ii], clover_cut_energy[jj]);
        } 
      }
      if(alpha_num==1){
        n_1a_gg++;
        for(int ii=0;ii<clover_cut_hit;ii++){
          for(int jj=0;jj<clover_cut_hit;jj++){
            if(ii==jj) continue;
            h2_1a_gg->Fill(clover_cut_energy[ii], clover_cut_energy[jj]);
          }
        }
      }
    }
  }

  cout << "n_gg " << n_gg << endl;
  cout << "n_1a_gg " << n_1a_gg << endl;
  fo->cd();
  h2_gg->Write();
  h2_1a_gg->Write();
}

//
void tree2m4b::th22m4b()
{
  double raw;
  int data;
  for(int i=0;i<4096;i++){
    for(int j=0;j<4096;j++){
      raw = h2_gg->GetBinContent(i+1, j+1);
      data = (int)raw;
      fo_gg.write((char*)&data,sizeof(int));

      raw = h2_1a_gg->GetBinContent(i+1, j+1);
      data = (int)raw;
      fo_1a_gg.write((char*)&data,sizeof(int));
    }
  }
}

//
void tree2m4b::m4b()
{
  tree2th2();
  th22m4b();
}