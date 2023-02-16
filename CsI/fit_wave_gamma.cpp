
Double_t Fittf(Double_t *i, Double_t *p);

//
void fit_wave_gamma()
{
  TGraph *gr = new TGraph(2999);

  ifstream fi;
  fi.open("wave_7.dat");
  if(!fi){
    cout << "can not open the file" << endl;
    return;
  }
  
  double a, b;
  int i = 1;
  while(1){
    fi >> a >> b;
    if(!fi.good()) break;

    gr->SetPoint(i, a, b);
    i++;
  }
  fi.close();

  TCanvas *c1 = new TCanvas("c1", "");
  c1->cd();
  gr->Draw();

  TF1 *tf = new TF1("tf", Fittf, 0, 2999, 7);
  
  tf->SetParameter(0, 0);
  tf->SetParameter(1, 600);
  tf->SetParameter(2, 1540);
  tf->SetParameter(3, 20);
  tf->FixParameter(4, 850);
  tf->SetParameter(5, 30);
  tf->SetParameter(6, 30);

  gr->Fit(tf,"");

  TF1 *tf_fast = new TF1("tf", Fittf, 0, 2999, 7);
  TF1 *tf_slow = new TF1("tf", Fittf, 0, 2999, 7);

  for(int i=0;i<7;i++){
    tf_fast->SetParameter(i, tf->GetParameter(i));
    tf_slow->SetParameter(i, tf->GetParameter(i));
  }

  tf_fast->SetParameter(6, 0.);
  tf_slow->SetParameter(5, 0.);

  ofstream fo;
  fo.open("fit_result_gamma.txt");
  for(int i=0;i<30000;i++){
    fo << i*0.1 << " " << tf->Eval(i*0.1) << " " << tf_fast->Eval(i*0.1) << " " << tf_slow->Eval(i*0.1) << endl;
  }
  fo.close();
}

//
// p[0]: baseline
// p[1]: t0 (in points)
// p[2]: tau rc (in points)
// p[3]: tau fast (in points)
// p[4]: tau slow (in points)
// p[5]: am fast
// p[6]: am slow
Double_t Fittf(Double_t *i, Double_t *p)
{
  Double_t s = p[0];
  Double_t x = i[0] - p[1];
  Double_t e = exp(-x/p[2]);

  if(x<0) return s;
  else{
    s += p[5]*(1-exp(-x/p[3]))*e; 
    s += p[6]*(1-exp(-x/p[4]))*e;
    return s;
  }
}