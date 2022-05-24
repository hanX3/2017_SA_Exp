void test()
{
  Double_t t[4] = {10., 30., 20., 40.};
  Int_t id[4];  

  TMath::Sort(4, t, id, kFALSE);
  for(int i=0;i<4;i++){
    cout << t[i] << endl;
  }

  for(int i=0;i<4;i++){
    cout << id[i] << endl;
  }

  return;
}
