/** Things to remember later
 * mzz range defined by ggZZ workspace, create 2 versions for onshell and offshell
 * give different ggzz RooKeysPdf name for 2e2mu and 4e
 *
 ****/

#include <iostream>
#include <cmath>
#include <vector>
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TStyle.h"
#include "TMath.h"
#include "TSpline.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooAbsPdf.h"
#include "RooPlot.h"
#include "HZZ2L2QRooPdfs.h"
#include "HZZ4L_RooHighmass.h"
#include "Width_conv.h"
#include "Width_conv_offshell.h"
#include "Width_integral.h"
#include "Width_conv_integral.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooKeysPdf.h"
#include "RooExtendPdf.h"
#include "RooFormulaVar.h"
#include "RooAbsCategory.h"
#include "Riostream.h" 
#include "RooWorkspace.h"
#include "SplinePdf.h"

using namespace std;
using namespace TMath;
using namespace RooFit;



void vbf_change(TString chan ="2e2mu",unsigned int iCat =0,bool onshell=true){

	TString filename;
	if(onshell) filename = "workspace125_onshell/hzz"+chan+Form("_13TeV.input_func_vbf_cat%d.root", iCat);
	else filename = "workspace125/hzz"+chan+Form("_13TeV.input_func_vbf_cat%d.root", iCat);
	TFile* fwor=new TFile(filename, "recreate");
	fwor->cd();

	double lumi = 12.9;
	double ggzz_xsec = 0.488;
	double x_xsec = 0.312*1.108; 

	if(onshell){
		if(chan=="2e2mu"){
		ggzz_xsec = 0.1009;
		x_xsec = 0.275*1.108;; 
		}
		else{
			ggzz_xsec = 0.0473;
			x_xsec = 0.149*1.108;;
		}
	}
		else{
			if(chan!="2e2mu"){
			ggzz_xsec=0.239;
			x_xsec = 0.167*1.108;; 
			}
		}

	 gStyle->SetOptStat(0);
	  double dcbPara_2e2mu_2nd[6][3][3]={ // [parameter type][category][quadratic coef]
	    { {1.0244,0,0}, {1.0818,0,0}, {1.0336,0,0} },
	    { {1.9154,0,0}, {2.1662,0,0}, {1.8703,0,0} },
	    { {-0.278857,0,0}, {-0.333179,0,0}, {-0.613171,0,0} },
	    { {2.9629,0,0}, {2.9260,0,0}, {3.4551,0,0} },
	    { {2.3241,0,0}, {2.0013,0,0}, {2.9526,0,0} },
	    { {1.2975,0,0}, {1.4417,0,0}, {2.1706,0,0} }
	  };
	  double dcbPara_4mu_2nd[6][3][3]=
	  {
	    { {1.2651,0,0}, {1.2368,0,0}, {1.3985,0,0} },
	    { {2.0250,0,0}, {2.1748,0,0}, {2.0196,0,0} },
	    { {-0.219142,0,0}, {-0.174066,0,0}, {-0.249654,0,0} },
	    { {1.8784,0,0}, {2.0663,0,0}, {2.0315,0,0} },
	    { {2.4881,0,0}, {1.7939,0,0}, {2.8807,0,0} },
	    { {1.0501,0,0}, {1.0493,0,0}, {1.4110,0,0} }
	  };
	  double dcbPara_4e_2nd[6][3][3]=
	  {
	    { {0.96764,0,0}, {0.95576,0,0}, {1.0171,0,0} },
	    { {1.9963,0,0}, {1.9721,0,0}, {1.6302,0,0} },
	    { {-0.383618,0,0}, {-0.544708,0,0}, {-1.05107,0,0} },
	    { {3.7472,0,0}, {4.4534,0,0}, {4.1982,0,0} },
	    { {3.5587,0,0}, {2.3467,0,0}, {4.8193,0,0} },
	    { {1.5794,0,0}, {1.9007,0,0}, {2.7310,0,0} }
	  };

	double eff[3][11]={

//VBF->0p_4e_param_0:
-4.47849,4.5637,-80.8308,107.137,3.38191,0.00298955,-1.33756e-06,0.0179383,188.843,13.8356,1.83965e-10,
//VBF->0p_4mu_param_0:
-4.44681,4.59543,-87.5324,114.192,3.22478,0.00318981,-1.53157e-06,0.0216856,182.532,11.7483,2.26166e-10,
//VBF->0p_2e2mu_param_0:
-4.43582,4.60461,-196.311,191.783,2.22227,0.00204376,-9.41486e-07,0.0718321,148.027,50.0609,1.35394e-10
	};

	double dcbPara_2nd[6][3];
	double* effsig;


	  if (chan=="4e"){
	    for (int p=0; p<6; p++){ for (int k=0; k<3; k++) dcbPara_2nd[p][k]=dcbPara_4e_2nd[p][iCat][k]; }
	    effsig=eff[0];
	  }
	  if (chan=="4mu"){
	    for (int p=0; p<6; p++){ for (int k=0; k<3; k++) dcbPara_2nd[p][k]=dcbPara_4mu_2nd[p][iCat][k]; }
	    effsig=eff[1];
	  }
	  if (chan=="2e2mu"){
	    for (int p=0; p<6; p++){ for (int k=0; k<3; k++) dcbPara_2nd[p][k]=dcbPara_2e2mu_2nd[p][iCat][k]; }
	    effsig=eff[2];
	  }


	double lowarr[2]={100.5,100.5};
	double higharr[2]={1500.5,150.5};
	const int nbinsarr[2]={1500,100};

	double recolowarr[2]={104,105};
	double recohigharr[2]={1604.,140.};
	//const int reconbinsarr[2]={750,100};

	const double low= lowarr[onshell];
	const double high=higharr[onshell];
	const int nbins= nbinsarr[onshell]; 

	const double low_reco=recolowarr[onshell];
	const double high_reco=recohigharr[onshell];
	//const int nbins_reco=reconbinsarr[onshell];

	cout << low<<"\t"<<high<<endl;
	cout << low_reco<<"\t"<<high_reco<<endl;

	TString ap = (onshell ? "_onshell" : "");	
	TFile *fpdfbkg = new TFile("/afs/cern.ch/user/w/wahung/work/public/CMSSW_7_1_5/src/HiggsAnalysis/CombinedLimit/Mass-Width/Mass-Width_carol/prepareInputs/vbfpdfs"+ap+".root");
	RooWorkspace *wbkg =( RooWorkspace*)fpdfbkg->Get ("w"); 	


	//RooRealVar* mzz = new RooRealVar("ZZMass","M_{ZZ} (GeV)",125,low,high);
	RooRealVar* mzz = wbkg->var("ZZMass"); 
	RooRealVar* mreco= new RooRealVar("mreco","M_{ZZ} (GeV)",125,low_reco,high_reco);
	//RooRealVar* mdiff= new RooRealVar("mdiff","M_{ZZ} (GeV)",125,low_reco,high_reco);

	RooRealVar *r= new RooRealVar("r","signal strength",1.,0.000,1000);
	RooRealVar *rvbf_ggh= new RooRealVar("rvbf_ggh","rvb_ggh",1.,0.000,1000);
	RooFormulaVar *rvbf= new RooFormulaVar("rvbf","@0*@1",RooArgSet(*r,*rvbf_ggh));
	
	RooRealVar* mean = new RooRealVar("mean_pole","mean_pole",125,100,180);
	RooRealVar* sigma= new RooRealVar("sigma_pole","sigma_pole",0.00407,0.,10.);

	//RooConstVar* mean_125= new RooConstVar("mean_125","mean_125",125);
	//RooConstVar* sigma_125= new RooConstVar("sigma_125","sigma_125",0.00407);

	//RooPlot* frame= mreco->frame(low_reco,high_reco) ;
	//	RooPlot* frame= mreco->frame(150,250) ;
	//RooPlot* frame_mzz= mzz->frame(Title("Z mass")) ;
	TCanvas* cframe[4];
	for (unsigned int c=0; c<4;c++) cframe[c]=new TCanvas(Form("c%i",c),"", 750,700);
	RooPlot* frame=mreco->frame(low_reco,high_reco);
	RooPlot* frame_mzz=mzz->frame(Title("ZZ Mass"));

	RooPlot* frame_width= sigma->frame(Title("width")) ;
	RooPlot* frame_mean= mean->frame(Title("mean")) ;
	cframe[3]->cd();	frame_mean->Draw();
	fwor->cd();
	

	TFile *flo=new TFile("/afs/cern.ch/user/w/wahung/work/public/CMSSW_7_1_5/src/HiggsAnalysis/CombinedLimit/Mass-Width/Mass-Width_carol/prepareInputs/width_new_spline.root","read");
	TString chn = "";
	if(chan!="2e2mu")
		chn="4e";
	TSpline3 *lo=(TSpline3*)flo->Get("sp_xsec_statnom"+chn);
	fwor->cd();		

	  SplinePdf par2_int("par2_int"+chan+Form("%d", iCat), "", *mzz, *mean, *sigma, *lo);
	  RooRealVar m_gauss("m_gauss", "", 125);
	  RooRealVar w_gauss("w_gauss", "", 0.004);
	  RooGaussian gauss("gauss", "", *mzz, m_gauss, w_gauss);

 	  TString pdfn = "2e2mu";
   	  if (chan!="2e2mu") pdfn = "4e";
	  RooKeysPdf *pdfbkg = (RooKeysPdf*)wbkg->pdf("vbfpdfbkg_"+pdfn); 
	

	RooConstVar *ggzznorm= new RooConstVar("vbfbkgnorm"+chan+Form("%d",iCat),"",lumi*ggzz_xsec);
	RooExtendPdf pdf_ggzz("vbfpdf_bkg"+chan+Form("%d",iCat),"vbfpdf_bkg"+chan+Form("%d",iCat),*pdfbkg,*ggzznorm);
//	pdf_ggzz.plotOn(frame_mzz);
//	frame_mzz->Draw();
//	return;

	RooConstVar *xnorm= new RooConstVar("vbfxnorm"+chan+Form("%d",iCat),"",lumi*x_xsec);
	RooExtendPdf pdf_x("vbfpdf_x"+chan+Form("%d",iCat),"vbfpdf_x"+chan+Form("%d",iCat),par2_int,*xnorm);


  TFile *fphase_noweight=new TFile("/afs/cern.ch/user/w/wahung/work/public/CMSSW_7_1_5/src/HiggsAnalysis/CombinedLimit/Mass-Width/Mass-Width_carol/prepareInputs/fgraph_vbf_phase.root");
  TGraph *cosfunc = (TGraph*)fphase_noweight->Get("cos");
  TGraph *sinfunc = (TGraph*)fphase_noweight->Get("sin");


//	TFile *fbkge = new TFile ("VBFbkg_eff.root");
//	TGraph *eff_bkg =  (TGraph*)fbkge->Get("bkgeff_"+chan);
	
	TFile *fbkge = new TFile ("/afs/cern.ch/user/w/wahung/work/public/CMSSW_7_1_5/src/HiggsAnalysis/CombinedLimit/Mass-Width/Mass-Width_carol/prepareInputs/bkg_reg_eff.root");
	TGraph *eff_bkg =  (TGraph*)fbkge->Get("VBF_reg_"+chan);


	TGraph *effxkf_sig= new TGraph(nbins);
	TGraph *effxkf_bkg= new TGraph(nbins);

  TFile *fxsec=new TFile("/afs/cern.ch/user/w/wahung/work/public/CMSSW_7_1_5/src/HiggsAnalysis/CombinedLimit/Mass-Width/Mass-Width_carol/prepareInputs/xsec.root");
  TGraph *vbfxs= (TGraph*)fxsec->Get("vbf");
  TGraph *whxs= (TGraph*)fxsec->Get("wh");
  TGraph *zhxs= (TGraph*)fxsec->Get("zh");


	for(int i =0;i<nbins;i++){
		double cva = low+ i*(high-low)/double(nbins);
		double effval_sig = (effsig[0]+effsig[1]*TMath::Erf( (cva-effsig[2])/effsig[3] ))*(effsig[4]+effsig[5]*cva+effsig[6]*cva*cva+effsig[10]*cva*cva*cva)+effsig[7]*TMath::Gaus(cva,effsig[8],effsig[9]);
    double m4l = cva;
    //if (m4l > 900.) m4l = 900.;
    //double effcate = 3.844411e-01 + 2.755289e-04*m4l + -5.098494e-07*m4l*m4l + 2.491489e-10*m4l*m4l*m4l;
		double effcate_sig=1./3.;
		
    	//double effcate = 1.0/3.0;
		double effcate_bkg = (m4l < 200 ? (3.849250e-01 + 2.677220e-03*(m4l-200)) : (m4l < 300 ? 3.849250e-01+1.520690e-04*(m4l-200) + -1.379680e-07*(m4l-200)*(m4l-200) : +1.426950e+02/(m4l+3.971120e+02) - 1.426950e+02/(300+3.971120e+02) + 3.849250e-01+1.520690e-04*100 + -1.379680e-07*100*100 )) ;
		double vbffrac=1;
		if(cva<2000)
				vbffrac = vbfxs->Eval(cva)*0.955/(whxs->Eval(cva)*0.654+zhxs->Eval(cva)*0.669+vbfxs->Eval(cva)*0.955);
		if(iCat){
			effcate_sig= effcate_sig*vbffrac;
		}
   	else{ 
       effcate_sig= (1-effcate_sig)*vbffrac + (1-vbffrac)*1./0.7;
			 effcate_bkg = 1-effcate_bkg;
		}
		double va_bkg= eff_bkg->Eval(cva)*effcate_bkg; 	
		double va_sig= effval_sig*effcate_sig; 	
//		if(chan=="2e2mu"){
//			va_bkg*=0.95;
//			va_sig*=0.95;
//		}
//		return;
		effxkf_sig->SetPoint(effxkf_sig->GetN(),cva,va_sig);
		effxkf_bkg->SetPoint(effxkf_bkg->GetN(),cva,va_bkg);
	}
	effxkf_sig->SetName("vbfsigeffxkf"+chan+Form("%d",iCat));
	effxkf_bkg->SetName("vbfbkgeffxkf"+chan+Form("%d",iCat));

	//effxkf_sig->Draw("al");
	//effxkf_bkg->Draw("lsame");
	//return;

	mean->setRange(100,1500);
	sigma->setRange(0.00005,100.);
	mean->setVal(125);
	sigma->setVal(0.004165);

	TString formu_2nd="@1+@2*@0+@3*pow(@0,2)";	

	RooArgList formuList_a1;
	RooArgList formuList_a2;
	RooArgList formuList_mean;
	RooArgList formuList_n1;
	RooArgList formuList_n2;
	RooArgList formuList_sigma;
	formuList_a1.add(*mzz);
	formuList_a2.add(*mzz);
	formuList_mean.add(*mzz);
	formuList_n1.add(*mzz);
	formuList_n2.add(*mzz);
	formuList_sigma.add(*mzz);

	RooConstVar* a1_p0_0_2nd[3] ;
	RooConstVar* a2_p0_0_2nd[3] ;
	RooConstVar* mean_p0_0_2nd[3] ;
	RooConstVar* n1_p0_0_2nd[3] ;
	RooConstVar* n2_p0_0_2nd[3] ;
	RooConstVar* sigma_p0_0_2nd[3] ;
	for(int i =0; i<3;i++){
		a1_p0_0_2nd[i]= new RooConstVar(Form("%s_%d_a1_p0_0_2nd",chan.Data(),i),Form("%s_%d_a1_p0_0_2nd",chan.Data(),i),dcbPara_2nd[0][i]);
		a2_p0_0_2nd[i]= new RooConstVar(Form("%s_%d_a2_p0_0_2nd",chan.Data(),i),Form("%s_%d_a2_p0_0_2nd",chan.Data(),i),dcbPara_2nd[1][i]);
		mean_p0_0_2nd[i]= new RooConstVar(Form("%s_%d_mean_p0_0_2nd",chan.Data(),i),Form("%s_%d_mean_p0_0_2nd",chan.Data(),i),dcbPara_2nd[2][i]);
		n1_p0_0_2nd[i]= new RooConstVar(Form("%s_%d_n1_p0_0_2nd",chan.Data(),i),Form("%s_%d_n1_p0_0_2nd",chan.Data(),i),dcbPara_2nd[3][i]);
		n2_p0_0_2nd[i]= new RooConstVar(Form("%s_%d_n2_p0_0_2nd",chan.Data(),i),Form("%s_%d_n2_p0_0_2nd",chan.Data(),i),dcbPara_2nd[4][i]);
		sigma_p0_0_2nd[i]= new RooConstVar(Form("%s_%d_sigma_p0_0_2nd",chan.Data(),i),Form("%s_%d_sigma_p0_0_2nd",chan.Data(),i),dcbPara_2nd[5][i]);

		formuList_a1.add(*a1_p0_0_2nd[i]);
		formuList_a2.add(*a2_p0_0_2nd[i]);
		formuList_mean.add(*mean_p0_0_2nd[i]);
		formuList_n1.add(*n1_p0_0_2nd[i]);
		formuList_n2.add(*n2_p0_0_2nd[i]);
		formuList_sigma.add(*sigma_p0_0_2nd[i]);
	}

	RooFormulaVar* a1_p0_2nd= new RooFormulaVar("a1_p0_2nd"+chan,"a1_p0_2nd"+chan,formu_2nd,formuList_a1);
	RooFormulaVar* a2_p0_2nd= new RooFormulaVar("a2_p0_2nd"+chan,"a2_p0_2nd"+chan,formu_2nd,formuList_a2);
	RooFormulaVar* mean_p0_2nd= new RooFormulaVar("mean_p0_2nd"+chan,"mean_p0_2nd"+chan,formu_2nd,formuList_mean);
	RooFormulaVar* n1_p0_2nd= new RooFormulaVar("n1_p0_2nd"+chan,"n1_p0_2nd"+chan,formu_2nd,formuList_n1);
	RooFormulaVar* n2_p0_2nd= new RooFormulaVar("n2_p0_2nd"+chan,"n2_p0_2nd"+chan,formu_2nd,formuList_n2);
	RooFormulaVar* sigma_p0_2nd= new RooFormulaVar("sigma_p0_2nd"+chan,"sigma_p0_2nd"+chan,formu_2nd,formuList_sigma);

	//a1_p0_2nd->plotOn(frame_mzz);
	//a2_p0_2nd->plotOn(frame_mzz,LineColor(6));
	//n1_p0_2nd->plotOn(frame_mzz,LineColor(kCyan));
	//n2_p0_2nd->plotOn(frame_mzz,LineColor(kOrange));
	//mean_p0_2nd->plotOn(frame_mzz,LineColor(2));
	//sigma_p0_2nd->plotOn(frame_mzz,LineColor(3));
	//frame_mzz->Draw();
	//return;

	//	RooFormulaVar *mzz_shift = new RooFormulaVar("mzz_shift","","@0+@1",RooArgList(*mean_p0_2nd,*mzz));


    //RooFormulaVar *n2_p0_up = new RooFormulaVar("n2_p0_up","","@0+0.2*@0",*n2_p0_2nd);
    //RooFormulaVar *n2_p0_dn = new RooFormulaVar("n2_p0_dn","","@0-0.2*@0",*n2_p0_2nd);
    RooFormulaVar *sigma_p0_up = new RooFormulaVar("sigma_p0_up","","@0+0.2*@0",*sigma_p0_2nd);
    RooFormulaVar *sigma_p0_dn = new RooFormulaVar("sigma_p0_dn","","@0-0.2*@0",*sigma_p0_2nd);

	RooWorkspace w("w");
	RooDoubleCB dcrReso("dcrReso"+chan,"Double Crystal ball"+chan,*mreco,*mzz,*mean_p0_2nd,*sigma_p0_2nd,*a1_p0_2nd,*n1_p0_2nd,*a2_p0_2nd,*n2_p0_2nd);
	RooDoubleCB dcrReso_up("dcrReso"+chan+"_up","dcb up"+chan,*mreco,*mzz,*mean_p0_2nd,*sigma_p0_up,*a1_p0_2nd,*n1_p0_2nd,*a2_p0_2nd,*n2_p0_2nd);
	RooDoubleCB dcrReso_dn("dcrReso"+chan+"_dn","dcb up"+chan,*mreco,*mzz,*mean_p0_2nd,*sigma_p0_dn,*a1_p0_2nd,*n1_p0_2nd,*a2_p0_2nd,*n2_p0_2nd);
	RooAbsReal *final_integral; 
	RooAbsPdf* convpdf_spline;


if(onshell){
	convpdf_spline = new Width_conv("qqH", "qqH",*mreco, *mean, *sigma, *rvbf, RooArgList(pdf_x, pdf_ggzz,dcrReso),*cosfunc, *sinfunc, *effxkf_sig,*effxkf_bkg); 
//	convpdf_spline.plotOn(frame);
	w.import(*convpdf_spline,RecycleConflictNodes());
	Width_conv convpdf_spline_up("qqH_Res"+chan+"Up", "qqH_Res"+chan+"Up",*mreco, *mean, *sigma, *r, RooArgList(pdf_x, pdf_ggzz,dcrReso_up),*cosfunc, *sinfunc, *effxkf_sig,*effxkf_bkg); 
	Width_conv convpdf_spline_dn("qqH_Res"+chan+"Down", "qqH_Res"+chan+"Down",*mreco, *mean, *sigma, *r, RooArgList(pdf_x, pdf_ggzz,dcrReso_dn),*cosfunc, *sinfunc, *effxkf_sig,*effxkf_bkg); 
	w.import(convpdf_spline_up,RecycleConflictNodes());
	w.import(convpdf_spline_dn,RecycleConflictNodes());
	final_integral = convpdf_spline->createIntegral(*mreco);
}
else{
	convpdf_spline = new Width_conv_offshell("bqqH", "bqqH",*mreco, *mean, *sigma, *rvbf, RooArgList(pdf_x, pdf_ggzz,dcrReso),*cosfunc, *sinfunc, *effxkf_sig,*effxkf_bkg); 
//	convpdf_spline.plotOn(frame);
//	w.import(convpdf_spline,RecycleConflictNodes());
//	Width_conv_offshell convpdf_spline_up("qqH_Res"+chan+"Up", "qqH_Res"+chan+"Up",*mreco, *mean, *sigma, *r, RooArgList(pdf_x, pdf_ggzz,dcrReso_up),*cosfunc, *sinfunc, *effxkf_sig,*effxkf_bkg); 
//	Width_conv_offshell convpdf_spline_dn("qqH_Res"+chan+"Down", "qqH_Res"+chan+"Down",*mreco, *mean, *sigma, *r, RooArgList(pdf_x, pdf_ggzz,dcrReso_dn),*cosfunc, *sinfunc, *effxkf_sig,*effxkf_bkg); 
	final_integral = convpdf_spline->createIntegral(*mreco);
}

	mean->setVal(125);
	mreco->setVal(126);

	//pdf_ggzz.plotOn(frame_mzz);
	pdf_x.plotOn(frame_mzz);

	sigma->setVal(5.);
	convpdf_spline->plotOn(frame);
	sigma->setVal(1.);
	convpdf_spline->plotOn(frame,LineColor(2));
	sigma->setVal(0.004);
	convpdf_spline->plotOn(frame,LineColor(8));
	cframe[0]->cd();	frame->Draw();	fwor->cd();
    cframe[1]->cd(); 	frame_mzz->Draw(); 	fwor->cd();
//	return;

//	RooAbsReal *final_integral = convpdf_spline.createIntegral(*mreco);
	r->setVal(0);
	double bexp =  final_integral->getVal();
	cout << bexp<<endl;

	RooConstVar *bkg_integral= new RooConstVar("bkg_integral"+chan+Form("%d",iCat),"",bexp);

	mean->setVal(125);
	//ROOT::Math::Interpolator inter(200, ROOT::Math::Interpolation::kCSPLINE);
	sigma->setRange(0.00005,100.);

	TH2F *hint ; 
	TH2F *hsig ;
	if(!onshell){	
		hint= new TH2F("hint","",101,119.95,130.05,101,-0.0005,0.1005);
		hsig= new TH2F("hsig","",101,119.95,130.05,101,-0.0005,0.1005);
	}
	else{
		hint= new TH2F("hint","",101,119.95,130.05,101,-0.025,5.025);
		hsig= new TH2F("hsig","",101,119.95,130.05,101,-0.025,5.025);
		//hint= new TH2F("hint","",101,122.475,127.525,101,-0.025,5.025);
		//hsig= new TH2F("hsig","",101,122.475,127.525,101,-0.025,5.025);
	}
	//		double xi[201]; 
	//		double yi[201] ;

	for(int i = 0;i <101;i++){
		if(i%10==0)
			cout<<i<<endl;
		for(int j = 0;j <101;j++){
			double mv=hint->GetXaxis()->GetBinCenter(i+1);
			double sv=hint->GetYaxis()->GetBinCenter(j+1);
			sigma->setVal(sv);
			mean->setVal(mv);
			//sigma->setVal(0.001*(i+1));
			//mean->setVal(100+0.5*(j+1));
			r->setVal(1);
			double sbi =  final_integral->getVal();
			r->setVal(4);
			double sbi2 =  final_integral->getVal();
			double sexp = ((sbi2-sbi*2)+bexp)/2.;
			double iexp = sbi -sexp -bexp; 
			//cout<< sv<<"\t"<<mv<< "\t"<<sexp<< "\t"<<iexp<<endl;
			//			cout << sigma->getVal()<<"\t"<< mean->getVal()<<endl;
			//	float integral = final_integral->getVal();
			hint->Fill(mean->getVal(),sigma->getVal(),iexp);
			hsig->Fill(mean->getVal(),sigma->getVal(),sexp);
			//if(sigma->getVal()>1)
			//cout << sexp << endl;
			////		xi[i] = sigma->getVal();
			////		yi[i] = integral; 
		}
	}
	RooDataHist* hinthist= new RooDataHist("vbfhinthist"+chan+Form("%d",iCat),"vbfhinthist"+chan+Form("%d",iCat),RooArgSet(*mean,*sigma),hint);
	RooHistFunc *hintfunc = new RooHistFunc("vbfhintfunc"+chan+Form("%d",iCat),"",RooArgSet(*mean,*sigma),*hinthist);
	Width_integral inter_intergral ("vbfint_integral"+chan+Form("%d",iCat),"",*mean,*sigma,RooArgList(*hintfunc));

	RooDataHist* hsighist= new RooDataHist("vbfhsighist"+chan+Form("%d",iCat),"hsighist"+chan+Form("%d",iCat),RooArgSet(*mean,*sigma),hsig);
	RooHistFunc *hsigfunc = new RooHistFunc("vbfhsigfunc"+chan+Form("%d",iCat),"",RooArgSet(*mean,*sigma),*hsighist);
	Width_integral sig_intergral ("vbfsig_integral"+chan+Form("%d",iCat),"",*mean,*sigma,RooArgList(*hsigfunc));
	RooFormulaVar overall_intergral("qqH_norm","","@0*@2+ @1 + sqrt(@2)*@3",RooArgList(sig_intergral, *bkg_integral, *rvbf,inter_intergral));
	mean->setVal(125);
	sigma->setVal(0.004);
	cout<< sig_intergral.getVal()<<"\t"<< bkg_integral->getVal()<<"\t"<<inter_intergral.getVal()<<endl;

	Width_conv_integral convpdf_spline_integral("qqH", "qqH",*mreco, *mean, *sigma, *rvbf, RooArgList(pdf_x, pdf_ggzz,dcrReso,*hsigfunc,*hintfunc),*cosfunc, *sinfunc, *effxkf_sig,*effxkf_bkg); 
	Width_conv_integral convpdf_spline_integral_up("qqH_Res"+chan+"Up", "qqH_Res"+chan+"Up",*mreco, *mean, *sigma, *rvbf, RooArgList(pdf_x, pdf_ggzz,dcrReso_up,*hsigfunc,*hintfunc),*cosfunc, *sinfunc, *effxkf_sig,*effxkf_bkg); 
	Width_conv_integral convpdf_spline_integral_dn("qqH_Res"+chan+"Down", "qqH_Res"+chan+"Down",*mreco, *mean, *sigma, *rvbf, RooArgList(pdf_x, pdf_ggzz,dcrReso_dn,*hsigfunc,*hintfunc),*cosfunc, *sinfunc, *effxkf_sig,*effxkf_bkg); 

	w.import(convpdf_spline_integral,RecycleConflictNodes());
	w.import(convpdf_spline_integral_up,RecycleConflictNodes());
	w.import(convpdf_spline_integral_dn,RecycleConflictNodes());
	overall_intergral.plotOn(frame_width);
	cframe[2]->cd();	frame_width->Draw();	fwor->cd();

	mzz->setConstant(0);
	mean->setConstant(0);
	sigma->setConstant(0);
	mreco->setConstant(0);

	mreco->setRange(low_reco,high_reco);
	mean->setVal(125);
	sigma->setVal(0.004);
	r->setVal(1);

	convpdf_spline->plotOn(frame,LineColor(2));
	frame->Draw();

	w.import(overall_intergral,RecycleConflictNodes());

	mreco->Print("v");
	cout << "mreco nbins: "<< mreco->getBins()<<endl;

	w.Write();
	for(unsigned int c=0; c<4;c++){
		cframe[c]->cd();
		cframe[c]->Modified();
		cframe[c]->Update();
		fwor->WriteTObject(cframe[c]);
		cframe[c]->Close();
	}
	flo->Close();
	fwor->Close();
	
	return;
}

/*void  vbf(TString chan="4e",unsigned int iCat=0, bool onshell=1){
	gROOT->ProcessLine("gSystem->AddIncludePath(\"-I$ROOFITSYS/include/\")");
	gROOT->ProcessLine("gSystem->Load(\"libRooFit\")");
	gROOT->ProcessLine("gSystem->Load(\"libHiggsAnalysisCombinedLimit.so\")");

	gSystem->Exec("mkdir -p ./workspace125_onshell/../workspace125/");
	dosomething(chan, iCat, onshell);

}*/
