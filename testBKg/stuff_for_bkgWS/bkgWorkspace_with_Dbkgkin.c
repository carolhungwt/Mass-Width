//#include "ProcessNormalization.cc"
//#include "ProcessNormalization.h"
//#include "RooTFnBinding.h"
//#include "VerticalInterpPdf.cc+"
#include <string>
using namespace RooFit;
//using namespace std;

// highmass = 0 onshell
// highmass = 1 offshell
// highmass = 2 search 
//
//
// cate_vbf =0 ggH
// cate_vbf =1 VBF
// cate_vbf =2 RSE
// cate_vbf =3 TLE
// cate_vbf =4 inclusive 
//

void dosomething(TString chan="2e2mu", int cate_vbf=1, int highmass=0){
// double lumi = 9.235;
 double lumi = 35.867;
// double lumi = 10;

	double parzx_all[5][6]={
		//4e
//		0.00399059,151.251,36.6082,7.06054,-0.00497032,
//		//4mu
//		1.84404,134.765,22.2293,0.,0.,
//		//2e2mu
//		2.57065,143.701,24.071,0.,0.,
//		4e
		1,141.9,21.3,0,0,0,
		//4mu
		1,130.4,15.6,0,0,0,
		//2e2mu
		0.45,131.1,18.1,0.55,133.8,18.9,
		//4e RSE
		0.298623315141,238.804039513,38.5525989967,4.83114145422,-0.0097489713697,0,
		//2e2mu RSE
		0.000171310428786,209.221006175,26.5346636174,11.193766044,-0.00296426709129,0
	};
	double parzx[6]={0.};

	//if(cate_vbf!=2){
	if (chan=="4e") 	{
		for (int i=0;i<6;i++){parzx[i]=parzx_all[0][i];}
		
	} 
	if (chan=="4mu")  {
		for (int i=0;i<6;i++){parzx[i]=parzx_all[1][i];}
	}
	if (chan=="2e2mu") {
		for (int i=0;i<6;i++){parzx[i]=parzx_all[2][i];}
	} 
	//}
	//else{
	//if (chan=="2e2mu") {
	//	for (int i=0;i<6;i++){parzx[i]=parzx_all[4][i];}
	//} 
	//if (chan=="4e") {
	//	for (int i=0;i<6;i++){parzx[i]=parzx_all[3][i];}
	//} 
	//}
	ofstream yields("yields.txt",std::fstream::app);



	RooWorkspace w("w");
  double recolowarr[3]={105.,104,104.5};
  double recohigharr[3]={140.,1604.,3004.5};
  const int reconbinsarr[3]={35,750,2900};

//	const double low_reco=120.5;
//	const double high_reco=3020.5;
//	const int nbins_reco=2900;

	const double low_reco=recolowarr[highmass];
	const double high_reco=recohigharr[highmass];
	const int nbins_reco=reconbinsarr[highmass];
	


	RooRealVar* mreco= new RooRealVar("mreco","M_{ZZ} (GeV)",125,low_reco,high_reco);
	RooRealVar* dbkg_kin = new RooRealVar("dbkg_kin", "Dbkgkin", 0.5, 0.,1.);
 	 RooPlot* frame= mreco->frame(low_reco,high_reco) ;
//if(highmass==2)
	mreco->setBins(nbins_reco);
	dbkg_kin->setBins(10);
//qqzz pdf
	//TChain *tqqzz= new TChain("ZZTree/candTree");
	
	// simplify: for ggF, this is qqZZ bkg, for VBF, this should be ggZZ bkg
	// simplify: if no systematics wanted, comment out _up and _dn related stuff
	TString treename[5]={"","","_rse","_tle",""};
	TChain *tqqzz= new TChain("SelectedTree"+treename[cate_vbf]);

	TH1F *hqqzz= new TH1F ("hqqzz","",nbins_reco,low_reco,high_reco);
	TH1F *hqqzz_up= new TH1F ("hqqzz_up","",nbins_reco,low_reco,high_reco);
	TH1F *hqqzz_dn= new TH1F ("hqqzz_dn","",nbins_reco,low_reco,high_reco);

	//tqqzz->Add("./ZZ4lAnalysis.root");
	tqqzz->Add("qqzz_80.root");

		int channum=1;
		if(chan=="2e2mu")
			channum=3;
		else if(chan=="4e")
			channum=2;
		else
			channum=1;

		//tqqzz->Draw("ZZMass>>hqqzz",Form("weight*(chan==%d&&vbfcate==%d)",channum,cate_vbf));
		//tqqzz->Draw("ZZMass>>hqqzz_up",Form("weight_up*(chan==%d&&vbfcate==%d)",channum,cate_vbf));
		//tqqzz->Draw("ZZMass>>hqqzz_dn",Form("weight_dn*(chan==%d&&vbfcate==%d)",channum,cate_vbf));
		

    RooRealVar* wt= new RooRealVar("weight","wt",125,0.000,1000.);
    RooRealVar* wt_up= new RooRealVar("weight_up","wt_up",125,0.000,1000.);
    RooRealVar* wt_dn= new RooRealVar("weight_dn","wt_dn",125,0.000,1000.);


    float ZZMass;
    float weight, weight_up, weight_dn;
		int channel;
    tqqzz->SetBranchAddress("mreco",&ZZMass);
		 if(cate_vbf==1){
    tqqzz->SetBranchAddress("weight_vbf",&weight);
    tqqzz->SetBranchAddress("weight_vbf_up",&weight_up);
    tqqzz->SetBranchAddress("weight_vbf_dn",&weight_dn);
		tqqzz->Draw("mreco>>hqqzz",Form("weight_vbf*(chan==%d)",channum));
		tqqzz->Draw("mreco>>hqqzz_dn",Form("weight_vbf_dn*(chan==%d)",channum));
		tqqzz->Draw("mreco>>hqqzz_up",Form("weight_vbf_up*(chan==%d)",channum));
		 }
		 else if(cate_vbf!=4){
    tqqzz->SetBranchAddress("weight",&weight);
    tqqzz->SetBranchAddress("weight_up",&weight_up);
    tqqzz->SetBranchAddress("weight_dn",&weight_dn);
		tqqzz->Draw("mreco>>hqqzz",Form("weight*(chan==%d)",channum));
		tqqzz->Draw("mreco>>hqqzz_dn",Form("weight_dn*(chan==%d)",channum));
		tqqzz->Draw("mreco>>hqqzz_up",Form("weight_up*(chan==%d)",channum));
		 }
		 else {
    tqqzz->SetBranchAddress("weight_inc",&weight);
    tqqzz->SetBranchAddress("weight_inc_up",&weight_up);
    tqqzz->SetBranchAddress("weight_inc_dn",&weight_dn);
		tqqzz->Draw("mreco>>hqqzz",Form("weight_inc*(chan==%d)",channum));
		tqqzz->Draw("mreco>>hqqzz_dn",Form("weight_inc_dn*(chan==%d)",channum));
		tqqzz->Draw("mreco>>hqqzz_up",Form("weight_inc_up*(chan==%d)",channum));
		 }
    tqqzz->SetBranchAddress("chan",&channel);
		

//	hqqzz->Draw();
//	hqqzz_up->Draw("same");
//	hqqzz_dn->Draw("same");
if(cate_vbf!=2){
	cout<<chan<<"\t"<<cate_vbf<<"\t"<<hqqzz->Integral()*lumi*1000./6.635892e+06<<endl;
	cout<<chan<<"\t"<<cate_vbf<<"\t"<<hqqzz_up->Integral()*lumi*1000./6.635892e+06<<endl;
	cout<<chan<<"\t"<<cate_vbf<<"\t"<<hqqzz_dn->Integral()*lumi*1000./6.635892e+06<<endl;
}
else{
	cout<<chan<<"\t"<<cate_vbf<<"\t"<<1.6*hqqzz->Integral()*lumi*1000./6.635892e+06<<endl;
	cout<<chan<<"\t"<<cate_vbf<<"\t"<<1.6*hqqzz_up->Integral()*lumi*1000./6.635892e+06<<endl;
	cout<<chan<<"\t"<<cate_vbf<<"\t"<<1.6*hqqzz_dn->Integral()*lumi*1000./6.635892e+06<<endl;
}
	
	//cout<<chan<<"\t"<<cate_vbf<<"\t"<<hqqzz->Integral()*10*1000./3.853167e+06<<endl;
	//cout<<chan<<"\t"<<cate_vbf<<"\t"<<hqqzz_up->Integral()*10*1000./3.853167e+06<<endl;
	//cout<<chan<<"\t"<<cate_vbf<<"\t"<<hqqzz_dn->Integral()*10*1000./3.853167e+06<<endl;
	//return;
    TTree *cuttree =new TTree("SelectedTree","SelectedTree");
     cuttree->Branch("mreco",&ZZMass,"mreco/F");
     cuttree->Branch("weight",&weight,"weight/F");
     cuttree->Branch("weight_up",&weight_up,"weight_up/F");
     cuttree->Branch("weight_dn",&weight_dn,"weight_dn/F");

    for(int i =0;i<tqqzz->GetEntries();i++){
        tqqzz->GetEntry(i);
				if(channel==channum){
        cuttree->Fill();
				}
    }


		double rho =1;
		if(highmass==0)
			rho=4;
//******************* qqzz and Template **************************
    RooArgSet hist_argset;
    hist_argset.add(*mreco);
    hist_argset.add(*dbkg_kin);
    TFile *fqqzz = new TFile("qqzz_template.root","read");
    TH2F* th2f_qqzz = (TH2F*) fqqzz->Get("qqzz_template");
    TH2F* th2f_qqzz_up = (TH2F*) fqqzz->Get("qqzz_template_up");
    TH2F* th2f_qqzz_dn = (TH2F*) fqqzz->Get("qqzz_template_dn");
    

    cout<<"Background expectatioon: "<<endl;
    RooDataSet bkgdata ("bkgdata"+chan+Form("_%d",cate_vbf),"",cuttree,RooArgSet(*mreco,*wt),"weight");
    RooKeysPdf qqzzkeyspdf("qqzzkeyspdf"+chan+Form("_%d",cate_vbf),"",*mreco,bkgdata,RooKeysPdf::MirrorBoth,rho);
//    TH1* qqzz_th1 = (TH1*) qqzzkeyspdf.createHistogram("mreco");
//    RooDataHist qqzz_th1_datahist("qqzz_th1_datahist","qqzz_th1_datahist",RooArgSet(*mreco),qqzz_th1);
//    RooHistPdf qqzz_th1_histpdf("qqzz_th1_histpdf","qqzz_th1_histpdf", RooArgSet(*mreco), qqzz_th1_datahist);
    RooDataHist qqzzdatahist("qqzz_datahist"+chan+Form("_%d",cate_vbf),"qqzz_datahist", hist_argset, th2f_qqzz);
    RooHistPdf qqzzhistpdf("qqzz_histpdf"+chan+Form("_%d",cate_vbf),"qqzz_histpdf", hist_argset, qqzzdatahist);
    RooProdPdf qqzzpdf("bkg_qqzz","", qqzzkeyspdf, Conditional(qqzzhistpdf,*dbkg_kin));

    RooDataSet bkgdata_up("bkgdata_up"+chan+Form("_%d",cate_vbf),"",cuttree,RooArgSet(*mreco,*wt_up),"weight_up");
    RooKeysPdf qqzzkeyspdf_up("qqzzkeyspdf_up"+chan+Form("_%d",cate_vbf),"",*mreco,bkgdata_up,RooKeysPdf::MirrorLeft,rho);
//    TH1* qqzz_th1_up = (TH1*) qqzzkeyspdf_up.createHistogram("mreco");
//    RooDataHist qqzz_th1_datahist_up("qqzz_th1_datahist_up","qqzz_th1_datahist_up",RooArgSet(*mreco),qqzz_th1_up);
//    RooHistPdf qqzz_th1_histpdf_up("qqzz_th1_histpdf_up","qqzz_th1_histpdf_up", RooArgSet(*mreco), qqzz_th1_datahist_up);
    RooDataHist qqzzdatahist_up("qqzz_datahist_up"+chan+Form("_%d",cate_vbf),"qqzz_datahist", hist_argset, th2f_qqzz_up);
    RooHistPdf qqzzhistpdf_up("qqzz_histpdf_up"+chan+Form("_%d",cate_vbf),"qqzz_histpdf", hist_argset, qqzzdatahist_up);
    RooProdPdf qqzzpdf_up("bkg_qqzz_EWKscale_VVUp","", qqzzkeyspdf_up, Conditional(qqzzhistpdf_up,*dbkg_kin));


    RooDataSet bkgdata_dn("bkgdata_dn"+chan+Form("_%d",cate_vbf),"",cuttree,RooArgSet(*mreco,*wt_dn),"weight_dn");
    RooKeysPdf qqzzkeyspdf_dn("qqzzkeyspdf_dn"+chan+Form("_%d",cate_vbf),"",*mreco,bkgdata_dn,RooKeysPdf::MirrorLeft);
//    TH1* qqzz_th1_dn = (TH1*) qqzzkeyspdf_dn.createHistogram("mreco");
//    RooDataHist qqzz_th1_datahist_dn("qqzz_th1_datahist_dn","qqzz_th1_datahist_dn",RooArgSet(*mreco),qqzz_th1_dn);
//    RooHistPdf qqzz_th1_histpdf_dn("qqzz_th1_histpdf_dn","qqzz_th1_histpdf_dn", RooArgSet(*mreco), qqzz_th1_datahist_dn);
    RooDataHist qqzzdatahist_dn("qqzz_datahist_dn"+chan+Form("_%d",cate_vbf),"qqzz_datahist", hist_argset, th2f_qqzz_dn);
    RooHistPdf qqzzhistpdf_dn("qqzz_histpdf_dn"+chan+Form("_%d",cate_vbf),"qqzz_histpdf", hist_argset, qqzzdatahist_dn);
    RooProdPdf qqzzpdf_dn("bkg_qqzz_EWKscale_VVDown","",qqzzkeyspdf_dn, Conditional(qqzzhistpdf_dn,*dbkg_kin));

	
		
/*    TCanvas *c_qqzz = new TCanvas();
    c_qqzz->cd();
    bkgdata.plotOn(frame,Binning(35));
    qqzzpdf.plotOn(frame);
    frame->Draw();
    c_qqzz->Update();
    c_qqzz->SaveAs("~/www/mass_width/13TeV_36_8fb/test_bkg_qqzz_"+chan+"_"+Form("%dS.png",cate_vbf));
*/

/*
	//hqqzz->Draw();
		for(int i =0;i<nbins_reco;i++){
			double ma = hqqzz->GetBinCenter(i+1);
			double bc = hqqzz->GetBinContent(i+1);
			if(bc==0)
				hqqzz->SetBinContent(i+1,1.e-10);
			double bc_up= hqqzz_up->GetBinContent(i+1);
			if(bc_up==0)
				hqqzz_up->SetBinContent(i+1,1.e-10);
			double bc_dn= hqqzz_dn->GetBinContent(i+1);
			if(bc_dn==0)
				hqqzz_dn->SetBinContent(i+1,1.e-10);
			//double eff = 6.548111e-03 - 5.866523e-06*ma*TMath::Gaus((ma-2.432632e+02)/2.272477e+01);
			//if(!cate_vbf)
			//	eff = 1.-eff;
			hqqzz->SetBinContent(i+1,hqqzz->GetBinContent(i+1)*eff);
			hqqzz_up->SetBinContent(i+1,hqqzz_up->GetBinContent(i+1)*eff);
			hqqzz_dn->SetBinContent(i+1,hqqzz_dn->GetBinContent(i+1)*eff);
		}
//	hqqzz->Draw("same");
*/	
/*	
	RooDataHist* qqzzhist= new RooDataHist("qqzzhist"+chan+Form("_%d",cate_vbf),"qqzzhist"+chan+Form("_%d",cate_vbf),RooArgSet(*mreco),hqqzz);
	//simplify: important, if VBF, change name "bkg_qqzz" to "bkg_ggzz"
	RooHistPdf* qqzzpdf= new RooHistPdf("bkg_qqzz","bkg_qqzz",RooArgSet(*mreco),*qqzzhist);

	RooDataHist* qqzzhist_up= new RooDataHist("qqzzhist_up"+chan+Form("_%d",cate_vbf),"qqzzhist_up"+chan+Form("_%d",cate_vbf),RooArgSet(*mreco),hqqzz_up);
	RooHistPdf* qqzzpdf_up= new RooHistPdf("bkg_qqzz_EWKscale_VVUp","bkg_qqzz_EWKscale_VVUp",RooArgSet(*mreco),*qqzzhist_up);
	RooDataHist* qqzzhist_dn= new RooDataHist("qqzzhist_dn"+chan+Form("_%d",cate_vbf),"qqzzhist_dn"+chan+Form("_%d",cate_vbf),RooArgSet(*mreco),hqqzz_dn);
	RooHistPdf* qqzzpdf_dn= new RooHistPdf("bkg_qqzz_EWKscale_VVDown","bkg_qqzz_EWKscale_VVDown",RooArgSet(*mreco),*qqzzhist_dn);
*/	

  RooRealVar *ewkvar=new RooRealVar("EWKscale_VV","EWKscale_VV",-7,7);
  ProcessNormalization *int_qq;
	int_qq= new ProcessNormalization("bkg_qqzz_norm","bkg_qqzz_norm",1.);
	double hqqzz_dn_int = hqqzz_dn->Integral();
	double hqqzz_up_int = hqqzz_up->Integral();
	double hqqzz_norm_int = hqqzz->Integral();
  int_qq->addAsymmLogNormal(hqqzz_dn_int/hqqzz_norm_int, hqqzz_up_int/hqqzz_norm_int, *ewkvar);
	cout << hqqzz_dn_int/hqqzz_norm_int<<" "<< hqqzz_up_int/hqqzz_norm_int<<endl;
	
	//zjet pdf

	TF1 *fsum ;
	int parsize ;
	double exp;
	 fsum=new TF1("fsum"+chan,"landau( 0 )+ landau(3)",50,3000);
//	 if(cate_vbf==2)
//		 fsum=new TF1("fsum"+chan,"landau( 0 )*(1 + exp( pol1(3)))",50,3000);
	 parsize=6;
//	 if(cate_vbf==2)
//		 parsize=5;
	 fsum->SetParameters(parzx);
	 if(cate_vbf!=2){
	if(chan=="4e")
	 exp=9.8*lumi/12.9;
	 else if(chan=="2e2mu")
		 exp = 20.4*lumi/12.9;
		else
			exp=10.2*lumi/12.9;
	 }
	 else{
			if(chan=="4e")
				exp = 13.2*lumi/12.9; 
			else if(chan=="2e2mu")
				exp = 18.3*lumi/12.9;
	 }
//	else{ 
//	 fsum=new TF1("fsum"+chan,"landau( 0 )",low_reco,high_reco);
//	 parsize=3;
//	 fsum->SetParameters(parzx);
//	 if(chan=="2e2mu")
//		 exp = 21.9;
//		else
//			exp=11.5;
//	}
//
	cout <<fsum->GetExpFormula("p")<<endl;
	
	if(cate_vbf==1){
	yields<< chan<< " "<< cate_vbf<<" ZX "<< 0.0762466*exp*fsum->Integral(low_reco,high_reco)/fsum->Integral(50,3000)<<endl;
	cout<< chan<< " "<< cate_vbf<<" ZX "<< 0.0762466*exp*fsum->Integral(low_reco,high_reco)/fsum->Integral(50,3000)<<endl;
	}
	else if(cate_vbf==0){
	yields<< chan<< " "<< cate_vbf<<" ZX "<< (1-0.0762466)*exp*fsum->Integral(low_reco,high_reco)/fsum->Integral(50,3000)<<endl;
	cout<< chan<< " "<< cate_vbf<<" ZX "<< (1-0.0762466)*exp*fsum->Integral(low_reco,high_reco)/fsum->Integral(50,3000)<<endl;
	}
	else{
	cout<< chan<< " "<< cate_vbf<<" ZX "<< exp*fsum->Integral(300,high_reco)/fsum->Integral(50,3000)<<endl;
	yields<< chan<< " "<< cate_vbf<<" ZX "<< exp*fsum->Integral(300,high_reco)/fsum->Integral(50,3000)<<endl;
	}
		
		RooArgList* parlist_zx=new RooArgList("parlist_zx");
		const int parsize_const = parsize;
		RooConstVar *pars_zx[parsize_const];

				parlist_zx->add(*mreco);
				
		for (int pa=0; pa<parsize; pa++){ 
				pars_zx[pa]=new RooConstVar(Form("par_zx_w%d_%s",pa,chan.Data()),"",parzx[pa]);	
				cout << parzx[pa]<<endl;
				parlist_zx->add(*pars_zx[pa]);
			}

	//RooAbsReal* zx_shape= bindFunction(fsum,*mreco,*parlist_zx) ;
  //TH1 *zxth1=zx_shape->createHistogram("zxth1",*mreco);
	//RooDataHist* zxhist= new RooDataHist("zxhist"+chan,"zxhist"+chan,RooArgSet(*mreco),zxth1);
	//RooHistPdf* zjetpdf= new RooHistPdf("bkg_zjet","bkg_zjet",RooArgSet(*mreco),*zxhist);

//	parlist_zx->Print("v");
	//RooGenericPdf 
	TString form="@1*TMath::Landau(@0,@2,@3)+@4*TMath::Landau(@0,@5,@6)";
//	if(cate_vbf==2)
//		form= "@1*TMath::Landau(@0,@2,@3)*(1+TMath::Exp(@4+@5*@0))";
//	RooGenericPdf *zjetpdf = new RooGenericPdf("bkg_zjet","bkg_zjet","@1*TMath::Landau(@0,@2,@3)+@4*TMath::Landau(@0,@5,@6)",*parlist_zx);

//********************* ZX and Template ***************************

			TFile *f_zx = new TFile("ZX_template.root","read");
			//TH2F* th2f_zx = (TH2F*) f_zx->Get("hist_temp_fast");
			TH2F* th2f_zx = (TH2F*) f_zx->Get("ZX_template");
			RooDataHist zxdatahist("zx_datahist"+chan+Form("_%d",cate_vbf), "", hist_argset, th2f_zx);
			RooHistPdf zxhistpdf("zx_histpdf"+chan+Form("_%d",cate_vbf),"", hist_argset, zxdatahist);
			RooGenericPdf * zjetpdf_generic = new RooGenericPdf("bkg_zjet_generic"+chan+Form("_%d",cate_vbf),"bkg_zjet_generic",form,*parlist_zx);
			TH1F* zx_th1f = (TH1F*) zjetpdf_generic->createHistogram("mreco");
			RooDataHist zx_th1_datahist("zx_th1_datahist"+chan+Form("_%d",cate_vbf),"",RooArgSet(*mreco),zx_th1f);
			RooHistPdf zx_th1_histpdf("zx_th1_histpdf"+chan+Form("_%d",cate_vbf),"",RooArgSet(*mreco),zx_th1_datahist);
			RooProdPdf *zjetpdf = new RooProdPdf("bkg_zjet","", zx_th1_histpdf, Conditional(zxhistpdf, *dbkg_kin));
			RooProdPdf *zjetpdf_dn = (RooProdPdf*) zjetpdf->Clone("bkg_zjet_EWKscale_VVDown");
			RooProdPdf *zjetpdf_up = (RooProdPdf*) zjetpdf->Clone("bkg_zjet_EWKscale_VVUp");
	
	//zjetpdf->plotOn(frame);
	//frame->Draw();
	//return;
//	TCanvas *c2=new TCanvas("c2","",800,600);
//	fsum->Draw("");
//
//	return;
//data
		TChain *tdata = new TChain("SelectedTree");
		tdata->Add("data.root");
//	if(highmass==2){
//		TH1F *histdata = new TH1F("histdata","",nbins_reco,low_reco,high_reco);
////		tdata->Add("root://lxcms03//data3/Higgs/160720/AllData/ZZ4lAnalysis.root");
//		if(chan=="2e2mu")
//			tdata->Draw("mreco>>histdata",Form("chan==3&&vbfcate==%d",cate_vbf));
//		else if(chan=="4mu")
//			tdata->Draw("mreco>>histdata",Form("chan==1&&vbfcate==%d",cate_vbf));
//		else if(chan=="4e")
//			tdata->Draw("mreco>>histdata",Form("chan==2&&vbfcate==%d",cate_vbf));
//		RooDataHist* data_obs = new RooDataHist("data_obs","data_obs",*mreco,histdata);
//		cout<<data_obs->sumEntries()<<endl;
//		w.import(*data_obs);
//	}
//	else{

		TString cutstring = Form("chan==%d&&vbfcate==%d",channum,cate_vbf);
		if(cate_vbf==4)
			cutstring = Form("chan==%d&&(vbfcate==0||vbfcate==1)",channum);
		TTree* reducetree= tdata->CopyTree(cutstring);
		RooDataSet* data_obs = new RooDataSet("data_obs","data_obs",reducetree,hist_argset);
		w.import(*data_obs);
		cout<<data_obs->sumEntries()<<endl;
//		TFile *fwork ;
//		if(highmass==2)
//			fwork= new TFile("workspace_data/hzz4l_"+chan+Form("%dS_13TeV.input_func.root",cate_vbf),"recreate");
//  	fwork->cd();
//    w.Write();
//    fwork->Close();
//return;

//	}

	w.import(qqzzpdf,RecycleConflictNodes());
//simplif no systematics, comment out begin
	w.import(qqzzpdf_up,RecycleConflictNodes());
	w.import(qqzzpdf_dn,RecycleConflictNodes());
	w.import(*int_qq,RecycleConflictNodes());
	w.import(*zjetpdf,RecycleConflictNodes());
	w.import(*zjetpdf_dn,RecycleConflictNodes());
	w.import(*zjetpdf_up,RecycleConflictNodes());


//		frame->Draw("");
//		frame_mzz->Draw("");
//	leg->Draw();

		w.Print();
		

		TFile *fwork ;
		if(highmass==0)
			fwork= new TFile("workspace_2d_bkg_onshell_ggH_dbkgkin/hzz4l_"+chan+Form("%dS_13TeV.input_func.root",cate_vbf),"recreate");
		if(highmass==1)
			fwork= new TFile("workspace_2d_bkg_offshell/hzz4l_"+chan+Form("%dS_13TeV.input_func.root",cate_vbf),"recreate");
		if(highmass==2)
			fwork= new TFile("workspace_2d_bkg/hzz4l_"+chan+Form("%dS_13TeV.input_func.root",cate_vbf),"recreate");
		fwork->cd();
		w.Write();
		fwork->Close();

}
void bkgWorkspace_with_Dbkgkin(TString chan, int vbfcate, int highmass){
//	for(int i=1;i<3;i++){
//		dosomething("2e2mu",1,i);
//		dosomething("2e2mu",0,i);
//		if(i==2){
//		dosomething("2e2mu",2,i);
//		dosomething("4e",2,i);
//		}
//		dosomething("4e",1,i);
//		dosomething("4e",0,i);
//		dosomething("4mu",1,i);
//		dosomething("4mu",0,i);
//	}
	  gROOT->ProcessLine("gSystem->AddIncludePath(\"-I$ROOFITSYS/include/\")");
    gROOT->ProcessLine("gSystem->Load(\"libRooFit\")");
    gROOT->ProcessLine("gSystem->Load(\"libHiggsAnalysisCombinedLimit.so\")");
		dosomething(chan,vbfcate,highmass);
}
