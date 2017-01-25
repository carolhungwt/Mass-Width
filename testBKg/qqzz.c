#include "external_cConstants.h"
#include <vector>
void qqzz(){
		
		TString treename [3]={"ZZTree/candTree","ZZTreelooseEle/candTree","ZZTreetle/candTree"};
		TString newtreename[3]={"","_rse","_tle"};
		TFile* fnew = new TFile("qqzz_80.root","recreate");
		for(int t =0;t<3;t++){
	  TChain *tqqzz= new TChain(treename[t]);
	  tqqzz->Add("root://lxcms03://data3/Higgs/160726/ZZTo4l/ZZ4lAnalysis.root");
		TH2F *temp_zz = new TH2F("temp_zz"+newtreename[t],"",289,110,3000,30,0.,1.); 
	  //tqqzz->Add("root://lxcms03://data3/Higgs/160624/ZZTo4l/ZZ4lAnalysis.root");
		float ZZPt,ZZMass;
		float xsec,KFactorEWKqqZZ,overallEventWeight,KFactorQCDqqZZ_M;
		vector<float> *LepPt=new vector<float>;
		short Z1Flav,Z2Flav;
		short nCleanedJetsPt30;
		float pvbf_VAJHU_old;
		float phjj_VAJHU_old;
		float bkg_VAMCFM,p0plus_VAJHU;
		short ZZsel;
		float TLE_dR_Z;
		vector<short> *LepLepId=new vector<short>;
		tqqzz->SetBranchAddress("pvbf_VAJHU_highestPTJets",&pvbf_VAJHU_old);
		tqqzz->SetBranchAddress("phjj_VAJHU_highestPTJets",&phjj_VAJHU_old);
		tqqzz->SetBranchAddress("p0plus_VAJHU",&p0plus_VAJHU);
		tqqzz->SetBranchAddress("bkg_VAMCFM",&bkg_VAMCFM);
		tqqzz->SetBranchAddress("ZZPt",&ZZPt);
		tqqzz->SetBranchAddress("ZZMass",&ZZMass);
		tqqzz->SetBranchAddress("Z1Flav",&Z1Flav);
		tqqzz->SetBranchAddress("Z2Flav",&Z2Flav);
		tqqzz->SetBranchAddress("KFactor_EW_qqZZ",&KFactorEWKqqZZ);
		//tqqzz->SetBranchAddress("KFactorEWKqqZZ",&KFactorEWKqqZZ);
		tqqzz->SetBranchAddress("xsec",&xsec);
		tqqzz->SetBranchAddress("ZZsel",&ZZsel);
		tqqzz->SetBranchAddress("TLE_dR_Z",&TLE_dR_Z);
		tqqzz->SetBranchAddress("LepLepId",&LepLepId);
		tqqzz->SetBranchAddress("LepPt",&LepPt);
		tqqzz->SetBranchAddress("overallEventWeight",&overallEventWeight);
		tqqzz->SetBranchAddress("KFactor_QCD_qqZZ_M",&KFactorQCDqqZZ_M);
		//tqqzz->SetBranchAddress("KFactorQCDqqZZ_M",&KFactorQCDqqZZ_M);
		tqqzz->SetBranchAddress("nCleanedJetsPt30",&nCleanedJetsPt30);
		float weight, weight_up, weight_dn;
		float weight_vbf, weight_vbf_up, weight_vbf_dn;
		float weight_inc, weight_inc_up, weight_inc_dn;
		int chan;
		int vbfcate;
		TTree *tnew = new TTree("SelectedTree"+newtreename[t],"SelectedTree"+newtreename[t]);
		tnew->Branch("mreco",&ZZMass,"mreco/F");
		tnew->Branch("weight",&weight,"weight/F");
		tnew->Branch("weight_up",&weight_up,"weight_up/F");
		tnew->Branch("weight_dn",&weight_dn,"weight_dn/F");
		tnew->Branch("weight_vbf",&weight_vbf,"weight_vbf/F");
		tnew->Branch("weight_vbf_up",&weight_vbf_up,"weight_vbf_up/F");
		tnew->Branch("weight_vbf_dn",&weight_vbf_dn,"weight_vbf_dn/F");
		tnew->Branch("weight_inc",&weight_inc,"weight_inc/F");
		tnew->Branch("weight_inc_up",&weight_inc_up,"weight_inc_up/F");
		tnew->Branch("weight_inc_dn",&weight_inc_dn,"weight_inc_dn/F");
		tnew->Branch("chan",&chan,"chan/I");
		tnew->Branch("vbfcate",&vbfcate,"vbfcate/I");
		for(int i=0;i<tqqzz->GetEntries();i++){
			tqqzz->GetEntry(i);
			if(ZZsel!=120 && t>0)
				continue;
			if(t>0)
				if(ZZMass<300)
					continue;
			weight= xsec*KFactorEWKqqZZ * overallEventWeight*KFactorQCDqqZZ_M;
//      double eff = 6.548111e-03 - 5.866523e-06*ZZMass*TMath::Gaus((ZZMass-2.432632e+02)/2.272477e+01);
      double eff = 0.0139011;
			if(t>0)
				eff=0;
			double rho = ZZPt/(LepPt->at(0)+LepPt->at(1)+LepPt->at(2)+LepPt->at(3)); 

			if(t!=2){
			if(abs(Z1Flav)==abs(Z2Flav) && abs(Z1Flav)==121){
				chan=2;
			}
			else if (abs(Z1Flav)==abs(Z2Flav) && abs(Z1Flav)!=121){
				chan=1;
			}
			else{
				chan=3;
			}
			}
			short ZZFlav = Z1Flav*Z2Flav;
			float dbkg_kin = p0plus_VAJHU/(p0plus_VAJHU + bkg_VAMCFM*getDbkgkinConstant(ZZFlav, ZZMass));
			temp_zz->Fill(ZZMass,dbkg_kin,weight);

			bool patle = true;
			if(t==2){
							for (int k=0 ; k<LepLepId->size() ; k++){ 
										if (abs(LepLepId->at(k))==22 && LepPt->at(k)<=30) 
											patle = false;
							}
							 if(TLE_dR_Z<=1.6)
								 patle = false;
							if(ZZFlav==29282)
								chan =2;
							else if(Z1Flav*Z2Flav==40898)
								chan =3;
			}
			if(!patle)
				continue;
			//float vbfMela = pvbf_VAJHU_old / ( phjj_VAJHU_old*0.06 + pvbf_VAJHU_old );
			float vbfMela = pvbf_VAJHU_old / ( phjj_VAJHU_old + pvbf_VAJHU_old );
			if(vbfMela>0.5  && nCleanedJetsPt30>=2)
				vbfcate=1;
			else
				vbfcate=0;

				if(rho<0.3){
					weight_dn = weight*(1-abs((KFactorQCDqqZZ_M-1)*(KFactorEWKqqZZ-1))); 
					weight_up = weight*(1+abs((KFactorQCDqqZZ_M-1)*(KFactorEWKqqZZ-1))); 
				}
				else{
					weight_dn = weight*(1-abs((KFactorEWKqqZZ-1))); 
					weight_up = weight*(1+abs((KFactorEWKqqZZ-1))); 
				}

				weight_inc= weight;
				weight_inc_dn= weight_dn;
				weight_inc_up= weight_up;

				weight_vbf= weight*eff;
				weight_vbf_dn= weight_dn*eff;
				weight_vbf_up= weight_up*eff;

				weight*= (1-eff);
				weight_dn*= (1-eff);
				weight_up*= (1-eff);

			
			tnew->Fill();
		}
//		tnew->Draw("ZZMass","weight_up","same");
//		tnew->Draw("ZZMass","weight_dn","same");
		tqqzz->SetLineColor(2);
		tqqzz->SetMarkerColor(2);
//		tqqzz->Draw("ZZMass","xsec*KFactorEWKqqZZ * overallEventWeight*KFactorQCDqqZZ_M","same");
		fnew->cd();
		temp_zz->Write();	
		tnew->Write();
		}
		fnew->Close();
}
