import FWCore.ParameterSet.Config as cms

ana = cms.EDAnalyzer('SimpleEfficiencyProducer',


					vertexSrc = cms.string('offlinePrimaryVertices'),
                                                  trackSrc = cms.InputTag('generalTracks'),
                                                  genParticleSrc = cms.InputTag('genParticles'),
                                                  offlineDCA = cms.untracked.double(3.0),
                                                  offlineptErr = cms.untracked.double(0.1),
				  		  offlinenhits = cms.untracked.double(11),
						  offlineChi2 = cms.untracked.double(0.15),
						  Nmin = cms.untracked.int32(0),
                                                  Nmax = cms.untracked.int32(10000),
                                                  vzLow = cms.untracked.double(-15.0),
                                                  vzHigh = cms.untracked.double(+15.0),
                                                  etaBins = cms.untracked.vdouble(-2.4,-2.3,-2.2,-2.1,-2,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,
                                                                                  -1.3,-1.2,-1.1,-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,
                                                                                  -0.2,-0.1,0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1.1,
                                                                                  1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2,2.1,2.2,2.3,2.4),
                                                  ptBins = cms.vdouble(
                                                                      0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45,
                                                                      0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95,
                                                                      1.0, 1.05, 1.1, 1.15, 1.2,
                                                                      1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0,
                                                                      2.5, 3.0, 4.0, 5.0, 7.5, 10.0, 12.0, 15.0,
                                                                      20.0, 25.0, 30.0, 45.0, 60.0, 90.0, 120.0, 
                                                                      180.0, 300.0, 500.0
                                                                      )
                                                  
)
