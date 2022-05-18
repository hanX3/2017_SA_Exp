//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr 22 10:31:24 2022 by ROOT version 6.12/06
// from TTree tree/PKU Desktop Digitizer Data
// found on file: ../../../data/run_R0039.root
//////////////////////////////////////////////////////////

#ifndef pre_ana_h
#define pre_ana_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#define NUMBER 16384

// Header file for the classes stored in the TTree if any.

class pre_ana {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Short_t         ch;
   UShort_t        size;
   UInt_t          timestamp;
   Short_t         chargeshort;
   Short_t         chargelong;
   Short_t         baseline;
   UInt_t          format;
   UInt_t          format2;
   UInt_t          extras;
   Short_t         pur;
   UShort_t        dt[20000];   //[size]
   UShort_t        data[20000];   //[size]
   Int_t           nevt;

   // List of branches
   TBranch        *b_ch;   //!
   TBranch        *b_size;   //!
   TBranch        *b_timestamp;   //!
   TBranch        *b_chargeshort;   //!
   TBranch        *b_chargelong;   //!
   TBranch        *b_baseline;   //!
   TBranch        *b_format;   //!
   TBranch        *b_format2;   //!
   TBranch        *b_extras;   //!
   TBranch        *b_pur;   //!
   TBranch        *b_dt;   //!
   TBranch        *b_data;   //!
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
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../../../../../data/CsI/source/run_R0039.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../../../../../data/CsI/source/run_R0039.root");
      }
      f->GetObject("tree",tree);

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

   fChain->SetBranchAddress("ch", &ch, &b_ch);
   fChain->SetBranchAddress("size", &size, &b_size);
   fChain->SetBranchAddress("timestamp", &timestamp, &b_timestamp);
   fChain->SetBranchAddress("chargeshort", &chargeshort, &b_chargeshort);
   fChain->SetBranchAddress("chargelong", &chargelong, &b_chargelong);
   fChain->SetBranchAddress("baseline", &baseline, &b_baseline);
   fChain->SetBranchAddress("format", &format, &b_format);
   fChain->SetBranchAddress("format2", &format2, &b_format2);
   fChain->SetBranchAddress("extras", &extras, &b_extras);
   fChain->SetBranchAddress("pur", &pur, &b_pur);
   fChain->SetBranchAddress("dt", dt, &b_dt);
   fChain->SetBranchAddress("data", data, &b_data);
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
