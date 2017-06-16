//dbkgkin_mreco_hist.cc
//#include "TrimTree_2D_Limit.h"
#include "dbkgkin_mreco_hist_vbf_4plots.h"
using namespace std;
using namespace RooFit;


void dbkgkin_mreco_hist_vbf_4plots(){
//	TString outputFile = "cond_hist_withWeights.root";
//	TChain tc("t1");
//	tc.Add("cond_hist.root");
	vector<TFile> fs;
	TFile f1("cond_hist_vbf_bkg.root");
	TFile f2("cond_hist_vbf_sig.root");
	TFile f3("cond_hist_vbf_bsi.root");
//	fs.push_back(f1);
//	fs.push_back(f2);
//	fs.push_back(f3);
	vector<TTree*> ts(3);
	TTree* temp_t;
//	for(int i =0; i<3; i++){
	ts[0] = (TTree*) f1.Get("t1");
	ts[1] = (TTree*) f2.Get("t1");
        ts[2] = (TTree*) f3.Get("t1");
		
	vector<TH2F*> th2fs(4);
	RooRealVar mreco("ZZMass", "ZZMass", 125., 105., 140.);
	RooRealVar dbkgkin("DBkgKin", "Dbkgkin", 0.,1.);
	RooRealVar Dmass("DMass", "ZZMass/ZZMassErrCorr", 0., 1.);
	RooArgSet argset(mreco, dbkgkin);
	TH2F *hist_temp;
	
	for(int i=0; i<3; i++){
	ts[i]->SetBranchAddress("ZZMass", &zzmass);
	ts[i]->SetBranchAddress("DBkgKin", &dbkg);
	ts[i]->SetBranchAddress("overallEventWeight", &overallEventWeight);
	hist_temp = new TH2F(Form("2d_hist_unnorm_%d",i) ,Form("2d_hist_unnorm_%d",i), xbin, xmin, xmax, ybin, 0., 1.); 	
	for(int j=0; j<ts[i]->GetEntries(); j++){
		ts[i]->GetEntry(j);
		if(zzmass>140.||zzmass<105.)	continue;
		hist_temp->Fill(zzmass, dbkg, overallEventWeight);	
		}
	th2fs[i]=hist_temp;
	}
	th2fs[0]->SetNameTitle("DBkg_kin_vs_m4l_vbf_bkg","DBkg_kin_vs_m4l_vbf_bkg");
	th2fs[1]->SetNameTitle("DBkg_kin_vs_m4l_vbf_sig","DBkg_kin_vs_m4l_vbf_sig");
	th2fs[2]->SetNameTitle("DBkg_kin_vs_m4l_vbf_bsi","DBkg_kin_vs_m4l_vbf_bsi");

	hist_temp = new TH2F("DBkg_kin_vs_m4l_vbf_int", "DBkg_kin_vs_m4l_vbf_int", xbin, xmin, xmax, ybin, 0., 1.);
	int xfirst, yfirst, xlast, ylast, gbin;
	xfirst = th2fs[0]->GetXaxis()->GetFirst();
	xlast = th2fs[0]->GetXaxis()->GetLast();
	yfirst = th2fs[0]->GetYaxis()->GetFirst();
	ylast = th2fs[0]->GetYaxis()->GetLast();
	float bkgCon, sigCon, bsiCon, tempCon;
	for(int nx=xfirst; nx<xlast+1; nx++){
		for(int ny=yfirst; ny<ylast+1; ny++){
			gbin = th2fs[0]->GetBin(nx, ny);	
//			cout<<"nx: "<<nx<<"   ny: "<<ny<<"   global: "<<gbin<<endl;
			bkgCon = th2fs[0]->GetBinContent(gbin);
			sigCon = th2fs[1]->GetBinContent(gbin);
			bsiCon = th2fs[2]->GetBinContent(gbin);
			tempCon = bsiCon - bkgCon -sigCon;

//			cout<<"global bin: "<<gbin<<"   intCon: "<<tempCon<<endl;
			hist_temp->SetBinContent(gbin, tempCon);
		}
	}
	th2fs[3] = hist_temp;
		
	TFile fnew("dbkgkin_mreco_vbf_norm_hist.root", "recreate");
	TCanvas c1("","",1000, 800);
	gStyle->SetOptStat(0);
	c1.Divide(2,2);
	for(int i=0; i<th2fs.size(); i++){
	c1.cd(i+1);
//	th2fs[i]=norm_th2f(th2fs[i], 1);
	th2fs[i]->GetYaxis()->SetTitle("DBkgkin");
	th2fs[i]->GetXaxis()->SetTitle("M_{4l} (GeV)");
	th2fs[i]->Draw("colz");

	fnew.WriteTObject(th2fs[i]);
	}
	fnew.Close();
	c1.SaveAs("~/www/hist_2d_test_vbf_hist_norm.png");
	return;
}

