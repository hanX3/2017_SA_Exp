//

void draw_multi_fom()
{
  TFile *file_in = TFile::Open("draw_th1_all_230216.root");
  if(file_in->IsZombie()){
    cout << "wrong open the file" << endl;
    return;
  }

  TTree *tr = (TTree*)file_in->Get("tr_csi_16");

  int a[10];
  TH1D *h[10];
  for(int i=0;i<10;i++){
    h[i] = new TH1D(TString::Format("h%02d",i).Data(),TString::Format("h%02d",i).Data(),100,2,12);
    a[i] = 60+20*i;

    tr->Draw(TString::Format("xx/(yy-180)>>h%02d",i).Data(),TString::Format("yy>180&&(xx/(yy+20.)>3.)&&(xx/(yy+100)<20.)&&xx/36.>%d",a[i]).Data());
  }

  h[0]->Draw();
  for(int i=1;i<6;i++){
    h[i]->SetLineColor(i+1);
    h[i]->Draw("same");
  }

}