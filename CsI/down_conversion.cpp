//
void down_conversion()
{
  ifstream fi;
  fi.open("wave_4.dat");
  if(!fi){
    cout << "can not open wave_1.dat" << endl;
    return;
  }

  double a;
  double b;
  vector<double> v_a;
  vector<double> v_b;


  while(1){
    fi >> a >> b;
    if(!fi.good()) break;

    v_a.push_back(a);
    v_b.push_back(b);
  }

  cout << v_a.size() << endl;

  vector<double> v_a_dc;
  vector<double> v_b_dc;

  for(int i=0;i<v_a.size();i++){
    if(i%4==0){
      v_a_dc.push_back(v_a[i]);
      v_b_dc.push_back(v_b[i]);
    }
  }


  ofstream fo;
  fo.open("wave_4_dc.dat");
  for(int i=0;i<v_a_dc.size();i++){
    fo << v_a_dc[i] << " " << v_b_dc[i]/v_b_dc[v_a_dc.size()-1] << endl;
  }

  fi.close();
  fo.close();
}