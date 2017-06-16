//TrimTree_2D_Limit.h

Short_t Z1Flav, Z2Flav;
float ZZMass, ZZMassErrCorr, DMass;
float Z1Mass, Z2Mass, mreco;
int ZZflav;
int nEntry;
int event;
int chan;
float siggen, bkggen;
float DBkgKin, dbkg_kin_check;
float genHMass;
float getDbkgkinConstant(int ZZflav, float ZZMass);
float getDBkgKin(int ZZflav, float siggen, float bkggen);
float weightH, weightHBI, weight2;
float overallEventWeight;

//TString f_org="root://lxcms03//data3/Higgs/170222/ggTo2e2mu_Contin_MCFM701/ZZ4lAnalysis.root";
//TString f_org="/afs/cern.ch/user/w/wahung/work/public/Data_Sample/ggZZ_2e2mu.root";
//TString f_org = "ggH_2e2mu.root";
string chs = {"4e", "2e2mu", "4mu"};

TString f_org="root://lxcms03//data3/Higgs/170222/VBFTo4eJJ_Contin_phantom128/ZZ4lAnalysis.root";
//TString f_org="root://lxcms03//data3/Higgs/170222/VBFTo4eJJ_0PMH125Contin_phantom128/ZZ4lAnalysis.root";
TString outputFile = "cond_hist_vbf_4e_bkg.root";
TString treeAddress = "ZZTree/candTree";
//TString treeAddress = "candTree";
