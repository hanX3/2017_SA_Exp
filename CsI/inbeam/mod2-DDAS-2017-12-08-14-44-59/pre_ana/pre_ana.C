#define pre_ana_cxx
#include "pre_ana.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void pre_ana::Loop()
{
//   In a ROOT session, you can do:
//      root> .L pre_ana.C
//      root> pre_ana t
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
   TFile *fo = new TFile("../../../../../data/CsI/inbeam/mod2-DDAS-2017-12-08-14-44-59/data_ch00_300lsb.root", "recreate");
   if(!fo){
     cout << "cannot open the file" << endl;
     return;
   }

   Short_t ch_;
   UShort_t ltra_;
   UShort_t data_[4000];
   UShort_t dt_[4000];
   Int_t nevt_;

   TTree *tr = new TTree("tr", "source data");
   tr->Branch("ch", &ch_, "ch/S");
   tr->Branch("ltra", &ltra_, "ltra/s");
   tr->Branch("data", data_, "data[ltra]/s");
   tr->Branch("dt", dt_, "dt[ltra]/s");
   tr->Branch("nevt", &nevt_, "nevt/I");

   int ii = 0;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      if (ientry%10000 == 0)
         cout << ientry << endl;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if(ch != 0) continue;
      if ((data[500]-data[0]) < 300) continue;
      
	  ltra = 1200;
      ch_ = ch;
      ltra_ = ltra;
      memcpy(data_, data, ltra*sizeof(UShort_t));
      memcpy(dt_, dt, ltra*sizeof(UShort_t));
      nevt_ = ii;

      ii++;
      tr->Fill();
   }

   fo->cd();
   tr->Write();
   fo->Close();
}
