{
//========= Macro generated from object: cut_gamma_csi/Graph
//========= by ROOT version6.24/02
   
   TCutG *cutg = new TCutG("cut_gamma_csi",7);
   cutg->SetVarX("qdc_long");
   cutg->SetVarY("qdc_short");
   cutg->SetTitle("Graph");
   cutg->SetFillStyle(1000);
   cutg->SetPoint(0,33.0651,41.3097);
   cutg->SetPoint(1,15.7705,23.1773);
   cutg->SetPoint(2,15.1133,20.3877);
   cutg->SetPoint(3,16.4689,19.4288);
   cutg->SetPoint(4,34.9137,38.1714);
   cutg->SetPoint(5,34.9959,40.6995);
   cutg->SetPoint(6,33.0651,41.3097);
   cutg->Draw("");
}
