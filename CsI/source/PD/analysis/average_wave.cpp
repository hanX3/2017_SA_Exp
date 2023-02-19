
//
void average_wave()
{
  ifstream fi;
  // fi.open("gamma_csi_wave_id.txt");
  fi.open("alpha_csi_wave_id.txt");
  if(!fi){
    cout << "can not open fom.txt" << endl;
    return;
  }

  int id;
  TString str;
  Double_t data[3000];
  for(int i=0;i<3000;i++){
    data[i] = 0;
  }

  int j = 0;
  while(1){
    fi >> id;
    cout << "id " << id << endl;

    if(!fi.good()){
      break;
    }
    if(j==50) break;

    ifstream fii;
    fii.open(TString::Format("../../../../../data/CsI/source/paper_draw/pd_wave_id%06d.txt",id).Data());
    if(!fii){
      cout << "can not open " << TString::Format("../../../../../data/CsI/source/paper_draw/pd_wave_id%06d.txt",id) << endl;
      break;
    }

    int a;
    double b;
    while(1){
      fii >> a >> b;
      data[a] += b;

      if(!fii.good()) break;
    }

    j++;
    fii.close();
  }
  fi.close();

  //
  ofstream fo;
  fo.open("../../../../../data/CsI/source/paper_draw/pd_average_wave.txt");


  for(int i=0;i<3000;i++){
    data[i] /= j;
  }

  double sum = 0;
  for(int i=0;i<500;i++){
    sum += data[i];
  }
  sum /= 500.;
  
  for(int i=0;i<3000;i++){
    fo << i << " " << data[i]-sum << endl;
  }

  fo.close();
}