TH2F* norm_th2f(TH2F* pdf, bool verbal =0 ){
	int nxbin, nybin;
	nxbin = pdf->GetXaxis()->GetNbins();
	nybin = pdf->GetYaxis()->GetNbins();
	int nxfirst, nxlast, nyfirst, nylast;
	nxfirst = pdf->GetXaxis()->GetFirst();
	nxlast = pdf->GetXaxis()->GetLast();
	nyfirst = pdf->GetYaxis()->GetFirst();
	nylast = pdf->GetYaxis()->GetLast();	

	TH2F* newpdf = (TH2F*) pdf->Clone();

	float newCon, oldCon, temp; 
	int gBin;
	int tracker =0;
	vector<float> weights(nxbin, 0.);
	std::vector<std::vector<float>> binContent(nxbin+1, vector<float>(nybin+1,0.));
	//first loop over x then loop over y
	for(int nx=nxfirst; nx<nxlast+1; nx++){
		for(int ny=nyfirst; ny<nylast+1; ny++){
			gBin = pdf->GetBin(nx, ny, 0);
			oldCon=pdf->GetBinContent(gBin);
			if(verbal) cout<<"Bin: ("<<nx<<" ,"<<ny<<") And gBin: "<<gBin<<"   Bin Content: "<<oldCon<<endl;
			binContent[nx][ny]=oldCon;
			weights[nx-1]+=oldCon;
			tracker++;
		}
		if(verbal)	cout<<"Weight:   "<<weights[nx-1]<<endl<<endl;
		float checknorm=0.;
		for(int ny=nyfirst; ny<nylast+1; ny++){
			gBin = pdf->GetBin(nx, ny);
			temp = binContent[nx][ny]/weights[nx-1];	
			if(temp==0)	temp=0.0001;
			checknorm+=temp;
			newpdf->SetBinContent(gBin, temp);
			if(verbal)	cout<<"Bin: ("<<nx<<" ,"<<ny<<") And gBin: "<<gBin<<"   Bin Content: "<<newpdf->GetBinContent(gBin)<<endl;
		}
		if(verbal)	cout<<checknorm<<endl<<endl;
	}

/*	for(int i=0; i<nxbin;i++){
		cout<<weights[i]<<" ";
	}
		cout<<endl;
*/

	try{
		if(tracker!=(nxbin*nybin))
		throw 111;
	}
	catch(int n){	cout<<"=========================="<<endl;
			cout<<"Tracker: "<< tracker<<"   nx*ny: "<< nxbin*nybin<<endl;
			cout<<"Error "<<n<<"!!!"<<endl;
			cout<<"=========================="<<endl;}
	TCanvas c;
	gStyle->SetOptStat(0);
	c.cd();
	newpdf->Draw("colz");
	c.Update();
	string str(pdf->GetName());
	c.SaveAs(Form("~/www/%s.png",pdf->GetName()));

//	pdf = newpdf;
//	return 1;
	return newpdf;

}
