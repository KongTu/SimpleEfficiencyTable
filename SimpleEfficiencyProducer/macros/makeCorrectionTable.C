#include "RiceStyle.h"

using namespace std;

void makeCorrectionTable(){
	

	TFile* file = new TFile("../rootfiles/pPb_HIJING_SimpleEfficiencyTable_v2.root");
	TH2D* reco = (TH2D*) file->Get("ana/recoHist");
	TH2D* gen  = (TH2D*) file->Get("ana/genHist");

	reco->Divide( gen );

	// double etabins[] = {-2.4,-2.3,-2.2,-2.1,-2,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2,2.1,2.2,2.3,2.4};
	// const int Netabins = sizeof(etabins)/sizeof(etabins[0]) - 1;
	// double ptbins[] = {0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45,
 //        0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95,
 //        1.0, 1.05, 1.1, 1.15, 1.2,
 //        1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0,
 //        2.5, 3.0, 4.0, 5.0, 7.5, 10.0, 12.0, 15.0,
 //        20.0, 25.0, 30.0, 45.0, 60.0, 90.0, 120.0, 
 //        180.0, 300.0, 500.0};
 //    const int Nptbins = sizeof(ptbins)/sizeof(ptbins[0]) - 1;

	// TH2D* t1 = new TH2D("recoHist", "recoHist", Netabins, etabins, Nptbins, ptbins );
	// for(int eta = 0; eta < Netabins; eta++){

	// 	for(int pt = 0; pt < Nptbins; pt++){

	// 		double temp1 = reco->GetBinContent(eta+1, pt+1);
	// 		double temp2 = gen->GetBinContent(eta+1, pt+1);
	// 		double ratio = temp1/temp2;
	// 		t1->SetBinContent(eta+1, pt+1, ratio);
	// 	}
	// }


	TFile f1("HIJING_eff.root", "RECREATE");
	reco->Write();
}