//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jan 10 09:06:27 2023 by ROOT version 6.24/02
// from TTree tr/cut info
// found on file: ../../../cut.root
//////////////////////////////////////////////////////////

#ifndef gate_h
#define gate_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class gate {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           clover_cut_hit;
   Int_t           clover_cut_id[8];   //[clover_cut_hit]
   Double_t        clover_cut_energy[8];   //[clover_cut_hit]
   Int_t           proton_num;
   Int_t           alpha_num;
   Int_t           csi_cut_hit;
   Int_t           csi_cut_id[4];   //[csi_cut_hit]
   Int_t           csi_cut_type[4];   //[csi_cut_hit]
   Double_t        csi_cut_qdc1[4];   //[csi_cut_hit]
   Double_t        csi_cut_qdc2[4];   //[csi_cut_hit]
   Double_t        csi_cut_qdc3[4];   //[csi_cut_hit]
   Double_t        csi_cut_qdc4[4];   //[csi_cut_hit]

   // List of branches
   TBranch        *b_clover_cut_hit;   //!
   TBranch        *b_clover_cut_id;   //!
   TBranch        *b_clover_cut_energy;   //!
   TBranch        *b_proton_num;   //!
   TBranch        *b_alpha_num;   //!
   TBranch        *b_csi_cut_hit;   //!
   TBranch        *b_csi_cut_id;   //!
   TBranch        *b_csi_cut_type;   //!
   TBranch        *b_csi_cut_qdc1;   //!
   TBranch        *b_csi_cut_qdc2;   //!
   TBranch        *b_csi_cut_qdc3;   //!
   TBranch        *b_csi_cut_qdc4;   //!

   gate(TTree *tree=0);
   virtual ~gate();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   void gate_gamma(Double_t e1, Double_t e2);
   void gate_alpha_gamma(Double_t e1, Double_t e2);
};

#endif

#ifdef gate_cxx
gate::gate(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../../../cut.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../../../cut.root");
      }
      f->GetObject("tr",tree);

   }
   Init(tree);
}

gate::~gate()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t gate::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t gate::LoadTree(Long64_t entry)
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

void gate::Init(TTree *tree)
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

   fChain->SetBranchAddress("clover_cut_hit", &clover_cut_hit, &b_clover_cut_hit);
   fChain->SetBranchAddress("clover_cut_id", clover_cut_id, &b_clover_cut_id);
   fChain->SetBranchAddress("clover_cut_energy", clover_cut_energy, &b_clover_cut_energy);
   fChain->SetBranchAddress("proton_num", &proton_num, &b_proton_num);
   fChain->SetBranchAddress("alpha_num", &alpha_num, &b_alpha_num);
   fChain->SetBranchAddress("csi_cut_hit", &csi_cut_hit, &b_csi_cut_hit);
   fChain->SetBranchAddress("csi_cut_id", csi_cut_id, &b_csi_cut_id);
   fChain->SetBranchAddress("csi_cut_type", csi_cut_type, &b_csi_cut_type);
   fChain->SetBranchAddress("csi_cut_qdc1", csi_cut_qdc1, &b_csi_cut_qdc1);
   fChain->SetBranchAddress("csi_cut_qdc2", csi_cut_qdc2, &b_csi_cut_qdc2);
   fChain->SetBranchAddress("csi_cut_qdc3", csi_cut_qdc3, &b_csi_cut_qdc3);
   fChain->SetBranchAddress("csi_cut_qdc4", csi_cut_qdc4, &b_csi_cut_qdc4);
   Notify();
}

Bool_t gate::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void gate::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t gate::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef gate_cxx
