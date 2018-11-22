#include <AsgTools/MessageCheck.h>
#include <MuonAnalysis/MuonSelection.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODMuon/MuonContainer.h>
#include <xAODTruth/TruthParticleContainer.h>
#include <xAODTracking/TrackParticlexAODHelpers.h>
#include <xAODTracking/VertexContainer.h>
#include <TSystem.h>
#include <math.h>

MuonSelection :: MuonSelection (const std::string& name,
                                  ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm (name, pSvcLocator),
      m_grl ("GoodRunsListSelectionTool/grl", this), 
      m_muonSelection ("CP::MuonSelectionTool", this),
      CB (),
      ME (), 
      MSO (),
      MSOE (),
      ID (),
			muon_truth ()
{
  positiveMuons = new std::vector<const xAOD::Muon*>();
  negativeMuons = new std::vector<const xAOD::Muon*>();
}

MuonSelection :: ~MuonSelection () {
  // Delete positive muon variables
	if (p_pdgID_truth) delete p_pdgID_truth;
	if (p_pt_truth) delete p_pt_truth;
	if (p_eta_truth) delete p_eta_truth;
	if (p_phi_truth) delete p_phi_truth;
	if (p_m_truth) delete p_m_truth;
  if (p_passIDcuts) delete p_passIDcuts;
  if (p_passAll) delete p_passAll;
  if (p_ptcone40) delete p_ptcone40;
  if (p_eLoss) delete p_eLoss;
  if (p_muonType) delete p_muonType;
  if (p_quality) delete p_quality;
  if (p_primaryAuthor) delete p_primaryAuthor;
  if (p_authors) delete p_authors;
  if (p_nPrecisionLayers) delete p_nPrecisionLayers;
  if (p_nPrecisionHoleLayers) delete p_nPrecisionHoleLayers;
  if (p_nPhiLayers) delete p_nPhiLayers;
  if (p_nPhiHoleLayers) delete p_nPhiHoleLayers;
  if (p_nTriggerLayers) delete p_nTriggerLayers;
  if (p_nTriggerHoleLayers) delete p_nTriggerHoleLayers;
  if (p_innerSmallHits) delete p_innerSmallHits;
  if (p_innerLargeHits) delete p_innerLargeHits;
  if (p_middleSmallHits) delete p_middleSmallHits;
  if (p_middleLargeHits) delete p_middleLargeHits;
  if (p_outerSmallHits) delete p_outerSmallHits;
  if (p_outerLargeHits) delete p_outerLargeHits;
  if (p_extendedSmallHits) delete p_extendedSmallHits;
  if (p_extendedLargeHits) delete p_extendedLargeHits;
  if (p_innerSmallHoles) delete p_innerSmallHoles;
  if (p_innerLargeHoles) delete p_innerLargeHoles;
  if (p_middleSmallHoles) delete p_middleSmallHoles;
  if (p_middleLargeHoles) delete p_middleLargeHoles;
  if (p_outerSmallHoles) delete p_outerSmallHoles;
  if (p_outerLargeHoles) delete p_outerLargeHoles;
  if (p_extendedSmallHoles) delete p_extendedSmallHoles;
  if (p_extendedLargeHoles) delete p_extendedLargeHoles;

  // Delete negative muon variables
	if (n_pdgID_truth) delete n_pdgID_truth;
	if (n_pt_truth) delete n_pt_truth;
	if (n_eta_truth) delete n_eta_truth;
	if (n_phi_truth) delete n_phi_truth;
	if (n_m_truth) delete n_m_truth;
  if (n_passIDcuts) delete n_passIDcuts;
  if (n_passAll) delete n_passAll;
  if (n_ptcone40) delete n_ptcone40;
  if (n_eLoss) delete n_eLoss;
  if (n_muonType) delete n_muonType;
  if (n_quality) delete n_quality;
  if (n_primaryAuthor) delete n_primaryAuthor;
  if (n_authors) delete n_authors;
  if (n_nPrecisionLayers) delete n_nPrecisionLayers;
  if (n_nPrecisionHoleLayers) delete n_nPrecisionHoleLayers;
  if (n_nPhiLayers) delete n_nPhiLayers;
  if (n_nPhiHoleLayers) delete n_nPhiHoleLayers;
  if (n_nTriggerLayers) delete n_nTriggerLayers;
  if (n_nTriggerHoleLayers) delete n_nTriggerHoleLayers;
  if (n_innerSmallHits) delete n_innerSmallHits;
  if (n_innerLargeHits) delete n_innerLargeHits;
  if (n_middleSmallHits) delete n_middleSmallHits;
  if (n_middleLargeHits) delete n_middleLargeHits;
  if (n_outerSmallHits) delete n_outerSmallHits;
  if (n_outerLargeHits) delete n_outerLargeHits;
  if (n_extendedSmallHits) delete n_extendedSmallHits;
  if (n_extendedLargeHits) delete n_extendedLargeHits;
  if (n_innerSmallHoles) delete n_innerSmallHoles;
  if (n_innerLargeHoles) delete n_innerLargeHoles;
  if (n_middleSmallHoles) delete n_middleSmallHoles;
  if (n_middleLargeHoles) delete n_middleLargeHoles;
  if (n_outerSmallHoles) delete n_outerSmallHoles;
  if (n_outerLargeHoles) delete n_outerLargeHoles;
  if (n_extendedSmallHoles) delete n_extendedSmallHoles;
  if (n_extendedLargeHoles) delete n_extendedLargeHoles;
  
  // Delete positive track parameters
  if (p_isCB) delete p_isCB;
  if (p_isME) delete p_isME;
  if (p_isMSO) delete p_isMSO;
  if (p_isMSOE) delete p_isMSOE;
  if (p_isID) delete p_isID;
  if (p_eta_CB) delete p_eta_CB;
  if (p_eta_ME) delete p_eta_ME;
  if (p_eta_MSO) delete p_eta_MSO;
  if (p_eta_MSOE) delete p_eta_MSOE;
  if (p_eta_ID) delete p_eta_ID;
  if (p_phi_CB) delete p_phi_CB;
  if (p_phi_ME) delete p_phi_ME;
  if (p_phi_MSO) delete p_phi_MSO;
  if (p_phi_MSOE) delete p_phi_MSOE;
  if (p_phi_ID) delete p_phi_ID;
  if (p_z0_CB) delete p_z0_CB;
  if (p_z0_ME) delete p_z0_ME;
  if (p_z0_MSO) delete p_z0_MSO;
  if (p_z0_MSOE) delete p_z0_MSOE;
  if (p_z0_ID) delete p_z0_ID;
  if (p_d0sig_CB) delete p_d0sig_CB;
  if (p_d0sig_ME) delete p_d0sig_ME;
  if (p_d0sig_MSO) delete p_d0sig_MSO;
  if (p_d0sig_MSOE) delete p_d0sig_MSOE;
  if (p_d0sig_ID) delete p_d0sig_ID;
  if (p_qOverP_CB) delete p_qOverP_CB;
  if (p_qOverP_ME) delete p_qOverP_ME;
  if (p_qOverP_MSO) delete p_qOverP_MSO;
  if (p_qOverP_MSOE) delete p_qOverP_MSOE;
  if (p_qOverP_ID) delete p_qOverP_ID;
  if (p_dqOverP_CB) delete p_dqOverP_CB;
  if (p_dqOverP_ME) delete p_dqOverP_ME;
  if (p_dqOverP_MSO) delete p_dqOverP_MSO;
  if (p_dqOverP_MSOE) delete p_dqOverP_MSOE;
  if (p_dqOverP_ID) delete p_dqOverP_ID;
  if (p_rchi2_CB) delete p_rchi2_CB;
  if (p_rchi2_ME) delete p_rchi2_ME;
  if (p_rchi2_MSO) delete p_rchi2_MSO;
  if (p_rchi2_MSOE) delete p_rchi2_MSOE;
  if (p_rchi2_ID) delete p_rchi2_ID;
  
  // Delete negative track parameters
  if (n_isCB) delete n_isCB;
  if (n_isME) delete n_isME;
  if (n_isMSO) delete n_isMSO;
  if (n_isMSOE) delete n_isMSOE;
  if (n_isID) delete n_isID;
  if (n_eta_CB) delete n_eta_CB;
  if (n_eta_ME) delete n_eta_ME;
  if (n_eta_MSO) delete n_eta_MSO;
  if (n_eta_MSOE) delete n_eta_MSOE;
  if (n_eta_ID) delete n_eta_ID;
  if (n_phi_CB) delete n_phi_CB;
  if (n_phi_ME) delete n_phi_ME;
  if (n_phi_MSO) delete n_phi_MSO;
  if (n_phi_MSOE) delete n_phi_MSOE;
  if (n_phi_ID) delete n_phi_ID;
  if (n_z0_CB) delete n_z0_CB;
  if (n_z0_ME) delete n_z0_ME;
  if (n_z0_MSO) delete n_z0_MSO;
  if (n_z0_MSOE) delete n_z0_MSOE;
  if (n_z0_ID) delete n_z0_ID;
  if (n_d0sig_CB) delete n_d0sig_CB;
  if (n_d0sig_ME) delete n_d0sig_ME;
  if (n_d0sig_MSO) delete n_d0sig_MSO;
  if (n_d0sig_MSOE) delete n_d0sig_MSOE;
  if (n_d0sig_ID) delete n_d0sig_ID;
  if (n_qOverP_CB) delete n_qOverP_CB;
  if (n_qOverP_ME) delete n_qOverP_ME;
  if (n_qOverP_MSO) delete n_qOverP_MSO;
  if (n_qOverP_MSOE) delete n_qOverP_MSOE;
  if (n_qOverP_ID) delete n_qOverP_ID;
  if (n_dqOverP_CB) delete n_dqOverP_CB;
  if (n_dqOverP_ME) delete n_dqOverP_ME;
  if (n_dqOverP_MSO) delete n_dqOverP_MSO;
  if (n_dqOverP_MSOE) delete n_dqOverP_MSOE;
  if (n_dqOverP_ID) delete n_dqOverP_ID;
  if (n_rchi2_CB) delete n_rchi2_CB;
  if (n_rchi2_ME) delete n_rchi2_ME;
  if (n_rchi2_MSO) delete n_rchi2_MSO;
  if (n_rchi2_MSOE) delete n_rchi2_MSOE;
  if (n_rchi2_ID) delete n_rchi2_ID;

  if (positiveMuons) delete positiveMuons;
  if (negativeMuons) delete negativeMuons;
}

StatusCode MuonSelection :: initialize ()
{
  // set up GRL tool
  const char* GRLFilePath = "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/GoodRunsLists/data17_13TeV/20180619/data17_13TeV.periodAllYear_DetStatus-v99-pro22-01_Unknown_PHYS_StandardGRL_All_Good_25ns_Triggerno17e33prim.xml";
  const char* fullGRLFilePath = gSystem->ExpandPathName (GRLFilePath);
  std::vector<std::string> vecStringGRL;
  vecStringGRL.push_back(fullGRLFilePath);
  ANA_CHECK(m_grl.setProperty( "GoodRunsListVec", vecStringGRL));
  ANA_CHECK(m_grl.setProperty("PassThrough", false));
  ANA_CHECK(m_grl.initialize());

  // set up muon selection tool
  ANA_CHECK (m_muonSelection.setProperty("MaxEta", 2.7));
  ANA_CHECK (m_muonSelection.setProperty("MuQuality", 1));
  ANA_CHECK (m_muonSelection.initialize());

	// book trees
  ANA_CHECK(book (TTree ("RecoMuons", "Reconstructed muons")));
  TTree* zmumutree = tree ("RecoMuons");

  ANA_MSG_INFO ("About to initialize branches");

  // set branch addresses for zmumutree  
  // event level
  zmumutree->Branch("passGRL", &passGRL);
  zmumutree->Branch("eventNumber", &eventNumber);
  zmumutree->Branch("nPositiveMuons", &nPositiveMuons);
  zmumutree->Branch("nNegativeMuons", &nNegativeMuons);

  // positive muon variables
	p_pdgID_truth = new std::vector<int>();
	zmumutree->Branch("p_pdgID_truth", &p_pdgID_truth);
	p_pt_truth = new std::vector<double>();
	zmumutree->Branch("p_pt_truth", &p_pt_truth);
	p_eta_truth = new std::vector<double>();
	zmumutree->Branch("p_eta_truth", &p_eta_truth);
	p_phi_truth = new std::vector<double>();
	zmumutree->Branch("p_phi_truth", &p_phi_truth);
	p_m_truth = new std::vector<double>();
	zmumutree->Branch("p_m_truth", &p_m_truth);
	p_passIDcuts = new std::vector<bool>();
  zmumutree->Branch("p_passIDcuts", &p_passIDcuts);
  p_passAll = new std::vector<bool>();
  zmumutree->Branch("p_passAll", &p_passAll);  
  p_ptcone40 = new std::vector<float>();
  zmumutree->Branch("p_ptcone40",&p_ptcone40);
  p_eLoss = new std::vector<float>();
  zmumutree->Branch("p_eLoss",&p_eLoss);
  p_muonType = new std::vector<int>();
  zmumutree->Branch("p_muonType", &p_muonType);
  p_quality = new std::vector<int>();
  zmumutree->Branch("p_quality", &p_quality);
  p_primaryAuthor = new std::vector<int>();
  zmumutree->Branch("p_primaryAuthor", &p_primaryAuthor);
  p_authors = new std::vector<int>();
  zmumutree->Branch("p_authors", &p_authors);
  
  p_nPrecisionLayers = new std::vector<int>();
  zmumutree->Branch("p_nPrecisionLayers", &p_nPrecisionLayers); 
  p_nPrecisionHoleLayers = new std::vector<int>(); 
  zmumutree->Branch("p_nPrecisionHoleLayers", &p_nPrecisionHoleLayers); 
  p_nPhiLayers = new std::vector<int>(); 
  zmumutree->Branch("p_nPhiLayers", &p_nPhiLayers); 
  p_nPhiHoleLayers = new std::vector<int>(); 
  zmumutree->Branch("p_nPhiHoleLayers", &p_nPhiHoleLayers); 
  p_nTriggerLayers = new std::vector<int>(); 
  zmumutree->Branch("p_nTriggerLayers", &p_nTriggerLayers); 
  p_nTriggerHoleLayers = new std::vector<int>(); 
  zmumutree->Branch("p_nTriggerHoleLayers", &p_nTriggerHoleLayers); 
  p_innerSmallHits = new std::vector<int>(); 
  zmumutree->Branch("p_innerSmallHits", &p_innerSmallHits); 
  p_innerLargeHits = new std::vector<int>(); 
  zmumutree->Branch("p_innerLargeHits", &p_innerLargeHits); 
  p_middleSmallHits = new std::vector<int>(); 
  zmumutree->Branch("p_middleSmallHits", &p_middleSmallHits); 
  p_middleLargeHits = new std::vector<int>(); 
  zmumutree->Branch("p_middleLargeHits", &p_middleLargeHits); 
  p_outerSmallHits = new std::vector<int>(); 
  zmumutree->Branch("p_outerSmallHits", &p_outerSmallHits); 
  p_outerLargeHits = new std::vector<int>(); 
  zmumutree->Branch("p_outerLargeHits", &p_outerLargeHits); 
  p_extendedSmallHits = new std::vector<int>(); 
  zmumutree->Branch("p_extendedSmallHits", &p_extendedSmallHits); 
  p_extendedLargeHits = new std::vector<int>(); 
  zmumutree->Branch("p_extendedLargeHits", &p_extendedLargeHits); 
  p_innerSmallHoles = new std::vector<int>(); 
  zmumutree->Branch("p_innerSmallHoles", &p_innerSmallHoles); 
  p_innerLargeHoles = new std::vector<int>(); 
  zmumutree->Branch("p_innerLargeHoles", &p_innerLargeHoles); 
  p_middleSmallHoles = new std::vector<int>(); 
  zmumutree->Branch("p_middleSmallHoles", &p_middleSmallHoles); 
  p_middleLargeHoles = new std::vector<int>(); 
  zmumutree->Branch("p_middleLargeHoles", &p_middleLargeHoles); 
  p_outerSmallHoles = new std::vector<int>(); 
  zmumutree->Branch("p_outerSmallHoles", &p_outerSmallHoles); 
  p_outerLargeHoles = new std::vector<int>(); 
  zmumutree->Branch("p_outerLargeHoles", &p_outerLargeHoles); 
  p_extendedSmallHoles = new std::vector<int>(); 
  zmumutree->Branch("p_extendedSmallHoles", &p_extendedSmallHoles); 
  p_extendedLargeHoles = new std::vector<int>(); 
  zmumutree->Branch("p_extendedLargeHoles", &p_extendedLargeHoles); 
  
  // negative muon variables
	n_pdgID_truth = new std::vector<int>();
	zmumutree->Branch("n_pdgID_truth", &n_pdgID_truth);
	n_pt_truth = new std::vector<double>();
	zmumutree->Branch("n_pt_truth", &n_pt_truth);
	n_eta_truth = new std::vector<double>();
	zmumutree->Branch("n_eta_truth", &n_eta_truth);
	n_phi_truth = new std::vector<double>();
	zmumutree->Branch("n_phi_truth", &n_phi_truth);
	n_m_truth = new std::vector<double>();
	zmumutree->Branch("n_m_truth", &n_m_truth);
  n_passIDcuts = new std::vector<bool>();
  zmumutree->Branch("n_passIDcuts", &n_passIDcuts);
  n_passAll = new std::vector<bool>();
  zmumutree->Branch("n_passAll", &n_passAll);  
  n_ptcone40 = new std::vector<float>();
  zmumutree->Branch("n_ptcone40",&n_ptcone40);
  n_eLoss = new std::vector<float>();
  zmumutree->Branch("n_eLoss",&n_eLoss);
  n_muonType = new std::vector<int>();
  zmumutree->Branch("n_muonType", &n_muonType);
  n_quality = new std::vector<int>();
  zmumutree->Branch("n_quality", &n_quality);
  n_primaryAuthor = new std::vector<int>();
  zmumutree->Branch("n_primaryAuthor", &n_primaryAuthor);
  n_authors = new std::vector<int>();
  zmumutree->Branch("n_authors", &n_authors);
  
  n_nPrecisionLayers = new std::vector<int>();
  zmumutree->Branch("n_nPrecisionLayers", &n_nPrecisionLayers); 
  n_nPrecisionHoleLayers = new std::vector<int>(); 
  zmumutree->Branch("n_nPrecisionHoleLayers", &n_nPrecisionHoleLayers); 
  n_nPhiLayers = new std::vector<int>(); 
  zmumutree->Branch("n_nPhiLayers", &n_nPhiLayers); 
  n_nPhiHoleLayers = new std::vector<int>(); 
  zmumutree->Branch("n_nPhiHoleLayers", &n_nPhiHoleLayers); 
  n_nTriggerLayers = new std::vector<int>(); 
  zmumutree->Branch("n_nTriggerLayers", &n_nTriggerLayers); 
  n_nTriggerHoleLayers = new std::vector<int>(); 
  zmumutree->Branch("n_nTriggerHoleLayers", &n_nTriggerHoleLayers); 
  n_innerSmallHits = new std::vector<int>(); 
  zmumutree->Branch("n_innerSmallHits", &n_innerSmallHits); 
  n_innerLargeHits = new std::vector<int>(); 
  zmumutree->Branch("n_innerLargeHits", &n_innerLargeHits); 
  n_middleSmallHits = new std::vector<int>(); 
  zmumutree->Branch("n_middleSmallHits", &n_middleSmallHits); 
  n_middleLargeHits = new std::vector<int>(); 
  zmumutree->Branch("n_middleLargeHits", &n_middleLargeHits); 
  n_outerSmallHits = new std::vector<int>(); 
  zmumutree->Branch("n_outerSmallHits", &n_outerSmallHits); 
  n_outerLargeHits = new std::vector<int>(); 
  zmumutree->Branch("n_outerLargeHits", &n_outerLargeHits); 
  n_extendedSmallHits = new std::vector<int>(); 
  zmumutree->Branch("n_extendedSmallHits", &n_extendedSmallHits); 
  n_extendedLargeHits = new std::vector<int>(); 
  zmumutree->Branch("n_extendedLargeHits", &n_extendedLargeHits); 
  n_innerSmallHoles = new std::vector<int>(); 
  zmumutree->Branch("n_innerSmallHoles", &n_innerSmallHoles); 
  n_innerLargeHoles = new std::vector<int>(); 
  zmumutree->Branch("n_innerLargeHoles", &n_innerLargeHoles); 
  n_middleSmallHoles = new std::vector<int>(); 
  zmumutree->Branch("n_middleSmallHoles", &n_middleSmallHoles); 
  n_middleLargeHoles = new std::vector<int>(); 
  zmumutree->Branch("n_middleLargeHoles", &n_middleLargeHoles); 
  n_outerSmallHoles = new std::vector<int>(); 
  zmumutree->Branch("n_outerSmallHoles", &n_outerSmallHoles); 
  n_outerLargeHoles = new std::vector<int>(); 
  zmumutree->Branch("n_outerLargeHoles", &n_outerLargeHoles); 
  n_extendedSmallHoles = new std::vector<int>(); 
  zmumutree->Branch("n_extendedSmallHoles", &n_extendedSmallHoles); 
  n_extendedLargeHoles = new std::vector<int>(); 
  zmumutree->Branch("n_extendedLargeHoles", &n_extendedLargeHoles); 
  
  // positive muon track variables
  p_isCB = new std::vector<bool>();
  zmumutree->Branch("p_isCB", &p_isCB);
  p_isME = new std::vector<bool>();
  zmumutree->Branch("p_isME", &p_isME);
  p_isMSO = new std::vector<bool>();
  zmumutree->Branch("p_isMSO", &p_isMSO);
  p_isMSOE = new std::vector<bool>();
  zmumutree->Branch("p_isMSOE", &p_isMSOE);
  p_isID = new std::vector<bool>();
  zmumutree->Branch("p_isID", &p_isID);
  
  p_eta_CB = new std::vector<float>();
  zmumutree->Branch("p_eta_CB", &p_eta_CB);
  p_eta_ME = new std::vector<float>();
  zmumutree->Branch("p_eta_ME", &p_eta_ME); 
  p_eta_MSO = new std::vector<float>();
  zmumutree->Branch("p_eta_MSO", &p_eta_MSO); 
  p_eta_MSOE = new std::vector<float>();
  zmumutree->Branch("p_eta_MSOE", &p_eta_MSOE); 
  p_eta_ID = new std::vector<float>();
  zmumutree->Branch("p_eta_ID", &p_eta_ID); 

  p_phi_CB = new std::vector<float>();
  zmumutree->Branch("p_phi_CB", &p_phi_CB);
  p_phi_ME = new std::vector<float>();
  zmumutree->Branch("p_phi_ME", &p_phi_ME);
  p_phi_MSO = new std::vector<float>();
  zmumutree->Branch("p_phi_MSO", &p_phi_MSO);
  p_phi_MSOE = new std::vector<float>();
  zmumutree->Branch("p_phi_MSOE", &p_phi_MSOE);
  p_phi_ID = new std::vector<float>();
  zmumutree->Branch("p_phi_ID", &p_phi_ID);

  p_d0sig_CB = new std::vector<float>();
  zmumutree->Branch("p_d0sig_CB", &p_d0sig_CB);
  p_d0sig_ME = new std::vector<float>();
  zmumutree->Branch("p_d0sig_ME", &p_d0sig_ME);
  p_d0sig_MSO = new std::vector<float>();
  zmumutree->Branch("p_d0sig_MSO", &p_d0sig_MSO);
  p_d0sig_MSOE = new std::vector<float>();
  zmumutree->Branch("p_d0sig_MSOE", &p_d0sig_MSOE);
  p_d0sig_ID = new std::vector<float>();
  zmumutree->Branch("p_d0sig_ID", &p_d0sig_ID);

  p_z0_CB = new std::vector<float>();
  zmumutree->Branch("p_z0_CB", &p_z0_CB);
  p_z0_ME = new std::vector<float>();
  zmumutree->Branch("p_z0_ME", &p_z0_ME);
  p_z0_MSO = new std::vector<float>();
  zmumutree->Branch("p_z0_MSO", &p_z0_MSO);
  p_z0_MSOE = new std::vector<float>();
  zmumutree->Branch("p_z0_MSOE", &p_z0_MSOE);
  p_z0_ID = new std::vector<float>();
  zmumutree->Branch("p_z0_ID", &p_z0_ID);
  
  p_qOverP_CB = new std::vector<float>();
  zmumutree->Branch("p_qOverP_CB", &p_qOverP_CB);
  p_qOverP_ME = new std::vector<float>();
  zmumutree->Branch("p_qOverP_ME", &p_qOverP_ME);
  p_qOverP_MSO = new std::vector<float>();
  zmumutree->Branch("p_qOverP_MSO", &p_qOverP_MSO);
  p_qOverP_MSOE = new std::vector<float>();
  zmumutree->Branch("p_qOverP_MSOE", &p_qOverP_MSOE);
  p_qOverP_ID = new std::vector<float>();
  zmumutree->Branch("p_qOverP_ID", &p_qOverP_ID);
  
  p_dqOverP_CB = new std::vector<float>();
  zmumutree->Branch("p_dqOverP_CB", &p_dqOverP_CB);
  p_dqOverP_ME = new std::vector<float>();
  zmumutree->Branch("p_dqOverP_ME", &p_dqOverP_ME);
  p_dqOverP_MSO = new std::vector<float>();
  zmumutree->Branch("p_dqOverP_MSO", &p_dqOverP_MSO);
  p_dqOverP_MSOE = new std::vector<float>();
  zmumutree->Branch("p_dqOverP_MSOE", &p_dqOverP_MSOE);
  p_dqOverP_ID = new std::vector<float>();
  zmumutree->Branch("p_dqOverP_ID", &p_dqOverP_ID);

  p_rchi2_CB = new std::vector<float>();
  zmumutree->Branch("p_rchi2_CB", &p_rchi2_CB);
  p_rchi2_ME = new std::vector<float>();
  zmumutree->Branch("p_rchi2_ME", &p_rchi2_ME);
  p_rchi2_MSO = new std::vector<float>();
  zmumutree->Branch("p_rchi2_MSO", &p_rchi2_MSO);
  p_rchi2_MSOE = new std::vector<float>();
  zmumutree->Branch("p_rchi2_MSOE", &p_rchi2_MSOE);
  p_rchi2_ID = new std::vector<float>();
  zmumutree->Branch("p_rchi2_ID", &p_rchi2_ID);

  // negative muon track variables
  n_isCB = new std::vector<bool>();
  zmumutree->Branch("n_isCB", &n_isCB);
  n_isME = new std::vector<bool>();
  zmumutree->Branch("n_isME", &n_isME);
  n_isMSO = new std::vector<bool>();
  zmumutree->Branch("n_isMSO", &n_isMSO);
  n_isMSOE = new std::vector<bool>();
  zmumutree->Branch("n_isMSOE", &n_isMSOE);
  n_isID = new std::vector<bool>();
  zmumutree->Branch("n_isID", &n_isID);
  
  n_eta_CB = new std::vector<float>();
  zmumutree->Branch("n_eta_CB", &n_eta_CB);
  n_eta_ME = new std::vector<float>();
  zmumutree->Branch("n_eta_ME", &n_eta_ME); 
  n_eta_MSO = new std::vector<float>();
  zmumutree->Branch("n_eta_MSO", &n_eta_MSO); 
  n_eta_MSOE = new std::vector<float>();
  zmumutree->Branch("n_eta_MSOE", &n_eta_MSOE); 
  n_eta_ID = new std::vector<float>();
  zmumutree->Branch("n_eta_ID", &n_eta_ID); 

  n_phi_CB = new std::vector<float>();
  zmumutree->Branch("n_phi_CB", &n_phi_CB);
  n_phi_ME = new std::vector<float>();
  zmumutree->Branch("n_phi_ME", &n_phi_ME);
  n_phi_MSO = new std::vector<float>();
  zmumutree->Branch("n_phi_MSO", &n_phi_MSO);
  n_phi_MSOE = new std::vector<float>();
  zmumutree->Branch("n_phi_MSOE", &n_phi_MSOE);
  n_phi_ID = new std::vector<float>();
  zmumutree->Branch("n_phi_ID", &n_phi_ID);

  n_d0sig_CB = new std::vector<float>();
  zmumutree->Branch("n_d0sig_CB", &n_d0sig_CB);
  n_d0sig_ME = new std::vector<float>();
  zmumutree->Branch("n_d0sig_ME", &n_d0sig_ME);
  n_d0sig_MSO = new std::vector<float>();
  zmumutree->Branch("n_d0sig_MSO", &n_d0sig_MSO);
  n_d0sig_MSOE = new std::vector<float>();
  zmumutree->Branch("n_d0sig_MSOE", &n_d0sig_MSOE);
  n_d0sig_ID = new std::vector<float>();
  zmumutree->Branch("n_d0sig_ID", &n_d0sig_ID);

  n_z0_CB = new std::vector<float>();
  zmumutree->Branch("n_z0_CB", &n_z0_CB);
  n_z0_ME = new std::vector<float>();
  zmumutree->Branch("n_z0_ME", &n_z0_ME);
  n_z0_MSO = new std::vector<float>();
  zmumutree->Branch("n_z0_MSO", &n_z0_MSO);
  n_z0_MSOE = new std::vector<float>();
  zmumutree->Branch("n_z0_MSOE", &n_z0_MSOE);
  n_z0_ID = new std::vector<float>();
  zmumutree->Branch("n_z0_ID", &n_z0_ID);
  
  n_qOverP_CB = new std::vector<float>();
  zmumutree->Branch("n_qOverP_CB", &n_qOverP_CB);
  n_qOverP_ME = new std::vector<float>();
  zmumutree->Branch("n_qOverP_ME", &n_qOverP_ME);
  n_qOverP_MSO = new std::vector<float>();
  zmumutree->Branch("n_qOverP_MSO", &n_qOverP_MSO);
  n_qOverP_MSOE = new std::vector<float>();
  zmumutree->Branch("n_qOverP_MSOE", &n_qOverP_MSOE);
  n_qOverP_ID = new std::vector<float>();
  zmumutree->Branch("n_qOverP_ID", &n_qOverP_ID);
  
  n_dqOverP_CB = new std::vector<float>();
  zmumutree->Branch("n_dqOverP_CB", &n_dqOverP_CB);
  n_dqOverP_ME = new std::vector<float>();
  zmumutree->Branch("n_dqOverP_ME", &n_dqOverP_ME);
  n_dqOverP_MSO = new std::vector<float>();
  zmumutree->Branch("n_dqOverP_MSO", &n_dqOverP_MSO);
  n_dqOverP_MSOE = new std::vector<float>();
  zmumutree->Branch("n_dqOverP_MSOE", &n_dqOverP_MSOE);
  n_dqOverP_ID = new std::vector<float>();
  zmumutree->Branch("n_dqOverP_ID", &n_dqOverP_ID);

  n_rchi2_CB = new std::vector<float>();
  zmumutree->Branch("n_rchi2_CB", &n_rchi2_CB);
  n_rchi2_ME = new std::vector<float>();
  zmumutree->Branch("n_rchi2_ME", &n_rchi2_ME);
  n_rchi2_MSO = new std::vector<float>();
  zmumutree->Branch("n_rchi2_MSO", &n_rchi2_MSO);
  n_rchi2_MSOE = new std::vector<float>();
  zmumutree->Branch("n_rchi2_MSOE", &n_rchi2_MSOE);
  n_rchi2_ID = new std::vector<float>();
  zmumutree->Branch("n_rchi2_ID", &n_rchi2_ID);

  return StatusCode::SUCCESS;
}

// function for sorting xAOD::Muons by pt in descending order
struct CompareMuonPt
{
  bool operator()(const xAOD::Muon_v1*& m1, const xAOD::Muon_v1*& m2)
  {
    return m1->pt() > m2->pt();
  }
};

StatusCode MuonSelection :: execute ()
{
  // event info
  const xAOD::EventInfo *ei = nullptr;
  ANA_CHECK (evtStore()->retrieve(ei, "EventInfo"));

  // save event number
  eventNumber = ei->eventNumber();

  // check if event is MC or data
  bool isMC = false;
  if (ei->eventType(xAOD::EventInfo::IS_SIMULATION)) isMC = true;

  // check if data passes GRL
  passGRL = true;
  if (!isMC) {
    if (!m_grl->passRunLB(*ei)) {
      passGRL = false;
    }
  }

  // get muon container
  const xAOD::MuonContainer *muons = 0;
  ANA_CHECK (evtStore()->retrieve (muons, "Muons"));

  float pvtx_z = -1e6;
  bool correctVertex = false;

  /*// get vertex container
  const xAOD::VertexContainer *vtcs = 0;
  ANA_CHECK (evtStore()->retrieve (vtcs, "PrimaryVertices"));
 
  // find the primary vertex z coordinate
  if (correctVertex) {
    for (const xAOD::Vertex *vtx : *vtcs) {
      if (vtx->vertexType() == xAOD::VxType::PriVtx)
        pvtx_z = vtx->z();
    }
  }*/

  // skip event if no primary vertex found 
  if ( pvtx_z == -1e6 && correctVertex) {
    ANA_MSG_DEBUG("No primary vertex found! Skipping event.");
    return StatusCode::SUCCESS;
  }

  // event level
  nPositiveMuons = 0;
  nNegativeMuons = 0;

  // positive muon variables
	p_pdgID_truth->clear();
	p_pt_truth->clear();
	p_eta_truth->clear();
	p_phi_truth->clear();
	p_m_truth->clear();
  p_passIDcuts->clear();
  p_passAll->clear();
  p_ptcone40->clear();
  p_eLoss->clear();
  p_muonType->clear();
  p_quality->clear();
  p_primaryAuthor->clear();
  p_authors->clear();
  p_nPrecisionLayers->clear();
  p_nPrecisionHoleLayers->clear();  
  p_nPhiLayers->clear();  
  p_nPhiHoleLayers->clear();
  p_nTriggerLayers->clear();
  p_nTriggerHoleLayers->clear();
  p_innerSmallHits->clear();
  p_innerLargeHits->clear();
  p_middleSmallHits->clear();
  p_middleLargeHits->clear();
  p_outerSmallHits->clear();
  p_outerLargeHits->clear();
  p_extendedSmallHits->clear();
  p_extendedLargeHits->clear();
  p_innerSmallHoles->clear();
  p_innerLargeHoles->clear();
  p_middleSmallHoles->clear();
  p_middleLargeHoles->clear();
  p_outerSmallHoles->clear();
  p_outerLargeHoles->clear();
  p_extendedSmallHoles->clear();
  p_extendedLargeHoles->clear();

  // negative muon variables
	n_pdgID_truth->clear();
	n_pt_truth->clear();
	n_eta_truth->clear();
	n_phi_truth->clear();
	n_m_truth->clear();
  n_passIDcuts->clear();
  n_passAll->clear();
  n_ptcone40->clear();
  n_eLoss->clear();
  n_muonType->clear();
  n_quality->clear();
  n_primaryAuthor->clear();
  n_authors->clear();
  n_nPrecisionLayers->clear();
  n_nPrecisionHoleLayers->clear();  
  n_nPhiLayers->clear();  
  n_nPhiHoleLayers->clear();
  n_nTriggerLayers->clear();
  n_nTriggerHoleLayers->clear();
  n_innerSmallHits->clear();
  n_innerLargeHits->clear();
  n_middleSmallHits->clear();
  n_middleLargeHits->clear();
  n_outerSmallHits->clear();
  n_outerLargeHits->clear();
  n_extendedSmallHits->clear();
  n_extendedLargeHits->clear();
  n_innerSmallHoles->clear();
  n_innerLargeHoles->clear();
  n_middleSmallHoles->clear();
  n_middleLargeHoles->clear();
  n_outerSmallHoles->clear();
  n_outerLargeHoles->clear();
  n_extendedSmallHoles->clear();
  n_extendedLargeHoles->clear();

  // positive muon track variables
  p_isCB->clear();
  p_isME->clear();
  p_isMSO->clear();
  p_isMSOE->clear();
  p_isID->clear();
  p_eta_CB->clear();
  p_eta_ME->clear();
  p_eta_MSO->clear();
  p_eta_MSOE->clear();
  p_eta_ID->clear();
  p_phi_CB->clear();
  p_phi_ME->clear();
  p_phi_MSO->clear();
  p_phi_MSOE->clear();
  p_phi_ID->clear();
  p_z0_CB->clear();
  p_z0_ME->clear();
  p_z0_MSO->clear();
  p_z0_MSOE->clear();
  p_z0_ID->clear();
  p_d0sig_CB->clear();
  p_d0sig_ME->clear();
  p_d0sig_MSO->clear();
  p_d0sig_MSOE->clear();
  p_d0sig_ID->clear();
  p_qOverP_CB->clear();
  p_qOverP_ME->clear();
  p_qOverP_MSO->clear();
  p_qOverP_MSOE->clear();
  p_qOverP_ID->clear();
  p_dqOverP_CB->clear();
  p_dqOverP_ME->clear();
  p_dqOverP_MSO->clear();
  p_dqOverP_MSOE->clear();
  p_dqOverP_ID->clear();
  p_rchi2_CB->clear();
  p_rchi2_ME->clear();
  p_rchi2_MSO->clear();
  p_rchi2_MSOE->clear();
  p_rchi2_ID->clear();

  // negative muon track variables
  n_isCB->clear();
  n_isME->clear();
  n_isMSO->clear();
  n_isMSOE->clear();
  n_isID->clear();
  n_eta_CB->clear();
  n_eta_ME->clear();
  n_eta_MSO->clear();
  n_eta_MSOE->clear();
  n_eta_ID->clear();
  n_phi_CB->clear();
  n_phi_ME->clear();
  n_phi_MSO->clear();
  n_phi_MSOE->clear();
  n_phi_ID->clear();
  n_z0_CB->clear();
  n_z0_ME->clear();
  n_z0_MSO->clear();
  n_z0_MSOE->clear();
  n_z0_ID->clear();
  n_d0sig_CB->clear();
  n_d0sig_ME->clear();
  n_d0sig_MSO->clear();
  n_d0sig_MSOE->clear();
  n_d0sig_ID->clear();
  n_qOverP_CB->clear();
  n_qOverP_ME->clear();
  n_qOverP_MSO->clear();
  n_qOverP_MSOE->clear();
  n_qOverP_ID->clear();
  n_dqOverP_CB->clear();
  n_dqOverP_ME->clear();
  n_dqOverP_MSO->clear();
  n_dqOverP_MSOE->clear();
  n_dqOverP_ID->clear();
  n_rchi2_CB->clear();
  n_rchi2_ME->clear();
  n_rchi2_MSO->clear();
  n_rchi2_MSOE->clear();
  n_rchi2_ID->clear();

  positiveMuons->clear();
  negativeMuons->clear();

  // separate muons by charge
  for (const xAOD::Muon* muon : *muons) {
    if (muon->charge() > 0) {
      nPositiveMuons++;
      positiveMuons->push_back(muon);
    }
    if (muon->charge() < 0) {
      nNegativeMuons++;
      negativeMuons->push_back(muon);
    }
  }

  // sort muons in descending order of pt
  std::sort(positiveMuons->begin(), positiveMuons->end(), CompareMuonPt());
  std::sort(negativeMuons->begin(), negativeMuons->end(), CompareMuonPt());

  // positive muons
  for (const xAOD::Muon* muon : *positiveMuons) {
    p_passIDcuts->push_back(m_muonSelection->passedIDCuts(*muon));
    p_passAll->push_back(m_muonSelection->accept(*muon));
    p_muonType->push_back(muon->muonType());
    p_primaryAuthor->push_back(muon->author());
    p_quality->push_back(m_muonSelection->getQuality(*muon));

    const int unknown = 1<<0;
    const int MuidCo = 1<<1;
    const int STACO = 1<<2;
    const int MuTag = 1<<3;
    const int MuTagIMO = 1<<4;
    const int MuidSA = 1<<5;
    const int MuGirl = 1<<6;
    const int MuGirlLowBeta = 1<<7;
    const int CaloTag = 1<<8;
    const int CaloLikelihood = 1<<9;
    const int ExtrapolateMuonToIP = 1<<10;
    int authors = 0;
    if (muon->isAuthor(xAOD::Muon_v1::Author::unknown)) authors = authors | unknown;
    if (muon->isAuthor(xAOD::Muon_v1::Author::MuidCo)) authors = authors | MuidCo;
    if (muon->isAuthor(xAOD::Muon_v1::Author::STACO)) authors = authors | STACO;
    if (muon->isAuthor(xAOD::Muon_v1::Author::MuTag)) authors = authors | MuTag;
    if (muon->isAuthor(xAOD::Muon_v1::Author::MuTagIMO)) authors = authors | MuTagIMO;
    if (muon->isAuthor(xAOD::Muon_v1::Author::MuidSA)) authors = authors | MuidSA;
    if (muon->isAuthor(xAOD::Muon_v1::Author::MuGirl)) authors = authors | MuGirl;
    if (muon->isAuthor(xAOD::Muon_v1::Author::MuGirlLowBeta)) authors = authors | MuGirlLowBeta;
    if (muon->isAuthor(xAOD::Muon_v1::Author::CaloTag)) authors = authors | CaloTag;
    if (muon->isAuthor(xAOD::Muon_v1::Author::CaloLikelihood)) authors = authors | CaloLikelihood;
    if (muon->isAuthor(xAOD::Muon_v1::Author::ExtrapolateMuonToIP)) authors = authors | ExtrapolateMuonToIP;
    p_authors->push_back(authors);

    float isopt = -1;
    if (!muon->isolation(isopt, xAOD::Iso::ptcone40))
      ANA_MSG_DEBUG ("Muon isolation not available");
    p_ptcone40->push_back(isopt * 0.001); // GeV
    
    float eLoss = 0.;
    if (!muon->parameter(eLoss, xAOD::Muon::EnergyLoss) )
      ANA_MSG_DEBUG ("Muon energy loss not available");
    p_eLoss->push_back(eLoss * 0.001); // GeV

    p_nPrecisionLayers->push_back(muon->uint8SummaryValue(xAOD::SummaryType::numberOfPrecisionLayers));
    p_nPrecisionHoleLayers->push_back(muon->uint8SummaryValue(xAOD::SummaryType::numberOfPrecisionHoleLayers));
    p_nPhiLayers->push_back(muon->uint8SummaryValue(xAOD::SummaryType::numberOfPhiLayers));
    p_nPhiHoleLayers->push_back(muon->uint8SummaryValue(xAOD::SummaryType::numberOfPhiHoleLayers));
    p_nTriggerLayers->push_back(muon->uint8SummaryValue(xAOD::SummaryType::numberOfTriggerEtaLayers));
    p_nTriggerHoleLayers->push_back(muon->uint8SummaryValue(xAOD::SummaryType::numberOfTriggerEtaHoleLayers));
    p_innerSmallHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::innerSmallHits));
    p_innerLargeHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::innerLargeHits));
    p_middleSmallHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::middleSmallHits));
    p_middleLargeHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::middleLargeHits));
    p_outerSmallHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::outerSmallHits));
    p_outerLargeHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::outerLargeHits));
    p_extendedSmallHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::extendedSmallHits));
    p_extendedLargeHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::extendedLargeHits));
    p_innerSmallHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::innerSmallHoles));
    p_innerLargeHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::innerLargeHoles));
    p_middleSmallHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::middleSmallHoles));
    p_middleLargeHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::middleLargeHoles));
    p_outerSmallHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::outerSmallHoles));
    p_outerLargeHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::outerLargeHoles));
    p_extendedSmallHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::extendedSmallHoles));
    p_extendedLargeHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::extendedLargeHoles));
  
		// Truth information
		muon_truth = nullptr;
		if(muon->isAvailable<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink")) {
		  ElementLink<xAOD::TruthParticleContainer> link = muon->auxdata<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink");
			  if(link.isValid())
				     muon_truth = *link;
		}
		if (muon_truth) {
			p_pdgID_truth->push_back(muon_truth->pdgId());
			p_pt_truth->push_back(muon_truth->pt());
			p_eta_truth->push_back(muon_truth->eta());
			p_phi_truth->push_back(muon_truth->phi());
			p_m_truth->push_back(muon_truth->m());
		}
		else {
			p_pdgID_truth->push_back(-1e6);
			p_pt_truth->push_back(-1e6);
			p_eta_truth->push_back(-1e6);
			p_phi_truth->push_back(-1e6);
			p_m_truth->push_back(-1e6);
		}

    // CB tracks
    if ((muon->combinedTrackParticleLink())) {
      p_isCB->push_back(true);
      CB = *muon->combinedTrackParticleLink(); 
      p_eta_CB->push_back(CB->eta());
      p_phi_CB->push_back(CB->phi());
      if (CB->definingParametersCovMatrixVec().at(0) > 0) {
        p_d0sig_CB->push_back(xAOD::TrackingHelpers::d0significance(CB,
                            ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
      }
      else {
        ANA_MSG_INFO ("Positive CB track particle has d0-uncertainty" << CB->definingParametersCovMatrixVec().at(0));
        p_d0sig_CB->push_back(-1e5);    
      }

      if (correctVertex) 
        p_z0_CB->push_back(CB->z0() + CB->vz() - pvtx_z);
      else 
        p_z0_CB->push_back(CB->z0());
      
      p_qOverP_CB->push_back(CB->qOverP() / 0.001); // GeV^-1
      p_dqOverP_CB->push_back(sqrt(CB->definingParametersCovMatrix()(4,4)) / 0.001); // GeV^-1
      p_rchi2_CB->push_back(CB->chiSquared() / CB->numberDoF());
    }
    else {
      p_isCB->push_back(false);
      p_eta_CB->push_back(-1e6);
      p_phi_CB->push_back(-1e6);
      p_d0sig_CB->push_back(-1e6);
      p_z0_CB->push_back(-1e6);
      p_qOverP_CB->push_back(-1e6);
      p_dqOverP_CB->push_back(-1e6);
      p_rchi2_CB->push_back(-1e6);
    } 

    // ME tracks
    if ((muon->extrapolatedMuonSpectrometerTrackParticleLink())) {
      p_isME->push_back(true);
      ME = *muon->extrapolatedMuonSpectrometerTrackParticleLink(); 
      p_eta_ME->push_back(ME->eta());
      p_phi_ME->push_back(ME->phi());
      if (ME->definingParametersCovMatrixVec().at(0) > 0) {
        p_d0sig_ME->push_back(xAOD::TrackingHelpers::d0significance(ME,
                            ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
      }
      else {
        ANA_MSG_INFO ("Positive ME track particle has d0-uncertainty" << ME->definingParametersCovMatrixVec().at(0));
        p_d0sig_ME->push_back(-1e5);    
      }

      if (correctVertex) 
        p_z0_ME->push_back(ME->z0() + ME->vz() - pvtx_z);
      else 
        p_z0_ME->push_back(ME->z0());
      
      p_qOverP_ME->push_back(ME->qOverP() / 0.001); // GeV^-1
      p_dqOverP_ME->push_back(sqrt(ME->definingParametersCovMatrix()(4,4)) / 0.001); // GeV^-1
      p_rchi2_ME->push_back(ME->chiSquared() / ME->numberDoF());
    }
    else {
      p_isME->push_back(false);
      p_eta_ME->push_back(-1e6);
      p_phi_ME->push_back(-1e6);
      p_d0sig_ME->push_back(-1e6);
      p_z0_ME->push_back(-1e6);
      p_qOverP_ME->push_back(-1e6);
      p_dqOverP_ME->push_back(-1e6);
      p_rchi2_ME->push_back(-1e6);
    }
 
    // MSO tracks
    if ((muon->muonSpectrometerTrackParticleLink())) {
      p_isMSO->push_back(true);
      MSO = *muon->muonSpectrometerTrackParticleLink(); 
      p_eta_MSO->push_back(MSO->eta());
      p_phi_MSO->push_back(MSO->phi());
      if (MSO->definingParametersCovMatrixVec().at(0) > 0) {
        p_d0sig_MSO->push_back(xAOD::TrackingHelpers::d0significance(MSO,
                            ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
      }
      else {
        ANA_MSG_INFO ("Positive MSO track particle has d0-uncertainty" << MSO->definingParametersCovMatrixVec().at(0));
        p_d0sig_MSO->push_back(-1e5);    
      }

      if (correctVertex) 
        p_z0_MSO->push_back(MSO->z0() + MSO->vz() - pvtx_z);
      else 
        p_z0_MSO->push_back(MSO->z0());
      
      p_qOverP_MSO->push_back(MSO->qOverP() / 0.001); // GeV^-1
      p_dqOverP_MSO->push_back(sqrt(MSO->definingParametersCovMatrix()(4,4)) / 0.001); // GeV^-1
      p_rchi2_MSO->push_back(MSO->chiSquared() / MSO->numberDoF());
    }
    else {
      p_isMSO->push_back(false);
      p_eta_MSO->push_back(-1e6);
      p_phi_MSO->push_back(-1e6);
      p_d0sig_MSO->push_back(-1e6);
      p_z0_MSO->push_back(-1e6);
      p_qOverP_MSO->push_back(-1e6);
      p_dqOverP_MSO->push_back(-1e6);
      p_rchi2_MSO->push_back(-1e6);
    }
  
    // MSOE tracks
    if ((muon->msOnlyExtrapolatedMuonSpectrometerTrackParticleLink())) {
      p_isMSOE->push_back(true);
      MSOE = *muon->msOnlyExtrapolatedMuonSpectrometerTrackParticleLink(); 
      p_eta_MSOE->push_back(MSOE->eta());
      p_phi_MSOE->push_back(MSOE->phi());
      if (MSOE->definingParametersCovMatrixVec().at(0) > 0) {
        p_d0sig_MSOE->push_back(xAOD::TrackingHelpers::d0significance(MSOE,
                            ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
      }
      else {
        ANA_MSG_INFO ("Positive MSOE track particle has d0-uncertainty" << MSOE->definingParametersCovMatrixVec().at(0));
        p_d0sig_MSOE->push_back(-1e5);    
      }

      if (correctVertex) 
        p_z0_MSOE->push_back(MSOE->z0() + MSOE->vz() - pvtx_z);
      else 
        p_z0_MSOE->push_back(MSOE->z0());
      
      p_qOverP_MSOE->push_back(MSOE->qOverP() / 0.001); // GeV^-1
      p_dqOverP_MSOE->push_back(sqrt(MSOE->definingParametersCovMatrix()(4,4)) / 0.001); // GeV^-1
      p_rchi2_MSOE->push_back(MSOE->chiSquared() / MSOE->numberDoF());
    }
    else {
      p_isMSOE->push_back(false);
      p_eta_MSOE->push_back(-1e6);
      p_phi_MSOE->push_back(-1e6);
      p_d0sig_MSOE->push_back(-1e6);
      p_z0_MSOE->push_back(-1e6);
      p_qOverP_MSOE->push_back(-1e6);
      p_dqOverP_MSOE->push_back(-1e6);
      p_rchi2_MSOE->push_back(-1e6);
    }
  
    // ID tracks
    if ((muon->inDetTrackParticleLink())) {
      p_isID->push_back(true);
      ID = *muon->inDetTrackParticleLink(); 
      p_eta_ID->push_back(ID->eta());
      p_phi_ID->push_back(ID->phi());
      if (ID->definingParametersCovMatrixVec().at(0) > 0) {
        p_d0sig_ID->push_back(xAOD::TrackingHelpers::d0significance(ID,
                            ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
      }
      else {
        ANA_MSG_INFO ("Positive ID track particle has d0-uncertainty" << ID->definingParametersCovMatrixVec().at(0));
        p_d0sig_ID->push_back(-1e5);    
      }

      if (correctVertex) 
        p_z0_ID->push_back(ID->z0() + ID->vz() - pvtx_z);
      else 
        p_z0_ID->push_back(ID->z0());
      
      p_qOverP_ID->push_back(ID->qOverP() / 0.001); // GeV^-1
      p_dqOverP_ID->push_back(sqrt(ID->definingParametersCovMatrix()(4,4)) / 0.001); // GeV^-1
      p_rchi2_ID->push_back(ID->chiSquared() / ID->numberDoF());
    }
    else {
      p_isID->push_back(false);
      p_eta_ID->push_back(-1e6);
      p_phi_ID->push_back(-1e6);
      p_d0sig_ID->push_back(-1e6);
      p_z0_ID->push_back(-1e6);
      p_qOverP_ID->push_back(-1e6);
      p_dqOverP_ID->push_back(-1e6);
      p_rchi2_ID->push_back(-1e6);
    }
  }

  // negative muons
  for (const xAOD::Muon* muon : *negativeMuons) {
    n_passIDcuts->push_back(m_muonSelection->passedIDCuts(*muon));
    n_passAll->push_back(m_muonSelection->accept(*muon));
    n_muonType->push_back(muon->muonType());
    n_primaryAuthor->push_back(muon->author());
    n_quality->push_back(m_muonSelection->getQuality(*muon));
  
    const int unknown = 1<<0;
    const int MuidCo = 1<<1;
    const int STACO = 1<<2;
    const int MuTag = 1<<3;
    const int MuTagIMO = 1<<4;
    const int MuidSA = 1<<5;
    const int MuGirl = 1<<6;
    const int MuGirlLowBeta = 1<<7;
    const int CaloTag = 1<<8;
    const int CaloLikelihood = 1<<9;
    const int ExtrapolateMuonToIP = 1<<10;
    int authors = 0;
    if (muon->isAuthor(xAOD::Muon_v1::Author::unknown)) authors = authors | unknown;
    if (muon->isAuthor(xAOD::Muon_v1::Author::MuidCo)) authors = authors | MuidCo;
    if (muon->isAuthor(xAOD::Muon_v1::Author::STACO)) authors = authors | STACO;
    if (muon->isAuthor(xAOD::Muon_v1::Author::MuTag)) authors = authors | MuTag;
    if (muon->isAuthor(xAOD::Muon_v1::Author::MuTagIMO)) authors = authors | MuTagIMO;
    if (muon->isAuthor(xAOD::Muon_v1::Author::MuidSA)) authors = authors | MuidSA;
    if (muon->isAuthor(xAOD::Muon_v1::Author::MuGirl)) authors = authors | MuGirl;
    if (muon->isAuthor(xAOD::Muon_v1::Author::MuGirlLowBeta)) authors = authors | MuGirlLowBeta;
    if (muon->isAuthor(xAOD::Muon_v1::Author::CaloTag)) authors = authors | CaloTag;
    if (muon->isAuthor(xAOD::Muon_v1::Author::CaloLikelihood)) authors = authors | CaloLikelihood;
    if (muon->isAuthor(xAOD::Muon_v1::Author::ExtrapolateMuonToIP)) authors = authors | ExtrapolateMuonToIP;
    n_authors->push_back(authors);

    float isopt = -1;
    if (!muon->isolation(isopt, xAOD::Iso::ptcone40))
      ANA_MSG_DEBUG ("Muon isolation not available");
    n_ptcone40->push_back(isopt * 0.001); // GeV
    
    float eLoss = 0.;
    if (!muon->parameter(eLoss, xAOD::Muon::EnergyLoss) )
      ANA_MSG_DEBUG ("Muon energy loss not available");
    n_eLoss->push_back(eLoss * 0.001); // GeV

    n_nPrecisionLayers->push_back(muon->uint8SummaryValue(xAOD::SummaryType::numberOfPrecisionLayers));
    n_nPrecisionHoleLayers->push_back(muon->uint8SummaryValue(xAOD::SummaryType::numberOfPrecisionHoleLayers));
    n_nPhiLayers->push_back(muon->uint8SummaryValue(xAOD::SummaryType::numberOfPhiLayers));
    n_nPhiHoleLayers->push_back(muon->uint8SummaryValue(xAOD::SummaryType::numberOfPhiHoleLayers));
    n_nTriggerLayers->push_back(muon->uint8SummaryValue(xAOD::SummaryType::numberOfTriggerEtaLayers));
    n_nTriggerHoleLayers->push_back(muon->uint8SummaryValue(xAOD::SummaryType::numberOfTriggerEtaHoleLayers));
    n_innerSmallHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::innerSmallHits));
    n_innerLargeHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::innerLargeHits));
    n_middleSmallHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::middleSmallHits));
    n_middleLargeHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::middleLargeHits));
    n_outerSmallHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::outerSmallHits));
    n_outerLargeHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::outerLargeHits));
    n_extendedSmallHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::extendedSmallHits));
    n_extendedLargeHits->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::extendedLargeHits));
    n_innerSmallHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::innerSmallHoles));
    n_innerLargeHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::innerLargeHoles));
    n_middleSmallHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::middleSmallHoles));
    n_middleLargeHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::middleLargeHoles));
    n_outerSmallHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::outerSmallHoles));
    n_outerLargeHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::outerLargeHoles));
    n_extendedSmallHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::extendedSmallHoles));
    n_extendedLargeHoles->push_back(muon->uint8MuonSummaryValue(xAOD::MuonSummaryType::extendedLargeHoles));
    
		// Truth information
		muon_truth = nullptr;
		if(muon->isAvailable<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink")) {
		  ElementLink<xAOD::TruthParticleContainer> link = muon->auxdata<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink");
			  if(link.isValid())
				     muon_truth = *link;
		}
		if (muon_truth) {
			n_pdgID_truth->push_back(muon_truth->pdgId());
			n_pt_truth->push_back(muon_truth->pt());
			n_eta_truth->push_back(muon_truth->eta());
			n_phi_truth->push_back(muon_truth->phi());
			n_m_truth->push_back(muon_truth->m());
		}
		else {
			n_pdgID_truth->push_back(-1e6);
			n_pt_truth->push_back(-1e6);
			n_eta_truth->push_back(-1e6);
			n_phi_truth->push_back(-1e6);
			n_m_truth->push_back(-1e6);
		}

    // CB tracks
    if ((muon->combinedTrackParticleLink())) {
      n_isCB->push_back(true);
      CB = *muon->combinedTrackParticleLink(); 
      n_eta_CB->push_back(CB->eta());
      n_phi_CB->push_back(CB->phi());
      if (CB->definingParametersCovMatrixVec().at(0) > 0) {
        n_d0sig_CB->push_back(xAOD::TrackingHelpers::d0significance(CB,
                            ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
      }
      else {
        ANA_MSG_INFO ("Negative CB track particle has d0-uncertainty" << CB->definingParametersCovMatrixVec().at(0));
        n_d0sig_CB->push_back(-1e5);    
      }

      if (correctVertex) 
        n_z0_CB->push_back(CB->z0() + CB->vz() - pvtx_z);
      else 
        n_z0_CB->push_back(CB->z0());
      
      n_qOverP_CB->push_back(CB->qOverP() / 0.001); // GeV^-1
      n_dqOverP_CB->push_back(sqrt(CB->definingParametersCovMatrix()(4,4)) / 0.001); // GeV^-1
      n_rchi2_CB->push_back(CB->chiSquared() / CB->numberDoF());
    }
    else {
      n_isCB->push_back(false);
      n_eta_CB->push_back(-1e6);
      n_phi_CB->push_back(-1e6);
      n_d0sig_CB->push_back(-1e6);
      n_z0_CB->push_back(-1e6);
      n_qOverP_CB->push_back(-1e6);
      n_dqOverP_CB->push_back(-1e6);
      n_rchi2_CB->push_back(-1e6);
    } 

    // ME tracks
    if ((muon->extrapolatedMuonSpectrometerTrackParticleLink())) {
      n_isME->push_back(true);
      ME = *muon->extrapolatedMuonSpectrometerTrackParticleLink(); 
      n_eta_ME->push_back(ME->eta());
      n_phi_ME->push_back(ME->phi());
      if (ME->definingParametersCovMatrixVec().at(0) > 0) {
        n_d0sig_ME->push_back(xAOD::TrackingHelpers::d0significance(ME,
                            ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
      }
      else {
        ANA_MSG_INFO ("Negative ME track particle has d0-uncertainty" << ME->definingParametersCovMatrixVec().at(0));
        n_d0sig_ME->push_back(-1e5);    
      }

      if (correctVertex) 
        n_z0_ME->push_back(ME->z0() + ME->vz() - pvtx_z);
      else 
        n_z0_ME->push_back(ME->z0());
      
      n_qOverP_ME->push_back(ME->qOverP() / 0.001); // GeV^-1
      n_dqOverP_ME->push_back(sqrt(ME->definingParametersCovMatrix()(4,4)) / 0.001); // GeV^-1
      n_rchi2_ME->push_back(ME->chiSquared() / ME->numberDoF());
    }
    else {
      n_isME->push_back(false);
      n_eta_ME->push_back(-1e6);
      n_phi_ME->push_back(-1e6);
      n_d0sig_ME->push_back(-1e6);
      n_z0_ME->push_back(-1e6);
      n_qOverP_ME->push_back(-1e6);
      n_dqOverP_ME->push_back(-1e6);
      n_rchi2_ME->push_back(-1e6);
    }
 
    // MSO tracks
    if ((muon->muonSpectrometerTrackParticleLink())) {
      n_isMSO->push_back(true);
      MSO = *muon->muonSpectrometerTrackParticleLink(); 
      n_eta_MSO->push_back(MSO->eta());
      n_phi_MSO->push_back(MSO->phi());
      if (MSO->definingParametersCovMatrixVec().at(0) > 0) {
        n_d0sig_MSO->push_back(xAOD::TrackingHelpers::d0significance(MSO,
                            ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
      }
      else {
        ANA_MSG_INFO ("Negative MSO track particle has d0-uncertainty" << MSO->definingParametersCovMatrixVec().at(0));
        n_d0sig_MSO->push_back(-1e5);    
      }
  
      if (correctVertex) 
        n_z0_MSO->push_back(MSO->z0() + MSO->vz() - pvtx_z);
      else 
        n_z0_MSO->push_back(MSO->z0());
      
      n_qOverP_MSO->push_back(MSO->qOverP() / 0.001); // GeV^-1
      n_dqOverP_MSO->push_back(sqrt(MSO->definingParametersCovMatrix()(4,4)) / 0.001); // GeV^-1
      n_rchi2_MSO->push_back(MSO->chiSquared() / MSO->numberDoF());
    }
    else {
      n_isMSO->push_back(false);
      n_eta_MSO->push_back(-1e6);
      n_phi_MSO->push_back(-1e6);
      n_d0sig_MSO->push_back(-1e6);
      n_z0_MSO->push_back(-1e6);
      n_qOverP_MSO->push_back(-1e6);
      n_dqOverP_MSO->push_back(-1e6);
      n_rchi2_MSO->push_back(-1e6);
    }
  
    // MSOE tracks
    if ((muon->msOnlyExtrapolatedMuonSpectrometerTrackParticleLink())) {
      n_isMSOE->push_back(true);
      MSOE = *muon->msOnlyExtrapolatedMuonSpectrometerTrackParticleLink(); 
      n_eta_MSOE->push_back(MSOE->eta());
      n_phi_MSOE->push_back(MSOE->phi());
      if (MSOE->definingParametersCovMatrixVec().at(0) > 0) {
        n_d0sig_MSOE->push_back(xAOD::TrackingHelpers::d0significance(MSOE,
                            ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
      }
      else {
        ANA_MSG_INFO ("Negative MSOE track particle has d0-uncertainty" << MSOE->definingParametersCovMatrixVec().at(0));
        n_d0sig_MSOE->push_back(-1e5);    
      }
  
      if (correctVertex) 
        n_z0_MSOE->push_back(MSOE->z0() + MSOE->vz() - pvtx_z);
      else 
        n_z0_MSOE->push_back(MSOE->z0());
      
      n_qOverP_MSOE->push_back(MSOE->qOverP() / 0.001); // GeV^-1
      n_dqOverP_MSOE->push_back(sqrt(MSOE->definingParametersCovMatrix()(4,4)) / 0.001); // GeV^-1
      n_rchi2_MSOE->push_back(MSOE->chiSquared() / MSOE->numberDoF());
    }
    else {
      n_isMSOE->push_back(false);
      n_eta_MSOE->push_back(-1e6);
      n_phi_MSOE->push_back(-1e6);
      n_d0sig_MSOE->push_back(-1e6);
      n_z0_MSOE->push_back(-1e6);
      n_qOverP_MSOE->push_back(-1e6);
      n_dqOverP_MSOE->push_back(-1e6);
      n_rchi2_MSOE->push_back(-1e6);
    }
  
    // ID tracks
    if ((muon->inDetTrackParticleLink())) {
      n_isID->push_back(true);
      ID = *muon->inDetTrackParticleLink(); 
      n_eta_ID->push_back(ID->eta());
      n_phi_ID->push_back(ID->phi());
      if (ID->definingParametersCovMatrixVec().at(0) > 0) {
        n_d0sig_ID->push_back(xAOD::TrackingHelpers::d0significance(ID,
                            ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
      }
      else {
        ANA_MSG_INFO ("Negative ID track particle has d0-uncertainty" << ID->definingParametersCovMatrixVec().at(0));
        n_d0sig_ID->push_back(-1e5);
      }

      if (correctVertex) 
        n_z0_ID->push_back(ID->z0() + ID->vz() - pvtx_z);
      else 
        n_z0_ID->push_back(ID->z0());
      
      n_qOverP_ID->push_back(ID->qOverP() / 0.001); // GeV^-1
      n_dqOverP_ID->push_back(sqrt(ID->definingParametersCovMatrix()(4,4)) / 0.001); // GeV^-1
      n_rchi2_ID->push_back(ID->chiSquared() / ID->numberDoF());
    }
    else {
      n_isID->push_back(false);
      n_eta_ID->push_back(-1e6);
      n_phi_ID->push_back(-1e6);
      n_d0sig_ID->push_back(-1e6);
      n_z0_ID->push_back(-1e6);
      n_qOverP_ID->push_back(-1e6);
      n_dqOverP_ID->push_back(-1e6);
      n_rchi2_ID->push_back(-1e6);
    }
  }
  
  tree ("RecoMuons")->Fill();

  return StatusCode::SUCCESS;
}



StatusCode MuonSelection :: finalize ()
{
  return StatusCode::SUCCESS;
}
