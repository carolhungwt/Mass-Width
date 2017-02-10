#include <map>
#include "TMath.h"

void setColZGradient_OneColor(int col, bool shift = false) {
    const Int_t NRGBs = 2;
    const Int_t NCont = 255;
//    Double_t stops[NRGBs] = { 0.00, 0.06};
    Double_t stops[NRGBs] = { 0.00, 1};
    Double_t red[NRGBs]   = { 1., col==1?0.50:col==2?223./256.:col==3?255./256.:col==4?200./256.:col==5? 25./256.:col==6? 52./256.:col==7?116./256.:0.40 };
    Double_t green[NRGBs] = { 1., col==1?0.50:col==2? 48./256.:col==3?128./256.:col==4?167./256.:col==5?121./256.:col==6?182./256.:col==7?186./256.:0.40 };
    Double_t blue[NRGBs]  = { 1., col==1?0.50:col==2?164./256.:col==3?  0./256.:col==4?  0./256.:col==5?218./256.:col==6?152./256.:col==7?255./256.:0.40 };
    if(shift){
      for(int i=0; i<NRGBs; i++){
	red  [i] -= 0.10;
	green[i] -= 0.10;
	blue [i] -= 0.10;
      }
    }
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
}

void setColZGrad_Onecolor_mod(bool shift=false){
const Int_t NCont = 999;
  const Int_t NRGBs = 7;
  Double_t stops[NRGBs] ={ 0.00, 0.06, 0.15, 0.75, 0.90, 0.98, 1.00 };
  Double_t red[NRGBs]   ={ 0.00, 0.10, 0.30, 0.90, 1.00, 1.00, 1.00 };
  Double_t green[NRGBs] ={ 0.50, 0.75, 1.00, 0.90, 0.55, 0.00, 0.00 };
  Double_t blue[NRGBs]  ={ 1.00, 0.20, 0.00, 0.00, 0.00, 0.00, 0.00 };  
  if(shift){
      for(int i=0; i<NRGBs; i++){
        red  [i] -= 0.10;
        green[i] -= 0.10;
        blue [i] -= 0.10;
      }
    }
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
}


void setColZGradient_TwoColors() {
    const Int_t NRGBs = 2;
    const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 1.00 };
    Double_t red[NRGBs]   = { 60./256., 1.00 };
    Double_t green[NRGBs] = { 140./256., 1.00 };
    Double_t blue[NRGBs]  = { 1.00, 0.50 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
}


void setColZGradient_Rainbow2_Compact() {
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.12, 0.3, 0.50, 1.00 };
    Double_t red[NRGBs]   = { 0.35, 0.00, 0.87, 1.00, 0.70 };
    Double_t green[NRGBs] = { 0.30, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 1.00, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
}


