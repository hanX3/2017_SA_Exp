
void get_data_gg862()
{
  ifstream fi;
  fi.open("mxa_gg_862.txt");

  TH1D *h1 = new TH1D("h1", "", 8192, 0, 8192);

  int i = 0;
  int data = 0;
  while(1){
    fi >> data;
    if(!fi.good()) break;
    h1->SetBinContent(i+1, data);

    i++;
    if(i==8192) break;
  }

  h1->Rebin(2);
  // h1->Draw();

  TH1D *h2 = new TH1D("h2", "", 2048, 0, 2048);
  for(int j=0;j<2048;j++){
    h2->SetBinContent(j+1, h1->GetBinContent(j+1));
  }
  h2->Draw();

  ofstream fo;
  fo.open("./gate_gg_862.txt");

  for(int j=0;j<2048;j++){
    fo << j << " " << h2->GetBinContent(j+1) << endl;
  }

  fo.close();
}