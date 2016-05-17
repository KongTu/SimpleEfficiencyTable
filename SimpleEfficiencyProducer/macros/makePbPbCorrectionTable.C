#include "RiceStyle.h"

using namespace std;

void makePbPbCorrectionTable(){
	

	TFile* file = new TFile("../rootfiles/trkCorrFlow_hydjet100k_badWedge_narrowEta.root");
	
	TH2D* eff = (TH2D*) file->Get("rEff_cbin5");
	TH2D* fake  = (TH2D*) file->Get("rFak_cbin5");

	double ptbins[] = {0.3, 0.4, 0.5, 0.6, 0.8, 1.0, 1.2, 1.6, 2.0, 2.5, 3.0, 4.0, 6.0, 8.0, 12.0};

	TH2D* table = new TH2D("table", "table", 24, -2.4,2.4, 14, ptbins);

	for(int eta = 0; eta < eff->GetNbinsX(); eta++){
		for(int pt = 0; pt < eff->GetNbinsY(); pt++){

			double temp = 0;
			double eff1 = eff->GetBinContent(eta+1, pt+1);
			double fake1 = fake->GetBinContent(eta+1, pt+1);

			temp = eff1*(1-fake1);

			table->SetBinContent(eta+1, pt+1, temp);
		}
	}


	TFile f1("HIJING_PbPb_eff.root", "RECREATE");
	table->Write();
}