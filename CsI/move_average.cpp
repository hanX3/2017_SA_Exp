//

Double_t N = 10.;

//
void move_average()
{
  ifstream fi;
  fi.open("wave_1.dat");
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

  vector<double> v_b_ma;
  double sum = 0.;

  for(int i=0;i<v_a.size();i++){
    sum = 0.;

    if(i<(N-1)) v_b_ma.push_back(v_b[i]);
    else{
      for(int j=(i+1-N);j<(i+1);j++){
        sum += v_b[j];
      }
      v_b_ma.push_back(sum/N);
    }
  }


  ofstream fo;
  fo.open("wave_1_ma.dat");
  for(int i=0;i<v_a.size();i++){
    fo << v_a[i] << " " << v_b_ma[i]/v_b_ma[v_a.size()-1] << endl;
  }

  fi.close();
  fo.close();
}