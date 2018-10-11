#include <AsgTools/MessageCheck.h>
#include <MuonAnalysis/MuonAODAnalysis.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODMuon/MuonContainer.h>
#include <xAODTracking/TrackParticlexAODHelpers.h>
#include <xAODTracking/VertexContainer.h>
#include <TSystem.h>

MuonAODAnalysis :: MuonAODAnalysis (const std::string& name,
                                  ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm (name, pSvcLocator),
      m_muonSelection ("CP::MuonSelectionTool", this),
      m_grl ("GoodRunsListSelectionTool/grl", this)
{
}

MuonAODAnalysis :: ~MuonAODAnalysis () {
  if (m_pt) delete m_pt;
  if (m_eta) delete m_eta;
  if (m_phi) delete m_phi;
  if (m_e) delete m_e;
  if (m_charge) delete m_charge;
  if (m_eLoss) delete m_eLoss;
  if (m_d0sig) delete m_d0sig;
  if (m_z0) delete m_z0;
  if (m_muonType) delete m_muonType;
  if (m_quality) delete m_quality;
  if (m_author) delete m_author;
  if (m_passesIDCuts) delete m_passesIDCuts;
  if (m_acceptedMuon) delete m_acceptedMuon;
}

StatusCode MuonAODAnalysis :: initialize ()
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
  ANA_CHECK (m_muonSelection.setProperty("MaxEta", 2.5));
  ANA_CHECK (m_muonSelection.setProperty("MuQuality", 1));
  ANA_CHECK (m_muonSelection.initialize());

  // book tree
  ANA_CHECK(book (TTree ("muonAnalysis", "Muon Analysis ntuple")));
  TTree* mutree = tree ("muonAnalysis");

  m_pt = new std::vector<float>();
  mutree->Branch("Pt", &m_pt);
  m_eta = new std::vector<float>();
  mutree->Branch("Eta", &m_eta);
  m_phi = new std::vector<float>();
  mutree->Branch("Phi", &m_phi);
  m_e = new std::vector<float>();
  mutree->Branch("E", &m_e);
  m_charge = new std::vector<float>();
  mutree->Branch("Charge", &m_charge);
  m_eLoss = new std::vector<float>();
  mutree->Branch("EnergyLoss",&m_eLoss);
  m_d0sig = new std::vector<float>();
  mutree->Branch("d0sig", &m_d0sig);
  m_z0 = new std::vector<float>();
  mutree->Branch("z0", &m_z0);
  m_muonType = new std::vector<int>();
  mutree->Branch("MuonType", &m_muonType);
  m_quality = new std::vector<int>();
  mutree->Branch("Quality", &m_quality);
  m_author = new std::vector<int>();
  mutree->Branch("Author", &m_author);
  m_passesIDCuts = new std::vector<bool>();
  mutree->Branch("PassesIDCuts", &m_passesIDCuts);
  m_acceptedMuon = new std::vector<bool>();
  mutree->Branch("AcceptedMuon", &m_acceptedMuon);
  return StatusCode::SUCCESS;
}



StatusCode MuonAODAnalysis :: execute ()
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
  float pvtx_z = 0.;
  for (const xAOD::Vertex *vtx : *vtcs) {
    if (vtx->vertexType() == xAOD::VxType::PriVtx)
      pvtx_z = vtx->z();
  }

  m_pt->clear();
  m_eta->clear();
  m_phi->clear();
  m_e->clear();
  m_charge->clear();
  m_eLoss->clear();
  m_z0->clear();
  m_d0sig->clear();
  m_muonType->clear();
  m_quality->clear();
  m_author->clear();
  m_passesIDCuts->clear();
  m_acceptedMuon->clear();
 
  
  // loop over the muons in the container
  for (const xAOD::Muon *muon : *muons) {
    // Simple things
    m_pt->push_back(muon->pt() * 0.001); // GeV
    m_eta->push_back(muon->eta());
    m_phi->push_back(muon->phi());
    m_e->push_back(muon->e() * 0.001); // GeV
    m_charge->push_back(muon->charge());
    m_muonType->push_back(muon->muonType());
    m_author->push_back(muon->author());
    if (m_muonSelection->accept(*muon)) m_acceptedMuon->push_back(true);
    else m_acceptedMuon->push_back(false);
    m_quality->push_back(m_muonSelection->getQuality(*muon));
    if (m_muonSelection->passedIDCuts(*muon)) m_passesIDCuts->push_back(true);
    else m_passesIDCuts->push_back(false);
    
    // Access energy loss if it is available
    float eLoss = 0.;
    if (!muon->parameter(eLoss, xAOD::Muon::EnergyLoss) )
      ANA_MSG_INFO ("Muon energy loss not available");
    m_eLoss->push_back(eLoss * 0.001); // GeV
    
    // Compute d0sig using tracking helper function
    const xAOD::TrackParticle *ptp = muon->primaryTrackParticle();
    m_d0sig->push_back(xAOD::TrackingHelpers::d0significance(ptp,
              ei->beamPosSigmaX(), ei->beamPosSigmaY(), ei->beamPosSigmaXY()));
  
    // Compute z0 wrt primary vertex
    m_z0->push_back(ptp->z0() + ptp->vz() - pvtx_z);
  }

  // fill the muon attributes into the tree
  tree ("muonAnalysis")->Fill();

  return StatusCode::SUCCESS;
}



StatusCode MuonAODAnalysis :: finalize ()
{
  return StatusCode::SUCCESS;
}
