void plotScan_fTU_PMF_Twiki(double threshold = 0.49){
  gROOT->ProcessLine(".x tdrstyle.cc");
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadTopMargin(0.05);
  float mass = 125.;
  double excl[2] = {1,3.841};

  TChain* t[2];
  for(int c=0;c<2;c++) t[c] = new TChain("limit");

  Float_t CMS_zz4l_fai1,CMS_zz4l_fai2,CMS_zz4l_phiai1,deltaNLL;
  double minX=0,minY=0,minLL=20;

  TH1F* h0 = new TH1F("h0","",10,-1-0.005,1+0.005); // 10 points. expected
  TH1F* h0p = new TH1F("h0p","",301,-1-2./600.,1+2./600.); // 301 points, observed

  TGraph* gr[1];
  TH1F* hContainer[1] = {
	  h0, h0p
  };
  double** x = new double* [2];
  double** y = new double* [2];

  t[0]->Add("../higgsCombinescan.MultiDimFit.mH120.123456.root");
//  t[1]->Add("./Observed_Fits/higgsCombine2D_obs_1D_fTU_PMF.MultiDimFit.mH91.2.root");

  t[0]->SetBranchAddress("CMS_zz4l_fai1",&CMS_zz4l_fai1);
  t[0]->SetBranchAddress("deltaNLL",&deltaNLL);
//  t[1]->SetBranchAddress("CMS_zz4l_fai1",&CMS_zz4l_fai1);
//  t[1]->SetBranchAddress("deltaNLL",&deltaNLL);

  for (int ev = 0; ev < t[0]->GetEntries(); ev++){
	  t[0]->GetEntry(ev);
	  int binx = h0->GetXaxis()->FindBin(CMS_zz4l_fai1);
	  double bincontent = h0->GetBinContent(binx);
	  if (bincontent == 0) h0->SetBinContent(binx, 2 * deltaNLL);
  };
/*  for (int ev = 0; ev < t[1]->GetEntries(); ev++){
	  t[1]->GetEntry(ev);
	  int binx = h0p->GetXaxis()->FindBin(CMS_zz4l_fai1);
	  double bincontent = h0p->GetBinContent(binx);
	  if (bincontent == 0) h0p->SetBinContent(binx, 2 * deltaNLL);
  };
*/
  for (int c = 0; c < 1; c++){
	  for (int binx = 5; binx < (hContainer[c]->GetNbinsX() - 4); binx++){
		  double bincontent = hContainer[c]->GetBinContent(binx);
		  double bincontent_up = hContainer[c]->GetBinContent(binx + 4);
		  double bincontent_dn = hContainer[c]->GetBinContent(binx - 4);
		  double bincenter = hContainer[c]->GetXaxis()->GetBinCenter(binx);
		  double bincenter_up = hContainer[c]->GetXaxis()->GetBinCenter(binx + 2);
		  double bincenter_dn = hContainer[c]->GetXaxis()->GetBinCenter(binx - 2);
		  if(bincenter<=0.5) continue;
		  if (bincontent < (bincontent_up + bincontent_dn)*threshold){
			  bincontent = (bincontent_up - bincontent_dn) / (bincenter_up - bincenter_dn) * (bincenter - bincenter_dn) + bincontent_dn;
		  }
		  hContainer[c]->SetBinContent(binx,bincontent);
	  }
  }



  for (int c = 0; c < 1; c++){
	  x[c] = new double[hContainer[c]->GetNbinsX()];
	  y[c] = new double[hContainer[c]->GetNbinsX()];

	for (int binx = 1; binx <= hContainer[c]->GetNbinsX(); binx++){
		x[c][binx-1] = hContainer[c]->GetBinCenter(binx);
		y[c][binx-1] = hContainer[c]->GetBinContent(binx);
	};

	int nExp=hContainer[c]->GetNbinsX();
	int ipol = -1;
	double xupdown[2] = { 0, 1 };
	double yupdown[2] = { 0, 0 };
	double yMin=y[c][nExp-1],xMin=x[c][nExp-1];
	for (int ipo = 0; ipo<(nExp - 1); ipo++){
		if (yMin>y[c][ipo]){
			yMin=y[c][ipo];
			xMin=x[c][ipo];
		}
		if( (y[c][ipo] <= excl[1] && y[c][ipo + 1]>excl[1]) || (y[c][ipo] >= excl[1] && y[c][ipo + 1]<excl[1]) ){
			ipol = ipo;
			xupdown[0] = x[c][ipol];
			xupdown[1] = x[c][ipol + 1];
			yupdown[0] = y[c][ipol];
			yupdown[1] = y[c][ipol + 1];
		}
		else if( (y[c][ipo + 1] == excl[1] && ipo==(nExp - 2)) ){
			ipol = ipo + 1;
			xupdown[0] = x[c][ipol - 1];
			xupdown[1] = x[c][ipol];
			yupdown[0] = y[c][ipol - 1];
			yupdown[1] = y[c][ipol];
		}
		else continue;
		double a = (yupdown[1] - yupdown[0]) / (xupdown[1] - xupdown[0]);
		double b = yupdown[0];
		printf("Curve %i: 95%% limit %.4f\n",c, (excl[1] - b) / a + xupdown[0]);
	};
	ipol=-1;
	for (int ipo = 0; ipo<(nExp - 1); ipo++){
		if( (y[c][ipo] <= excl[0] && y[c][ipo + 1]>excl[0]) || (y[c][ipo] >= excl[0] && y[c][ipo + 1]<excl[0]) ){
			ipol = ipo;
			xupdown[0] = x[c][ipol];
			xupdown[1] = x[c][ipol + 1];
			yupdown[0] = y[c][ipol];
			yupdown[1] = y[c][ipol + 1];
		}
		else if (y[c][ipo + 1] == excl[0] && ipo==(nExp - 2)){
			ipol = ipo + 1;
			xupdown[0] = x[c][ipol - 1];
			xupdown[1] = x[c][ipol];
			yupdown[0] = y[c][ipol - 1];
			yupdown[1] = y[c][ipol];
		}
		else continue;
		double a = (yupdown[1] - yupdown[0]) / (xupdown[1] - xupdown[0]);
		double b = yupdown[0];
		printf("Curve %i: 1 sigma limit %.4f\n", c, (excl[0] - b) / a + xupdown[0]);
	};
	printf("Curve %i: Exp minimum %.4f\n", c, xMin);


	gr[c] = new TGraph(( hContainer[c]->GetNbinsX() ),x[c],y[c]);
	gr[c]->SetName(Form("tg_%i",c));
	gr[c]->SetTitle("");
	gr[c]->SetLineWidth(2);
	gr[c]->SetMarkerColor(0);
  };

	gr[0]->SetLineColor(kBlack);
	gr[0]->SetLineStyle(7);
	gr[1]->SetLineColor(kBlack);
	gr[1]->SetLineStyle(1);



  TCanvas *c1=new TCanvas("can_scan1D","",800,800);
	c1->SetFillColor(0);
	c1->SetBorderMode(0);
	c1->SetBorderSize(2);
	c1->SetTickx(1);
	c1->SetTicky(1);
	c1->cd();
	c1->SetFrameFillStyle(0);
	c1->SetFrameBorderMode(0);
	c1->SetFrameFillStyle(0);
	c1->SetFrameBorderMode(0);

  gr[0]->GetXaxis()->SetTitle("f_{t+u} cos(#phi_{#lower[-0.25]{t+u}})");
  gr[0]->GetYaxis()->SetTitle("-2 #Delta lnL");
  gr[0]->GetXaxis()->SetLabelSize(0.04);
  gr[0]->GetYaxis()->SetLabelSize(0.04);
  gr[0]->GetXaxis()->SetTitleSize(0.06);
  gr[0]->GetYaxis()->SetTitleSize(0.06);
  gr[0]->GetXaxis()->SetLabelFont(42);
  gr[0]->GetYaxis()->SetLabelFont(42);
  gr[0]->GetXaxis()->SetTitleFont(42);
  gr[0]->GetYaxis()->SetTitleFont(42);
  gr[0]->GetYaxis()->SetRangeUser(0., 120.);
  gr[0]->GetXaxis()->SetRangeUser(-1,1);
  gr[0]->Draw("AL");
//  gr[1]->Draw("lsame");

  TLegend *leg = new TLegend(0.2,0.7,0.5,0.92);
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.04);

  leg->AddEntry(gr[1], "Observed, #phi_{t+u} = 0 or #pi","l");
  leg->AddEntry(gr[0], "Expected, #phi_{t+u} = 0 or #pi","l");
  leg->Draw();

  float lumi7TeV=5.1;
  float lumi8TeV=19.7;

	TPaveText* pt = new TPaveText(0.15,0.955,0.96,1,"brNDC");
	pt->SetBorderSize(0);
	pt->SetFillStyle(0);
	pt->SetTextAlign(12);
	pt->SetTextFont(42);
	pt->SetTextSize(0.04);
	TText* text = pt->AddText(0.02,0.45,"#font[61]{CMS}");
	text->SetTextSize(0.044);
		text = pt->AddText(0.14, 0.42, "#font[52]{Unpublished}");
		text->SetTextSize(0.0315);
	text = pt->AddText(0.48,0.45,"#font[42]{19.7 fb^{-1} (8 TeV) + 5.1 fb^{-1} (7 TeV)}");
	text->SetTextSize(0.0315);
	pt->Draw();

