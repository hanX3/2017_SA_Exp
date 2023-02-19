//
void get_multi_wave()
{
  ifstream fi;
  fi.open("gamma_csi_wave_id.txt");
  // fi.open("alpha_csi_wave_id.txt");
  if(!fi){
    cout << "can not open fom.txt" << endl;
    return;
  }

  int id;
  TString str;

  gROOT->ProcessLine(".L get_wave_data.cpp");
  int i = 0;
  while(1){
    fi >> id;
    cout << "id " << id << endl;

    if(!fi.good()){
      break;
    }
    if(i==100) break;

    i++;
    str = TString::Format("get_wave_data(%d)",id);
    gROOT->ProcessLine(str.Data());
  }
}
