//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jan 12 18:43:39 2023 by ROOT version 6.24/02
// from TTree tr/cut info
// found on file: ../../../cut.root
//////////////////////////////////////////////////////////

#ifndef tree2m4b_h
#define tree2m4b_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2I.h>

#include <fstream>

// Header file for the classes stored in the TTree if any.

class tree2m4b {
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

   tree2m4b(TTree *tree=0);
   virtual ~tree2m4b();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   // hanX
   void tree2th2();
   void th22m4b();
   void m4b();

public:
   TFile *fo;
   TH2I *h2_gg;
   TH2I *h2_1a_gg;

   ofstream fo_gg;
   ofstream fo_1a_gg;
};

#endif

#ifdef tree2m4b_cxx
tree2m4b::tree2m4b(TTree *tree) : fChain(0) 
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

// hanX
   fo = new TFile("matrix.root", "recreate");
   h2_gg = new TH2I("h2_gg", "gamma-gamma", 4096, 0, 2048, 4096, 0, 2048);
   h2_1a_gg = new TH2I("h2_1a_gg", "gamma-gamma", 4096, 0, 2048, 4096, 0, 2048);

   fo_gg.open("gg.m4b", std::ios::binary);
   fo_1a_gg.open("1a_gg.m4b", std::ios::binary);
}

tree2m4b::~tree2m4b()
{
// hanX
  fo->Close();
  fo_gg.close();

   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t tree2m4b::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t tree2m4b::LoadTree(Long64_t entry)
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

void tree2m4b::Init(TTree *tree)
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

Bool_t tree2m4b::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void tree2m4b::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t tree2m4b::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef tree2m4b_cxx
