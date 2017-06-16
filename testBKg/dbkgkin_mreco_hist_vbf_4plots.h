//dbkgkin_mreco_hist.h

using namespace std;
using namespace RooFit;

double xmin = 105.;
double xmax = 140.;
int xbin = (xmax-xmin)+1;
int ybin = 20;
float zzmass, m4l,dbkg, weightH, weightHBI, weight2;
float overallEventWeight;
TString outputFile = "cond_hist_vbf_2e2mu_115_250.root";
//TString outputFile = "cond_hist_VBF_sig.root";
TString pngtag = "vbf_2e2mu_115_250";
vector<TH2F*> Extract_all_hist_2d(TTree *t1);
//TH2F* extract_int(TH2F* bkgPdf, TH2F* sigPdf, TH2F* bsiPdf, const float xmin, const float xmax);
vector<int> getDim(TH2F* pdf);
//TH2F* norm_th2f(TH2F* pdf);
TH2F* norm_th2f(TH2F* pdf, bool verbal =0 );
