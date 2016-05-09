// -*- C++ -*-
//
// Package:    SimpleEfficiencyProducer
// Class:      SimpleEfficiencyProducer
// 
/**\class SimpleEfficiencyProducer SimpleEfficiencyProducer.cc SimpleEfficiencyTable/SimpleEfficiencyProducer/src/SimpleEfficiencyProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Zhoudunming Tu,42 1-015,,
//         Created:  Sun May  8 14:30:10 CEST 2016
// $Id$
//
//


// system include files
#include "SimpleEfficiencyTable/SimpleEfficiencyProducer/interface/SimpleEfficiencyProducerBase.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
SimpleEfficiencyProducer::SimpleEfficiencyProducer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  trackSrc_ = iConfig.getParameter<edm::InputTag>("trackSrc");
  vertexSrc_ = iConfig.getParameter<std::string>("vertexSrc");
  genParticleSrc_ = iConfig.getParameter<edm::InputTag>("genParticleSrc");
  
  Nmin_ = iConfig.getUntrackedParameter<int>("Nmin");
  Nmax_ = iConfig.getUntrackedParameter<int>("Nmax");
    
  vzLow_ = iConfig.getUntrackedParameter<double>("vzLow");
  vzHigh_ = iConfig.getUntrackedParameter<double>("vzHigh");

  offlineptErr_ = iConfig.getUntrackedParameter<double>("offlineptErr", 0.0);
  offlineDCA_ = iConfig.getUntrackedParameter<double>("offlineDCA", 0.0);
  offlineChi2_ = iConfig.getUntrackedParameter<double>("offlineChi2", 0.0);
  offlinenhits_ = iConfig.getUntrackedParameter<double>("offlinenhits", 0.0);

  etaBins_ = iConfig.getUntrackedParameter<std::vector<double>>("etaBins");
  ptBins_ = iConfig.getUntrackedParameter<std::vector<double>>("ptBins");
}


SimpleEfficiencyProducer::~SimpleEfficiencyProducer()
{
}


//
// member functions
//

// ------------ method called for each event  ------------
void
SimpleEfficiencyProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;

  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(vertexSrc_,vertices);
  double bestvz=-999.9, bestvx=-999.9, bestvy=-999.9;
  double bestvzError=-999.9, bestvxError=-999.9, bestvyError=-999.9;
  const reco::Vertex & vtx = (*vertices)[0];
  bestvz = vtx.z(); bestvx = vtx.x(); bestvy = vtx.y();
  bestvzError = vtx.zError(); bestvxError = vtx.xError(); bestvyError = vtx.yError();
  
  //first selection; vertices
  if(bestvz < vzLow_ || bestvz > vzHigh_ ) return;

  Handle<reco::TrackCollection> tracks;
  iEvent.getByLabel(trackSrc_, tracks);

  int nTracks = 0;
  for(unsigned it = 0; it < tracks->size(); it++){

     const reco::Track & trk = (*tracks)[it];

     math::XYZPoint bestvtx(bestvx,bestvy,bestvz);
        
        double dzvtx = trk.dz(bestvtx);
        double dxyvtx = trk.dxy(bestvtx);
        double dzerror = sqrt(trk.dzError()*trk.dzError()+bestvzError*bestvzError);
        double dxyerror = sqrt(trk.d0Error()*trk.d0Error()+bestvxError*bestvyError);
        //double nhits = trk.numberOfValidHits();
        //double chi2 = trk.chi2();
        //double ndof = trk.ndof();
        double nlayers = trk.hitPattern().pixelLayersWithMeasurement();//only pixel layers
        //chi2n = chi2n/nlayers;

        if(!trk.quality(reco::TrackBase::highPurity)) continue;
        if(fabs(trk.ptError())/trk.pt() > offlineptErr_ ) continue;
        if(fabs(dzvtx/dzerror) > offlineDCA_) continue;
        if(fabs(dxyvtx/dxyerror) > offlineDCA_) continue;
        if(fabs(trk.eta()) < 2.4 && trk.pt() > 0.4 ){nTracks++;}// NtrkOffline        
        
        if(fabs(trk.eta()) > 2.4 ) continue;
        //if(chi2 > 5) continue;
        //if(ndof < 5) continue;
        if(nlayers <= 0 ) continue;

        recoHist->Fill(trk.eta(), trk.pt() );

  } 

  if( !useCentrality_ ) if( nTracks < Nmin_ || nTracks >= Nmax_ ) return;
  
  Ntrk->Fill(nTracks);

  edm::Handle<reco::GenParticleCollection> genParticleCollection;
  iEvent.getByLabel(genParticleSrc_, genParticleCollection);

  for(unsigned it=0; it<genParticleCollection->size(); ++it) {

    const reco::GenParticle & genCand = (*genParticleCollection)[it];
    int status = genCand.status();
    double genpt = genCand.pt();
    double geneta = genCand.eta();
    int gencharge = genCand.charge();

    if( status != 1  || gencharge == 0 ) continue;
    if( fabs(geneta) > 2.4 ) continue;

    genHist->Fill(geneta, genpt);

  }


}

// ------------ method called once each job just before starting event loop  ------------
void 
SimpleEfficiencyProducer::beginJob()
{

  edm::Service<TFileService> fs;
    
  TH2D::SetDefaultSumw2();

  const int NEtaBins = etaBins_.size() - 1;
  double etaBinsArray[100];

  for(unsigned i = 0; i < etaBins_.size(); i++){

    etaBinsArray[i] = etaBins_[i];
  }

  const int NptBins = ptBins.size() - 1;
  double ptBinsArray[100];

  for(unsigned i = 0; i < ptBins.size(); i++){

    ptBinsArray[i] = ptBins[i];
  }


  Ntrk = fs->make<TH1D>("Ntrk",";Ntrk",5000,0,5000);

  recoHist = fs->make<TH2D>("recoHist", ";#eta;p_{T}", NEtaBins, etaBinsArray, NptBins, ptBinsArray);
  genHist  = fs->make<TH2D>("genHist", ";#eta;p_{T}", NEtaBins, etaBinsArray, NptBins, ptBinsArray);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
SimpleEfficiencyProducer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
SimpleEfficiencyProducer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
SimpleEfficiencyProducer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
SimpleEfficiencyProducer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
SimpleEfficiencyProducer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SimpleEfficiencyProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(SimpleEfficiencyProducer);