void makeContourfL1fa2(TString dir="fa2phaseAdap", TString input="ttH_4l_scan", int nbin=100, float x1=0, float x2=1, float y1=-3.1415926, float y2=3.1415926, TString xbrname="CMS_zz4l_fg4", TString ybrname="CMS_zz4l_fg4phi",TString xname="f_{a2}", TString yname="#phi_{a2}",TString output,bool isdata=false, bool condit=false){
	bool color=1;
  gROOT->ProcessLine(".L contours2D.cxx");
  gROOT->ProcessLine(".L contours.cxx");
  gROOT->ProcessLine(".x tdrstyle.cc");
  gStyle->SetOptStat(0);
//  gStyle->SetPalette(1);
//setColZGradient_OneColor(5);
setColZGrad_Onecolor_mod();
//setColZGradient_TwoColors();
//setColZGradient_Rainbow2_Compact();
//gStyle->SetPalette(75);
//
//  gStyle->SetNumberContours(500);
  gStyle->SetPadRightMargin(0.20);
  gROOT->ForceStyle();

TChain *t=new TChain("limit");
//if(!isdata)
//t->Add("../cards_"+dir+"_8TeV/HCG/125.6/"+"higgsCombine"+input+"_merge.MultiDimFit.mH125.6.root");
//t->Add("higgsCombine"+input+".MultiDimFit.mH120.123456.root");
t->Add(dir+"/"+input);
//t->Add("higgsCombine"+input+".MultiDimFit.mH120.root");
//else


//t->Add("../cards_"+dir+"_8TeV/HCG/125.6/"+"higgsCombine"+input+".MultiDimFit.mH125.6.root");
//t->Add("/afs/cern.ch/work/x/xiaomeng/test/myWorkingArea/CMSSW_7_1_5/src/CombinationPy/CreateDatacards/HZZ/cards_fa2fa3/higgsCombinedata_-1to1_3.MultiDimFit.mH125.6.root");

float xlow = x1-(x2-x1)/2./nbin;
float xhigh = x2+(x2-x1)/2./nbin;
float ylow = y1-(y2-y1)/2./nbin;
float yhigh =y2+(y2-y1)/2./nbin;

TH2F *gr0 =new TH2F("h","",nbin+1,x1+0.5,x2,nbin+1,y1,y2-.3);

float deltaNLL;
float CMS_zz4l_fg4;
float CMS_zz4l_fg4phi;
t->SetBranchAddress("deltaNLL",&deltaNLL);
t->SetBranchAddress(xbrname,&CMS_zz4l_fg4);
t->SetBranchAddress(ybrname,&CMS_zz4l_fg4phi);
float fg4pre, fg4phipre;
float lowest=50;
float lowest_x,lowest_y;
float first_x, first_y;
		std::map <float, float> a4;
for (int i =0;i<t->GetEntries();i++){
	t->GetEntry(i);
	if(deltaNLL==0)
continue;
	if (i==0){
		first_x= CMS_zz4l_fg4;
		first_y= CMS_zz4l_fg4phi;
	}
	if (deltaNLL<lowest){
		lowest = deltaNLL;
		lowest_x = CMS_zz4l_fg4;
		lowest_y = CMS_zz4l_fg4phi;
	}
	if(a4.count(CMS_zz4l_fg4)){
		if(a4[CMS_zz4l_fg4]>deltaNLL)
			a4[CMS_zz4l_fg4]=deltaNLL;
	}
	else
			a4[CMS_zz4l_fg4]=deltaNLL;
}
//cout << lowest_x<<"\t"<<lowest_y<<endl;
TGraph *best= new TGraph(1);
  best->SetMarkerStyle(28);
  best->SetMarkerSize(2);
best->SetPoint(0,lowest_x,lowest_y);
//TGraph *best2= new TGraph(1);
//  best2->SetMarkerStyle(34);
//  best2->SetMarkerSize(2);
//  best2->SetMarkerColor(0);
//best2->SetPoint(0,lowest_x,lowest_y);

int bestN=1;
TFile *fnew = new TFile("ttmp.root","recreate");
TTree *ttmp = new TTree("limit","limit");
ttmp->Branch(xbrname,&CMS_zz4l_fg4,xbrname+"/F");
ttmp->Branch(ybrname,&CMS_zz4l_fg4phi,ybrname+"/F");
ttmp->Branch("deltaNLL",&deltaNLL,"deltaNLL/F");
int locount=0;

for(int i =0;i<t->GetEntries();i++){
t->GetEntry(i);
//if (deltaNLL==0){
//if(CMS_zz4l_fg4!=lowest_x || CMS_zz4l_fg4phi!=lowest_y){
//	locount++;
//	best->SetPoint(locount,CMS_zz4l_fg4,CMS_zz4l_fg4phi);
//}
//	continue;
//}
	if(deltaNLL>100)
		continue;
int binn=gr0->FindBin(CMS_zz4l_fg4,CMS_zz4l_fg4phi);
if(gr0->GetBinContent(binn)!=0){
	//cout<<CMS_zz4l_fg4<<" "<<CMS_zz4l_fg4phi<<endl;
	continue;
}
		//deltaNLL-=lowest;
		if(condit)
		deltaNLL-=a4[CMS_zz4l_fg4];
		else
			deltaNLL-=lowest;
double content = (deltaNLL)*2;  

//double content = TMath::Log((deltaNLL)*2);
		ttmp->Fill();
gr0->Fill(CMS_zz4l_fg4,CMS_zz4l_fg4phi,content);
fg4pre = CMS_zz4l_fg4;
fg4phipre = CMS_zz4l_fg4phi;
}

fnew->cd();
ttmp->Write();
//ttmp->Scan("deltaNLL:CMS_zz4l_fg4");
  TH2 *lscan = treeToHist2D(ttmp,xbrname,ybrname,"");
  lscan->SetName("lscan");
  TList * tlFF = contourFromTH2(lscan,2.3);
  TList * tlFF95 = contourFromTH2(lscan,5.99);
  TList * tlFFSEE = contourFromTH2(lscan,1);
	//cout<<tlFF->GetSize()<<endl;

  styleMultiGraph(tlFF,  /*color=*/1, /*width=*/2, /*style=*/6);
  styleMultiGraph(tlFF95,  /*color=*/1, /*width=*/2, /*style=*/1);
//
 TGraph *gSEE = tlFFSEE->At(0);
  TGraph *g68 = tlFF->At(0);
  TGraph *g95 = tlFF95->At(0);

  g68->SetName("g68");
  g95->SetName("g95");
  gSEE->SetName("gSEE");
  g68->SetLineColor(kBlack);
  g68->SetLineWidth(2);
  g68->SetLineStyle(6);
  g95->SetLineColor(kBlack);
  g95->SetLineWidth(2);
  g95->SetLineStyle(1);
  gSEE->SetLineColor(kBlack);
  gSEE->SetLineWidth(2);


  char vfull[1000];

  double xmin,xmax,ymin,ymax,x,y;
  xmin=1000;
  xmax=-1000;
  ymin=10000;
  ymax=-1000;
  //for (unsigned int j=0;j<gSEE->GetN();++j) {
  //  gSEE->GetPoint(j,x,y);
  //  if (x>xmax) 
  //    xmax=x;
  //  if (x<xmin) 
  //    xmin=x;
  //  if (y>ymax) 
  //    ymax=y;
  //  if (y<ymin) 
  //    ymin=y;
  //}
  //printf ("fa1 = %f +%f -%f\n",lowest_x,xmax-lowest_x,lowest_x-xmin);
  //printf ("fa2 = %f +%f -%f\n",lowest_y,ymax-lowest_y,lowest_y-ymin);

  TCanvas *c = new TCanvas("c","c",1000,800);
	c->SetFillColor(0);
	c->SetBorderMode(0);
	c->SetBorderSize(2);
	c->SetTickx(1);
	c->SetTicky(1);
	c->cd();
	c->SetFrameFillStyle(0);
	c->SetFrameBorderMode(0);
	c->SetFrameFillStyle(0);
	c->SetFrameBorderMode(0);



  gr0->GetYaxis()->SetTitle(yname);
  gr0->GetXaxis()->SetTitle(xname);
  gr0->GetXaxis()->SetLabelSize(0.04);
  gr0->GetYaxis()->SetLabelSize(0.04);
  gr0->GetYaxis()->SetTitleOffset(1);
//  gr0->GetXaxis()->CenterTitle();
//  gr0->GetYaxis()->CenterTitle();
//  gr0->GetYaxis()->SetRangeUser(-1,1);
//  gr0->GetXaxis()->SetRangeUser(-1,1);

  //c->cd();
	//lscan->Draw("colz");
						
  TGraph *gr2= new TGraph(1);
  int point=0;
  for (int k1=0;k1<gr0->GetNbinsX();k1++){
	for (int k2=0;k2<gr0->GetNbinsY();k2++){
//		if( fabs(fabs(gr0->GetXaxis()->GetBinCenter(k1+1)) + fabs(gr0->GetYaxis()->GetBinCenter(k2+1)) -1)<-0.01  ){
		if( gr0->GetBinContent(k1+1,k2+1)!=0&& ( (gr0->GetBinContent(k1+1,k2+1)>gr0->GetBinContent(k1+1,k2+2) && gr0->GetBinContent(k1+1,k2+1)>gr0->GetBinContent(k1+1,k2) )||(gr0->GetBinContent(k1+1,k2+1)>gr0->GetBinContent(k1,k2+1) && gr0->GetBinContent(k1+1,k2+1)>gr0->GetBinContent(k1+2,k2+1)))){
	//		if(!condit)
			gr0->SetBinContent(k1+1,k2+1,(gr0->GetBinContent(k1+2,k2+1)+gr0->GetBinContent(k1,k2+1))/2.);
	//		cout<< k1+1<<"\t"<< k2+1<<endl;
	//									;
	//		gr2->SetPoint( point,gr0->GetXaxis()->GetBinCenter(k1+1), gr0->GetYaxis()->GetBinCenter(k2+1));
			point ++;
			}
		}
	}
//						gr2->Draw("same");
	gr0->SetMaximum(gr0->GetMaximum());
//	gr0->SetMaximum(15);
  gr0->Draw("colz");
	if(!color){
		gr0->Reset();
		gr0->Draw();
	}

	//tlFF->Draw("same");
	//tlFF95->Draw("same");
	if(!color){
  TLine *l2=new TLine();
   l2->SetLineWidth(1);
   l2->SetLineStyle(2);
   if(fabs(y1-1)<0.1){
   	l2->DrawLine(-1,0.,0.,1);
   	l2->DrawLine(0.,1.,1,0.);
   	l2->DrawLine(1,0.,0.,-1);
   	l2->DrawLine(0.,-1.,-1.,0.);
  	TLine *line5=new TLine();
    	line5->SetLineStyle(2);
    	line5 ->DrawLine(-1,0,1,0);
      	line5 ->DrawLine(0,-1,0,1);
		}
		else if(fabs(y1-0)<0.1){
  TLine *line5=new TLine();
    line5->SetLineStyle(2);
      line5 ->DrawLine(0,1,1,0);
		}
	}
//	tlFF94->Draw("same");
//	g68->Draw("Csame");
//  g95->Draw("Csame");
  //if(g95_2p) g95_2p->Draw("Csame");
  //g68->Draw("Csame");
  //if(g68_2p) g68_2p->Draw("Csame");

  TMarker m;
  m.SetMarkerSize(2);
  m.SetMarkerColor(kRed+1);
  m.SetMarkerStyle(33);


//	m.DrawMarker(125,0.1);

  TMarker mmin;
  mmin.SetMarkerSize(2);
  mmin.SetMarkerColor(1);
  mmin.SetMarkerStyle(33);


//	m.DrawMarker(0.46,0);
//	TChain *fcptree= new TChain("limit");
//	//fcptree->Add("higgsCombinettH_4l_scan_fcp.MultiDimFit.mH125.123456.root");
//	fcptree->Add("higgsCombinettH_4l_scan_fcp_0bkg.MultiDimFit.mH125.123456.root");
//	TGraph *grfcp = new TGraph(92);
//	float g4,g2,nll;
//	fcptree->SetBranchAddress("CMS_zz4l_fg4",&g4);
//	fcptree->SetBranchAddress("CMS_zz4l_fg2",&g2);
//	fcptree->SetBranchAddress("deltaNLL",&nll);
//	int point=0;
//	for(int j =0; j<fcptree->GetEntries()-1; j++){
//		fcptree->GetEntry(j);
////		mmin.DrawMarker(g4,g2);
//		if(fabs(g4)<1&&fabs(g2)<1&&nll!=0){
//			grfcp->SetPoint(point,g4,g2);
//			cout << g4<<" "<<g2<<endl;
//			point++;
//		}
//	}
//	cout << point<<endl;

//	grfcp->SetLineColor(1);
//	grfcp->SetLineWidth(2);
//	grfcp->Draw("Lsame");

	if(isdata&&!condit){
  //best2->Draw("Psame");
  best->Draw("Psame");
	}
    
  TLegend *l = new TLegend(0.57,0.83,0.75,0.86);

//  l->AddEntry(g95,"95% CL","l");
//  l->AddEntry(g68,"68% CL","l");

	if(isdata&&!condit)
  l->AddEntry(best,"Best fit","p");
//  l->AddEntry(&m,"SM","p");
//  l->AddEntry(grfcp,"f_{CP} minimum","l");
  //l->AddEntry(&m,"Inject point","p");
  l->SetBorderSize(0);
  l->SetFillStyle(0);
  l->SetTextFont(42);
  l->SetTextSize(0.04);
  l->Draw();

  float lumi7TeV=5.1;
  float lumi8TeV=19.7;


	TPaveText* pt = new TPaveText(0.15,0.955,0.92,1,"brNDC");
	pt->SetBorderSize(0);
	pt->SetFillStyle(0);
	pt->SetTextAlign(12);
	pt->SetTextFont(42);
	pt->SetTextSize(0.04);
	//TText* text = pt->AddText(0.02,0.45,"#font[61]{CMS}");
	TText* text = pt->AddText(0.02,0.45,"#font[61]{CMS}");
	text->SetTextSize(0.044);
	text = pt->AddText(0.12, 0.45, "#it{Preliminary}");
//	text->SetTextSize(0.0315);
	text = pt->AddText(0.61,0.45,"#font[42]{36.8 fb^{-1} (13 TeV)}");
	text->SetTextSize(0.0315);
	pt->Draw();

	TPaveText* pt2 = new TPaveText(0.95,0.8,0.99,0.9,"brNDC");
	pt2->SetBorderSize(0);
	pt2->SetFillStyle(0);
	pt2->SetTextAlign(21);
	pt2->SetTextFont(42);
	pt2->SetTextSize(0.04);
	TText* text = pt2->AddText(0.01,1,"#font[42]{-2 #Delta ln L}");
	text->SetTextSize(0.056);
	text->SetTextAngle(90);
pt2->Draw();

	TPaveText* pt3 = new TPaveText(0.62,0.85,0.86,0.99,"brNDC");
		pt3->SetBorderSize(0);
			pt3->SetFillStyle(0);
				pt3->SetTextAlign(21);
					pt3->SetTextFont(42);
						pt3->SetTextSize(0.04);


//if(input.Contains("onshell"))
	TText* text = pt3->AddText(0.01,0.3,"#font[42]{105 GeV < m_{4l}< 140 GeV}");
//	else

//	TText* text = pt3->AddText(0.01,0.3,"#font[42]{105 GeV < m_{4l}< 1600 GeV}");
	pt3->Draw();

	//if(xlow<-1)


		  c->SaveAs(dir+"/"+output+".root");
		  c->SaveAs(dir+"/"+output+".pdf");
//		  c->SaveAs(dir+"/"+output+".png");
		  c->SaveAs("~/www/mass_width/"+output+".png");
		  c->SaveAs(dir+"/"+output+".eps");
		  c->SaveAs(dir+"/"+output+".C");
    
fnew->Close();
}
