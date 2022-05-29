//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue May 10 15:50:42 2022 by ROOT version 6.12/06
// from TTree t/south afria xia data
// found on file: ../../../data/inbeam/rawdata_mod2-DDAS-2017-12-11-04-46-05.root
//////////////////////////////////////////////////////////

#ifndef pre_ana_h
#define pre_ana_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class pre_ana {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Bool_t          pileup;
   Bool_t          outofr;
   Short_t         cid;
   Short_t         sid;
   Short_t         ch;
   UShort_t        evte;
   ULong64_t       ts;
   ULong64_t       ets;
   UShort_t        cfd;
   Bool_t          cfdft;
   UInt_t          trae;
   UInt_t          leae;
   UInt_t          gape;
   Double_t        base;
   UInt_t          qs[8];
   UShort_t        ltra;
   UShort_t        data[4000];   //[ltra]
   UShort_t        dt[4000];   //[ltra]
   Int_t           nevt;

   // List of branches
   TBranch        *b_pileup;   //!
   TBranch        *b_outofr;   //!
   TBranch        *b_cid;   //!
   TBranch        *b_sid;   //!
   TBranch        *b_ch;   //!
   TBranch        *b_evte;   //!
   TBranch        *b_ts;   //!
   TBranch        *b_ets;   //!
   TBranch        *b_cfd;   //!
   TBranch        *b_cfdft;   //!
   TBranch        *b_trae;   //!
   TBranch        *b_leae;   //!
   TBranch        *b_gape;   //!
   TBranch        *b_base;   //!
   TBranch        *b_qs;   //!
   TBranch        *b_ltra;   //!
   TBranch        *b_data;   //!
   TBranch        *b_dt;   //!
   TBranch        *b_nevt;   //!

   pre_ana(TTree *tree=0);
   virtual ~pre_ana();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef pre_ana_cxx
pre_ana::pre_ana(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../../../../../data/CsI/inbeam/mod2-DDAS-2017-12-08-14-44-59/rawdata_mod2-DDAS-2017-12-08-14-44-59.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../../../../../data/CsI/inbeam/mod2-DDAS-2017-12-08-14-44-59/rawdata_mod2-DDAS-2017-12-08-14-44-59.root");
      }
      f->GetObject("t",tree);

   }
   Init(tree);
}

pre_ana::~pre_ana()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t pre_ana::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t pre_ana::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void pre_ana::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("pileup", &pileup, &b_pileup);
   fChain->SetBranchAddress("outofr", &outofr, &b_outofr);
   fChain->SetBranchAddress("cid", &cid, &b_cid);
   fChain->SetBranchAddress("sid", &sid, &b_sid);
   fChain->SetBranchAddress("ch", &ch, &b_ch);
   fChain->SetBranchAddress("evte", &evte, &b_evte);
   fChain->SetBranchAddress("ts", &ts, &b_ts);
   fChain->SetBranchAddress("ets", &ets, &b_ets);
   fChain->SetBranchAddress("cfd", &cfd, &b_cfd);
   fChain->SetBranchAddress("cfdft", &cfdft, &b_cfdft);
   fChain->SetBranchAddress("trae", &trae, &b_trae);
   fChain->SetBranchAddress("leae", &leae, &b_leae);
   fChain->SetBranchAddress("gape", &gape, &b_gape);
   fChain->SetBranchAddress("base", &base, &b_base);
   fChain->SetBranchAddress("qs", qs, &b_qs);
   fChain->SetBranchAddress("ltra", &ltra, &b_ltra);
   fChain->SetBranchAddress("data", data, &b_data);
   fChain->SetBranchAddress("dt", dt, &b_dt);
   fChain->SetBranchAddress("nevt", &nevt, &b_nevt);
   Notify();
}

Bool_t pre_ana::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void pre_ana::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t pre_ana::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef pre_ana_cxx