/*
  TPaveText *oneSig = new TPaveText(0.18,0.16,0.28,0.19,"NDC");
  oneSig->SetFillColor(0);
  oneSig->SetFillStyle(0);
  oneSig->SetTextFont(42);
  oneSig->SetTextColor(kBlack);
  oneSig->SetBorderSize(0);
  oneSig->AddText("68\% CL"); 
  oneSig->Draw();
*/
  TPaveText *twoSig = new TPaveText(0.78,7.5,0.89,8.72);
  twoSig->SetFillColor(0);
  twoSig->SetFillStyle(0);
  twoSig->SetTextFont(42);
  twoSig->SetTextColor(kBlack);
  twoSig->SetBorderSize(0);
  twoSig->SetTextSize(0.03);
  twoSig->AddText("95\% CL");
  twoSig->Draw();
/*
  TLine *l1=new TLine();
  l1->SetLineStyle(9);
  l1->SetLineWidth(1);
  l1->SetLineColor(kBlack);
  l1->DrawLine(-1,1,1,1);
  l1->Draw("same");
*/  TLine *l2=new TLine();
  l2->SetLineStyle(9);
  l2->SetLineWidth(1);
  l2->SetLineColor(kBlack);
  l2->DrawLine(-1,3.84,1,3.84);
  l2->Draw("same");

  c1->SaveAs("can_scan1D_fTU_PMF_Twiki.eps");
  c1->SaveAs("can_scan1D_fTU_PMF_Twiki.pdf");
  c1->SaveAs("can_scan1D_fTU_PMF_Twiki.png");
  c1->SaveAs("can_scan1D_fTU_PMF_Twiki.root");
  c1->SaveAs("can_scan1D_fTU_PMF_Twiki.C");
}

void plot1D_no_obs(){
	plotScan_fTU_PMF_Twiki();
}
