
//
void get_fom()
{
  ifstream fi;
  fi.open("fom.txt");
  if(!fi){
    cout << "can not open fom.txt" << endl;
    return;
  }

  string line;
  getline(fi, line);

  int q1_stop;
  double angle;
  double a, b, c, d;


  TString str1, str2;
  gROOT->ProcessLine(".L ana_fom.cpp");
  while(1){
    fi >> q1_stop >> angle >> a >> b >> c >> d;
    cout << "q1_stop " << q1_stop << endl;
    cout << "angle " << angle << endl;
    cout << "a " << a << endl;
    cout << "b " << b << endl;
    cout << "c " << c << endl;
    cout << "d " << d << endl;

    if(!fi.good()){
      break;
    }

    str1 = TString::Format("rotate_qdc(%d, %f)",q1_stop,angle);
    str2 = TString::Format("fom(hy1, %f, %f, %f, %f)",a,b,c,d);
    gROOT->ProcessLine(str1.Data());
    gROOT->ProcessLine(str2.Data());
  }


}