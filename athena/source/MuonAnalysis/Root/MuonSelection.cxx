#include <AsgTools/MessageCheck.h>
#include <MuonAnalysis/MuonSelection.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODMuon/MuonContainer.h>
#include <xAODTracking/TrackParticlexAODHelpers.h>
#include <xAODTracking/VertexContainer.h>
#include <TSystem.h>
#include <math.h>

MuonSelection :: MuonSelection (const std::string& name,
                                  ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm (name, pSvcLocator),
      m_grl ("GoodRunsListSelectionTool/grl", this), 
      m_muonSelection ("CP::MuonSelectionTool", this),
      muon_1 (),
      muon_2 ()
{
}

MuonSelection :: ~MuonSelection () {
  if (m_pt_CB) delete m_pt_CB;
  if (m_pt_ME) delete m_pt_ME;
  if (m_pt_MS) delete m_pt_MS;
  if (m_pt_ID) delete m_pt_ID;
  if (m_eta_CB) delete m_eta_CB;
  if (m_eta_ME) delete m_eta_ME;
  if (m_eta_MS) delete m_eta_MS;
  if (m_eta_ID) delete m_eta_ID;
  if (m_phi_CB) delete m_phi_CB;
  if (m_phi_ME) delete m_phi_ME;
  if (m_phi_MS) delete m_phi_MS;
  if (m_phi_ID) delete m_phi_ID;
  if (m_d0sig_CB) delete m_d0sig_CB;
  if (m_d0sig_ME) delete m_d0sig_ME;
  if (m_d0sig_MS) delete m_d0sig_MS;
  if (m_d0sig_ID) delete m_d0sig_ID;
  if (m_z0_CB) delete m_z0_CB;
  if (m_z0_ME) delete m_z0_ME;
  if (m_z0_MS) delete m_z0_MS;
  if (m_z0_ID) delete m_z0_ID;
  if (m_e_CB) delete m_e_CB;
  if (m_e_ME) delete m_e_ME;
  if (m_e_MS) delete m_e_MS; 
  if (m_e_ID) delete m_e_ID;
  if (m_charge_CB) delete m_charge_CB;
  if (m_charge_ME) delete m_charge_ME;
  if (m_charge_MS) delete m_charge_MS;
  if (m_charge_ID) delete m_charge_ID;
  if (m_qOverP_CB) delete m_qOverP_CB;
  if (m_qOverP_ME) delete m_qOverP_ME;
  if (m_qOverP_MS) delete m_qOverP_MS;
  if (m_qOverP_ID) delete m_qOverP_ID;
  if (m_dqOverP_CB) delete m_dqOverP_CB;
  if (m_dqOverP_ME) delete m_dqOverP_ME;
  if (m_dqOverP_MS) delete m_dqOverP_MS;
  if (m_dqOverP_ID) delete m_dqOverP_ID;
  if (m_chi2_CB) delete m_chi2_CB;
  if (m_chi2_ME) delete m_chi2_ME;
  if (m_chi2_MS) delete m_chi2_MS;
  if (m_chi2_ID) delete m_chi2_ID;
  if (m_dof_CB) delete m_dof_CB;
  if (m_dof_ME) delete m_dof_ME;
  if (m_dof_MS) delete m_dof_MS;
  if (m_dof_ID) delete m_dof_ID;
  if (m_eLoss) delete m_eLoss;
  if (m_muonType) delete m_muonType;
  if (m_quality) delete m_quality;
  if (m_author) delete m_author;
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
  ANA_CHECK(book (TTree ("SelectedZDecayMuons", "Selected Z Decay Muon Pairs")));
  TTree* zmumutree = tree ("SelectedZDecayMuons");
  ANA_CHECK(book (TTree ("BackgroundMuons", "Low M muon pairs")));
  TTree* vetotree = tree ("BackgroundMuons");

  // set branch addresses for zmumutree  
  m_pt_CB = new std::vector<float>();
  zmumutree->Branch("Pt_CB", &m_pt_CB);
  m_pt_ME = new std::vector<float>();
  zmumutree->Branch("Pt_ME", &m_pt_ME);
  m_pt_MS = new std::vector<float>();
  zmumutree->Branch("Pt_MS", &m_pt_MS);
  m_pt_ID = new std::vector<float>();
  zmumutree->Branch("Pt_ID", &m_pt_ID);

  m_eta_CB = new std::vector<float>();
  zmumutree->Branch("Eta_CB", &m_eta_CB);
  m_eta_ME = new std::vector<float>();
  zmumutree->Branch("Eta_ME", &m_eta_ME); 
  m_eta_MS = new std::vector<float>();
  zmumutree->Branch("Eta_MS", &m_eta_MS); 
  m_eta_ID = new std::vector<float>();
  zmumutree->Branch("Eta_ID", &m_eta_ID); 

  m_phi_CB = new std::vector<float>();
  zmumutree->Branch("Phi_CB", &m_phi_CB);
  m_phi_ME = new std::vector<float>();
  zmumutree->Branch("Phi_ME", &m_phi_ME);
  m_phi_MS = new std::vector<float>();
  zmumutree->Branch("Phi_MS", &m_phi_MS);
  m_phi_ID = new std::vector<float>();
  zmumutree->Branch("Phi_ID", &m_phi_ID);

  m_d0sig_CB = new std::vector<float>();
  zmumutree->Branch("d0sig_CB", &m_d0sig_CB);
  m_d0sig_ME = new std::vector<float>();
  zmumutree->Branch("d0sig_ME", &m_d0sig_ME);
  m_d0sig_MS = new std::vector<float>();
  zmumutree->Branch("d0sig_MS", &m_d0sig_MS);
  m_d0sig_ID = new std::vector<float>();
  zmumutree->Branch("d0sig_ID", &m_d0sig_ID);

  m_z0_CB = new std::vector<float>();
  zmumutree->Branch("z0_CB", &m_z0_CB);
  m_z0_ME = new std::vector<float>();
  zmumutree->Branch("z0_ME", &m_z0_ME);
  m_z0_MS = new std::vector<float>();
  zmumutree->Branch("z0_MS", &m_z0_MS);
  m_z0_ID = new std::vector<float>();
  zmumutree->Branch("z0_ID", &m_z0_ID);
  
  m_qOverP_CB = new std::vector<float>();
  zmumutree->Branch("qOverP_CB", &m_qOverP_CB);
  m_qOverP_ME = new std::vector<float>();
  zmumutree->Branch("qOverP_ME", &m_qOverP_ME);
  m_qOverP_MS = new std::vector<float>();
  zmumutree->Branch("qOverP_MS", &m_qOverP_MS);
  m_qOverP_ID = new std::vector<float>();
  zmumutree->Branch("qOverP_ID", &m_qOverP_ID);
  
  m_dqOverP_CB = new std::vector<float>();
  zmumutree->Branch("dqOverP_CB", &m_dqOverP_CB);
  m_dqOverP_ME = new std::vector<float>();
  zmumutree->Branch("dqOverP_ME", &m_dqOverP_ME);
  m_dqOverP_MS = new std::vector<float>();
  zmumutree->Branch("dqOverP_MS", &m_dqOverP_MS);
  m_dqOverP_ID = new std::vector<float>();
  zmumutree->Branch("dqOverP_ID", &m_dqOverP_ID);

  m_chi2_CB = new std::vector<float>();
  zmumutree->Branch("chi2_CB", &m_chi2_CB);
  m_chi2_ME = new std::vector<float>();
  zmumutree->Branch("chi2_ME", &m_chi2_ME);
  m_chi2_MS = new std::vector<float>();
  zmumutree->Branch("chi2_MS", &m_chi2_MS);
  m_chi2_ID = new std::vector<float>();
  zmumutree->Branch("chi2_ID", &m_chi2_ID);

  m_dof_CB = new std::vector<float>();
  zmumutree->Branch("dof_CB", &m_dof_CB);
  m_dof_ME = new std::vector<float>();
  zmumutree->Branch("dof_ME", &m_dof_ME);
  m_dof_MS = new std::vector<float>();
  zmumutree->Branch("dof_MS", &m_dof_MS);
  m_dof_ID = new std::vector<float>();
  zmumutree->Branch("dof_ID", &m_dof_ID);

  zmumutree->Branch("m_CB", &m_m_CB);
  zmumutree->Branch("m_ME", &m_m_ME);
  zmumutree->Branch("m_MS", &m_m_MS);
  zmumutree->Branch("m_ID", &m_m_ID);
  
  m_e_CB = new std::vector<float>();
  zmumutree->Branch("E_CB", &m_e_CB);
  m_e_ME = new std::vector<float>();
  zmumutree->Branch("E_ME", &m_e_ME);
  m_e_MS = new std::vector<float>();
  zmumutree->Branch("E_MS", &m_e_MS);
  m_e_ID = new std::vector<float>();
  zmumutree->Branch("E_ID", &m_e_ID);
  
  m_charge_CB = new std::vector<float>();
  zmumutree->Branch("Charge_CB", &m_charge_CB);
  m_charge_ME = new std::vector<float>();
  zmumutree->Branch("Charge_ME", &m_charge_ME);
  m_charge_MS = new std::vector<float>();
  zmumutree->Branch("Charge_MS", &m_charge_MS); 
  m_charge_ID = new std::vector<float>();
  zmumutree->Branch("Charge_ID", &m_charge_ID);

  m_eLoss = new std::vector<float>();
  zmumutree->Branch("EnergyLoss",&m_eLoss);
  m_muonType = new std::vector<int>();
  zmumutree->Branch("MuonType", &m_muonType);
  m_quality = new std::vector<int>();
  zmumutree->Branch("Quality", &m_quality);
  m_author = new std::vector<int>();
  zmumutree->Branch("Author", &m_author);

  // set branch addresses for vetotree  
  m_pt_CB = new std::vector<float>();
  vetotree->Branch("Pt_CB", &m_pt_CB);
  m_pt_ME = new std::vector<float>();
  vetotree->Branch("Pt_ME", &m_pt_ME);
  m_pt_MS = new std::vector<float>();
  vetotree->Branch("Pt_MS", &m_pt_MS);
  m_pt_ID = new std::vector<float>();
  vetotree->Branch("Pt_ID", &m_pt_ID);

  m_eta_CB = new std::vector<float>();
  vetotree->Branch("Eta_CB", &m_eta_CB);
  m_eta_ME = new std::vector<float>();
  vetotree->Branch("Eta_ME", &m_eta_ME);
  m_eta_MS = new std::vector<float>();
  vetotree->Branch("Eta_MS", &m_eta_MS);
  m_eta_ID = new std::vector<float>();
  vetotree->Branch("Eta_ID", &m_eta_ID);

  m_phi_CB = new std::vector<float>();
  vetotree->Branch("Phi_CB", &m_phi_CB);
  m_phi_ME = new std::vector<float>();
  vetotree->Branch("Phi_ME", &m_phi_ME);
  m_phi_MS = new std::vector<float>();
  vetotree->Branch("Phi_MS", &m_phi_MS);
  m_phi_ID = new std::vector<float>();
  vetotree->Branch("Phi_ID", &m_phi_ID);

  m_d0sig_CB = new std::vector<float>();
  vetotree->Branch("d0sig_CB", &m_d0sig_CB);
  m_d0sig_ME = new std::vector<float>();
  vetotree->Branch("d0sig_ME", &m_d0sig_ME);
  m_d0sig_MS = new std::vector<float>();
  vetotree->Branch("d0sig_MS", &m_d0sig_MS);
  m_d0sig_ID = new std::vector<float>();
  vetotree->Branch("d0sig_ID", &m_d0sig_ID);

  m_z0_CB = new std::vector<float>();
  vetotree->Branch("z0_CB", &m_z0_CB);
  m_z0_ME = new std::vector<float>();
  vetotree->Branch("z0_ME", &m_z0_ME);
  m_z0_MS = new std::vector<float>();
  vetotree->Branch("z0_MS", &m_z0_MS);
  m_z0_ID = new std::vector<float>();
  vetotree->Branch("z0_ID", &m_z0_ID);

  m_qOverP_CB = new std::vector<float>();
  vetotree->Branch("qOverP_CB", &m_qOverP_CB);
  m_qOverP_ME = new std::vector<float>();
  vetotree->Branch("qOverP_ME", &m_qOverP_ME);
  m_qOverP_MS = new std::vector<float>();
  vetotree->Branch("qOverP_MS", &m_qOverP_MS);
  m_qOverP_ID = new std::vector<float>();
  vetotree->Branch("qOverP_ID", &m_qOverP_ID);
  
  m_dqOverP_CB = new std::vector<float>();
  vetotree->Branch("dqOverP_CB", &m_dqOverP_CB);
  m_dqOverP_ME = new std::vector<float>();
  vetotree->Branch("dqOverP_ME", &m_dqOverP_ME);
  m_dqOverP_MS = new std::vector<float>();
  vetotree->Branch("dqOverP_MS", &m_dqOverP_MS);
  m_dqOverP_ID = new std::vector<float>();
  vetotree->Branch("dqOverP_ID", &m_dqOverP_ID);

  m_chi2_CB = new std::vector<float>();
  vetotree->Branch("chi2_CB", &m_chi2_CB);
  m_chi2_ME = new std::vector<float>();
  vetotree->Branch("chi2_ME", &m_chi2_ME);
  m_chi2_MS = new std::vector<float>();
  vetotree->Branch("chi2_MS", &m_chi2_MS);
  m_chi2_ID = new std::vector<float>();
  vetotree->Branch("chi2_ID", &m_chi2_ID);

  m_dof_CB = new std::vector<float>();
  vetotree->Branch("dof_CB", &m_dof_CB);
  m_dof_ME = new std::vector<float>();
  vetotree->Branch("dof_ME", &m_dof_ME);
  m_dof_MS = new std::vector<float>();
  vetotree->Branch("dof_MS", &m_dof_MS);
  m_dof_ID = new std::vector<float>();
  vetotree->Branch("dof_ID", &m_dof_ID);
  
  vetotree->Branch("m_CB", &m_m_CB);
  vetotree->Branch("m_ME", &m_m_ME);
  vetotree->Branch("m_MS", &m_m_MS);
  vetotree->Branch("m_ID", &m_m_ID);

  m_e_CB = new std::vector<float>();
  vetotree->Branch("E_CB", &m_e_CB);
  m_e_ME = new std::vector<float>();
  vetotree->Branch("E_ME", &m_e_ME);
  m_e_MS = new std::vector<float>();
  vetotree->Branch("E_MS", &m_e_MS);
  m_e_ID = new std::vector<float>();
  vetotree->Branch("E_ID", &m_e_ID);

  m_charge_CB = new std::vector<float>();
  vetotree->Branch("Charge_CB", &m_charge_CB);
  m_charge_ME = new std::vector<float>();
  vetotree->Branch("Charge_ME", &m_charge_ME);
  m_charge_MS = new std::vector<float>();
  vetotree->Branch("Charge_MS", &m_charge_MS);
  m_charge_ID = new std::vector<float>();
  vetotree->Branch("Charge_ID", &m_charge_ID);

  m_eLoss = new std::vector<float>();
  vetotree->Branch("EnergyLoss",&m_eLoss);
  m_muonType = new std::vector<int>();
  vetotree->Branch("MuonType", &m_muonType);
  m_quality = new std::vector<int>();
  vetotree->Branch("Quality", &m_quality);
  m_author = new std::vector<int>();
  vetotree->Branch("Author", &m_author);
  
  return StatusCode::SUCCESS;
}



StatusCode MuonSelection :: execute ()
{
  // event info
  const xAOD::EventInfo *ei = nullptr;
  ANA_CHECK (evtStore()->retrieve(ei, "EventInfo"));

  // check if event is MC or data
  bool isMC = false;
  if (ei->eventType(xAOD::EventInfo::IS_SIMULATION)) isMC = true;

  // check if data passes GRL
  if (!isMC) {
    if (!m_grl->passRunLB(*ei)) {
      ANA_MSG_INFO ("Skip event not on GRL");
      return StatusCode::SUCCESS;
    }
  }

  // get muon container
  const xAOD::MuonContainer *muons = 0;
  ANA_CHECK (evtStore()->retrieve (muons, "Muons"));

  // get vertex container
  const xAOD::VertexContainer *vtcs = 0;
  ANA_CHECK (evtStore()->retrieve (vtcs, "PrimaryVertices"));
 
  // find the primary vertex z coordinate
  float pvtx_z = -1e6;
  bool correctVertex = false;
  if (correctVertex) {
    for (const xAOD::Vertex *vtx : *vtcs) {
      if (vtx->vertexType() == xAOD::VxType::PriVtx)
        pvtx_z = vtx->z();
    }
  }

  // skip event if no primary vertex found 
  if ( pvtx_z == -1e6 && correctVertex) {
    ANA_MSG_DEBUG("No primary vertex found! Skipping event.");
    return StatusCode::SUCCESS;
  }

  //ANA_MSG_INFO("Primary vertex found.");

  //pvtx_z = 0;
  
  m_pt_CB->clear();
  m_pt_ME->clear();
  m_pt_MS->clear();
  m_pt_ID->clear();
  m_eta_CB->clear();
  m_eta_ME->clear();
  m_eta_MS->clear();
  m_eta_ID->clear();
  m_phi_CB->clear();
  m_phi_ME->clear();
  m_phi_MS->clear();
  m_phi_ID->clear();
  m_z0_CB->clear();
  m_z0_ME->clear();
  m_z0_MS->clear();
  m_z0_ID->clear();
  m_d0sig_CB->clear();
  m_d0sig_ME->clear();
  m_d0sig_MS->clear();
  m_d0sig_ID->clear();
  m_e_CB->clear();
  m_e_ME->clear();
  m_e_MS->clear();
  m_e_ID->clear();
  m_charge_CB->clear();
  m_charge_ME->clear();
  m_charge_MS->clear();
  m_charge_ID->clear();
  m_qOverP_CB->clear();
  m_qOverP_ME->clear();
  m_qOverP_MS->clear();
  m_qOverP_ID->clear();
  m_dqOverP_CB->clear();
  m_dqOverP_ME->clear();
  m_dqOverP_MS->clear();
  m_dqOverP_ID->clear();
  m_chi2_CB->clear();
  m_chi2_ME->clear();
  m_chi2_MS->clear();
  m_chi2_ID->clear();
  m_dof_CB->clear();
  m_dof_ME->clear();
  m_dof_MS->clear();
  m_dof_ID->clear();
  m_m_CB = 0.;
  m_m_ME = 0.;
  m_m_MS = 0.;
  m_m_ID = 0.;
  m_eLoss->clear();
  m_muonType->clear();
  m_quality->clear();
  m_author->clear();


  // select muons
  muon_1 = nullptr;
  muon_2 = nullptr;
  for (const xAOD::Muon* muon : *muons) {
    if (muon->charge() > 0) {
      if (!muon_1) 
        muon_1 = muon;
      else if (muon->pt() > muon_1->pt()) 
        muon_1 = muon;
    }
    if (muon->charge() < 0) {
      if (!muon_2)
        muon_2 = muon;
      else if (muon->pt() > muon_2->pt())
        muon_2 = muon; 
    }
  }

  // check that we found opposite sign muons
  if (!muon_1 || !muon_2) {
    ANA_MSG_DEBUG ("No muon pair selected");
      return StatusCode::SUCCESS;
  }

  std::vector<const xAOD::Muon*> *my_muons = new std::vector<const xAOD::Muon*>();
  if (muon_1->pt() > muon_2->pt()) {
    my_muons->push_back(muon_1);
    my_muons->push_back(muon_2);
  }
  else {
    my_muons->push_back(muon_2);
    my_muons->push_back(muon_1);
  }

  for (const xAOD::Muon* muon : *my_muons) {
    // access muon object attributes not specific to tracks
    m_muonType->push_back(muon->muonType());
    m_author->push_back(muon->author());
    m_quality->push_back(m_muonSelection->getQuality(*muon));
  
    float eLoss = 0.;
    if (!muon->parameter(eLoss, xAOD::Muon::EnergyLoss) )
      ANA_MSG_DEBUG ("Muon energy loss not available");
    m_eLoss->push_back(eLoss * 0.001); // GeV

    // check if there's a CB track, then access attributes
    if ((muon->combinedTrackParticleLink())) { 
      m_pt_CB->push_back((*muon->combinedTrackParticleLink())->pt() * 0.001); // GeV
      m_eta_CB->push_back((*muon->combinedTrackParticleLink())->eta());
      m_phi_CB->push_back((*muon->combinedTrackParticleLink())->phi());
      m_e_CB->push_back((*muon->combinedTrackParticleLink())->e() * 0.001); // GeV
      m_charge_CB->push_back((*muon->combinedTrackParticleLink())->charge());
      m_d0sig_CB->push_back(xAOD::TrackingHelpers::d0significance((*muon->combinedTrackParticleLink()),
                            ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
      if (correctVertex) 
        m_z0_CB->push_back((*muon->combinedTrackParticleLink())->z0() + (*muon->combinedTrackParticleLink())->vz() - pvtx_z);
      else 
        m_z0_CB->push_back((*muon->combinedTrackParticleLink())->z0());
      
      m_qOverP_CB->push_back((*muon->combinedTrackParticleLink())->qOverP() / 0.001); // GeV^-1
      m_dqOverP_CB->push_back(sqrt((*muon->combinedTrackParticleLink())->definingParametersCovMatrix()(4,4)) / 0.001); // GeV^-1
      m_chi2_CB->push_back((*muon->combinedTrackParticleLink())->chiSquared());
      m_dof_CB->push_back((*muon->combinedTrackParticleLink())->numberDoF());
    }
    else {
      m_pt_CB->push_back(-1);
      m_eta_CB->push_back(-1e6);
      m_phi_CB->push_back(-1e6);
      m_e_CB->push_back(-1);
      m_charge_CB->push_back(0);
      m_d0sig_CB->push_back(-1);
      m_z0_CB->push_back(-1);
      m_qOverP_CB->push_back(-1);
      m_dqOverP_CB->push_back(-1);
      m_chi2_CB->push_back(-1);
      m_dof_CB->push_back(-1);
      m_m_CB = -1;
    } 

    // check if there's an ME track, then access attributes
    if ((muon->extrapolatedMuonSpectrometerTrackParticleLink())) {
      m_pt_ME->push_back((*muon->extrapolatedMuonSpectrometerTrackParticleLink())->pt() * 0.001); // GeV
      m_eta_ME->push_back((*muon->extrapolatedMuonSpectrometerTrackParticleLink())->eta());
      m_phi_ME->push_back((*muon->extrapolatedMuonSpectrometerTrackParticleLink())->phi());
      m_e_ME->push_back((*muon->extrapolatedMuonSpectrometerTrackParticleLink())->e() * 0.001); // GeV
      m_charge_ME->push_back((*muon->extrapolatedMuonSpectrometerTrackParticleLink())->charge());
      m_d0sig_ME->push_back(xAOD::TrackingHelpers::d0significance((*muon->extrapolatedMuonSpectrometerTrackParticleLink()),
                            ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
      if (correctVertex) 
        m_z0_ME->push_back((*muon->extrapolatedMuonSpectrometerTrackParticleLink())->z0() + (*muon->extrapolatedMuonSpectrometerTrackParticleLink())->vz() - pvtx_z);
      else
        m_z0_ME->push_back((*muon->extrapolatedMuonSpectrometerTrackParticleLink())->z0());
      
      m_qOverP_ME->push_back((*muon->extrapolatedMuonSpectrometerTrackParticleLink())->qOverP() / 0.001); // GeV^-1
      m_dqOverP_ME->push_back(sqrt((*muon->extrapolatedMuonSpectrometerTrackParticleLink())->definingParametersCovMatrix()(4,4)) / 0.001); // GeV^-1
      m_chi2_ME->push_back((*muon->extrapolatedMuonSpectrometerTrackParticleLink())->chiSquared());
      m_dof_ME->push_back((*muon->extrapolatedMuonSpectrometerTrackParticleLink())->numberDoF());
    }
    else {
      m_pt_ME->push_back(-1);
      m_eta_ME->push_back(-1e6);
      m_phi_ME->push_back(-1e6);
      m_e_ME->push_back(-1); 
      m_charge_ME->push_back(0);
      m_d0sig_ME->push_back(-1);
      m_z0_ME->push_back(-1);
      m_qOverP_ME->push_back(-1);
      m_dqOverP_ME->push_back(-1);
      m_chi2_ME->push_back(-1);
      m_dof_ME->push_back(-1);
      m_m_ME = -1;
    }

    // check if there's an MS track, then access attributes
    if ((muon->muonSpectrometerTrackParticleLink())) {
      m_pt_MS->push_back((*muon->muonSpectrometerTrackParticleLink())->pt() * 0.001); // GeV
      m_eta_MS->push_back((*muon->muonSpectrometerTrackParticleLink())->eta());
      m_phi_MS->push_back((*muon->muonSpectrometerTrackParticleLink())->phi());
      m_e_MS->push_back((*muon->muonSpectrometerTrackParticleLink())->e() * 0.001); // GeV
      m_charge_MS->push_back((*muon->muonSpectrometerTrackParticleLink())->charge());
      m_d0sig_MS->push_back(xAOD::TrackingHelpers::d0significance((*muon->muonSpectrometerTrackParticleLink()),
                            ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
      if (correctVertex)
        m_z0_MS->push_back((*muon->muonSpectrometerTrackParticleLink())->z0() + (*muon->muonSpectrometerTrackParticleLink())->vz() - pvtx_z);
      else 
        m_z0_MS->push_back((*muon->muonSpectrometerTrackParticleLink())->z0());
      
      m_qOverP_MS->push_back((*muon->muonSpectrometerTrackParticleLink())->qOverP() / 0.001); // GeV^-1
      m_dqOverP_MS->push_back(sqrt((*muon->muonSpectrometerTrackParticleLink())->definingParametersCovMatrix()(4,4)) / 0.001); // GeV^-1
      m_chi2_MS->push_back((*muon->muonSpectrometerTrackParticleLink())->chiSquared());
      m_dof_MS->push_back((*muon->muonSpectrometerTrackParticleLink())->numberDoF());
    }
    else {
      m_pt_MS->push_back(-1);
      m_eta_MS->push_back(-1e6);
      m_phi_MS->push_back(-1e6);
      m_e_MS->push_back(-1);
      m_charge_MS->push_back(0);
      m_d0sig_MS->push_back(-1);
      m_z0_MS->push_back(-1);
      m_qOverP_MS->push_back(-1);
      m_dqOverP_MS->push_back(-1);
      m_chi2_MS->push_back(-1);
      m_dof_MS->push_back(-1);
      m_m_MS = -1;
    }

    // check if there's an ID track, then access attributes
    if ((muon->inDetTrackParticleLink())) {
      m_pt_ID->push_back((*muon->inDetTrackParticleLink())->pt() * 0.001); // GeV
      m_eta_ID->push_back((*muon->inDetTrackParticleLink())->eta());
      m_phi_ID->push_back((*muon->inDetTrackParticleLink())->phi());
      m_e_ID->push_back((*muon->inDetTrackParticleLink())->e() * 0.001); // GeV
      m_charge_ID->push_back((*muon->inDetTrackParticleLink())->charge());
      m_d0sig_ID->push_back(xAOD::TrackingHelpers::d0significance((*muon->inDetTrackParticleLink()),
                            ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
      if (correctVertex) 
        m_z0_ID->push_back((*muon->inDetTrackParticleLink())->z0() + (*muon->inDetTrackParticleLink())->vz() - pvtx_z);
      else 
        m_z0_ID->push_back((*muon->inDetTrackParticleLink())->z0());
      
      m_qOverP_ID->push_back((*muon->inDetTrackParticleLink())->qOverP() / 0.001); // GeV^-1
      m_dqOverP_ID->push_back(sqrt((*muon->inDetTrackParticleLink())->definingParametersCovMatrix()(4,4)) / 0.001); // GeV^-1
      m_chi2_ID->push_back((*muon->inDetTrackParticleLink())->chiSquared());
      m_dof_ID->push_back((*muon->inDetTrackParticleLink())->numberDoF());
    }
    else {
      m_pt_ID->push_back(-1); 
      m_eta_ID->push_back(-1e6);
      m_phi_ID->push_back(-1e6);
      m_e_ID->push_back(-1); 
      m_charge_ID->push_back(0);
      m_d0sig_ID->push_back(-1);
      m_z0_ID->push_back(-1);
      m_qOverP_ID->push_back(-1);
      m_dqOverP_ID->push_back(-1);
      m_chi2_ID->push_back(-1);
      m_dof_ID->push_back(-1);
      m_m_ID = -1;
    } 
  }

  // Check that both selected muons had the particle track and compute invariant mass
  if (m_m_CB != -1) {
    m_m_CB = sqrt(2 * m_pt_CB->at(0) * m_pt_CB->at(1)
                    * (cosh(m_eta_CB->at(0)-m_eta_CB->at(1)) - cos(m_phi_CB->at(0)-m_phi_CB->at(1))));
  }
  if (m_m_ME != -1) {
    m_m_ME = sqrt(2 * m_pt_ME->at(0) * m_pt_ME->at(1)
                    * (cosh(m_eta_ME->at(0)-m_eta_ME->at(1)) - cos(m_phi_ME->at(0)-m_phi_ME->at(1))));
  }
  if (m_m_MS != -1) {
    m_m_MS = sqrt(2 * m_pt_MS->at(0) * m_pt_MS->at(1)
                    * (cosh(m_eta_MS->at(0)-m_eta_MS->at(1)) - cos(m_phi_MS->at(0)-m_phi_MS->at(1))));
  }
  if (m_m_ID != -1) {
    m_m_ID = sqrt(2 * m_pt_ID->at(0) * m_pt_ID->at(1)
                    * (cosh(m_eta_ID->at(0)-m_eta_ID->at(1)) - cos(m_phi_ID->at(0)-m_phi_ID->at(1))));
  }
 
  delete my_muons; 
  // if both muons pass the selection
  if (m_muonSelection->accept(*muon_1) && m_muonSelection->accept(*muon_2))
    tree ("SelectedZDecayMuons")->Fill();

  // fill background tree if any invariant mass is < 70 GeV
  // may change the requirement
  if ( (0 < m_m_CB && m_m_CB < 70) || (0 < m_m_ME && m_m_ME < 70) || (0 < m_m_ID && m_m_ID < 70) )
    tree ("BackgroundMuons")->Fill();

  return StatusCode::SUCCESS;
}



StatusCode MuonSelection :: finalize ()
{
  return StatusCode::SUCCESS;
}
