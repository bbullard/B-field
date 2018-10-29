#ifndef MuonAnalysis_MuonSelection_H
#define MuonAnalysis_MuonSelection_H

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <MuonAnalysisInterfaces/IMuonSelectionTool.h>
#include <AsgAnalysisInterfaces/IGoodRunsListSelectionTool.h>
#include <AsgTools/AnaToolHandle.h>
#include <TTree.h>
#include <TH1.h>
#include <vector>

class MuonSelection : public EL::AnaAlgorithm
{
public:
  // this is a standard algorithm constructor
  MuonSelection (const std::string& name, ISvcLocator* pSvcLocator);
  ~MuonSelection();

  // these are the functions inherited from Algorithm
  virtual StatusCode initialize () override;
  virtual StatusCode execute () override;
  virtual StatusCode finalize () override;

private:
  // Good Runs List tool
  asg::AnaToolHandle<IGoodRunsListSelectionTool> m_grl; //!
  // MuonSelectionTool
  asg::AnaToolHandle<CP::IMuonSelectionTool> m_muonSelection; //!

  mutable const xAOD::TrackParticle_v1* CB;
  mutable const xAOD::TrackParticle_v1* ME;
  mutable const xAOD::TrackParticle_v1* MSO;
  mutable const xAOD::TrackParticle_v1* MSOE;
  mutable const xAOD::TrackParticle_v1* ID;
  
  // Vector of positive and negative muons
  std::vector<const xAOD::Muon*>* positiveMuons;
  std::vector<const xAOD::Muon*>* negativeMuons;
  
  // Tree branch addresses
  // Event level
  bool passGRL = true;
  unsigned long long eventNumber = 0;
  unsigned int nPositiveMuons = 0;
  unsigned int nNegativeMuons = 0;

  // Positive Muon Variables
  std::vector<bool> *p_passIDcuts = nullptr;
  std::vector<bool> *p_passAll = nullptr;
  std::vector<float> *p_eLoss = nullptr;
  std::vector<int> *p_muonType = nullptr;
  std::vector<int> *p_quality = nullptr;
  std::vector<int> *p_primaryAuthor = nullptr;
  std::vector<int> *p_authors = nullptr;
  std::vector<int> *p_nPrecisionLayers = nullptr; 
  std::vector<int> *p_nPrecisionHoleLayers = nullptr; 
  std::vector<int> *p_nPhiLayers = nullptr; 
  std::vector<int> *p_nPhiHoleLayers = nullptr; 
  std::vector<int> *p_nTriggerLayers = nullptr; 
  std::vector<int> *p_nTriggerHoleLayers = nullptr; 
  std::vector<int> *p_innerSmallHits = nullptr; 
  std::vector<int> *p_innerLargeHits = nullptr; 
  std::vector<int> *p_middleSmallHits = nullptr; 
  std::vector<int> *p_middleLargeHits = nullptr; 
  std::vector<int> *p_outerSmallHits = nullptr; 
  std::vector<int> *p_outerLargeHits = nullptr; 
  std::vector<int> *p_extendedSmallHits = nullptr; 
  std::vector<int> *p_extendedLargeHits = nullptr; 
  std::vector<int> *p_innerSmallHoles = nullptr; 
  std::vector<int> *p_innerLargeHoles = nullptr; 
  std::vector<int> *p_middleSmallHoles = nullptr; 
  std::vector<int> *p_middleLargeHoles = nullptr; 
  std::vector<int> *p_outerSmallHoles = nullptr; 
  std::vector<int> *p_outerLargeHoles = nullptr; 
  std::vector<int> *p_extendedSmallHoles = nullptr; 
  std::vector<int> *p_extendedLargeHoles = nullptr; 
 
  // Negative Muon Variables
  std::vector<bool> *n_passIDcuts = nullptr;
  std::vector<bool> *n_passAll = nullptr;
  std::vector<float> *n_eLoss = nullptr;
  std::vector<int> *n_muonType = nullptr;
  std::vector<int> *n_quality = nullptr;
  std::vector<int> *n_primaryAuthor = nullptr;
  std::vector<int> *n_authors = nullptr;
  std::vector<int> *n_nPrecisionLayers = nullptr; 
  std::vector<int> *n_nPrecisionHoleLayers = nullptr; 
  std::vector<int> *n_nPhiLayers = nullptr; 
  std::vector<int> *n_nPhiHoleLayers = nullptr; 
  std::vector<int> *n_nTriggerLayers = nullptr; 
  std::vector<int> *n_nTriggerHoleLayers = nullptr; 
  std::vector<int> *n_innerSmallHits = nullptr; 
  std::vector<int> *n_innerLargeHits = nullptr; 
  std::vector<int> *n_middleSmallHits = nullptr; 
  std::vector<int> *n_middleLargeHits = nullptr; 
  std::vector<int> *n_outerSmallHits = nullptr; 
  std::vector<int> *n_outerLargeHits = nullptr; 
  std::vector<int> *n_extendedSmallHits = nullptr; 
  std::vector<int> *n_extendedLargeHits = nullptr; 
  std::vector<int> *n_innerSmallHoles = nullptr; 
  std::vector<int> *n_innerLargeHoles = nullptr; 
  std::vector<int> *n_middleSmallHoles = nullptr; 
  std::vector<int> *n_middleLargeHoles = nullptr; 
  std::vector<int> *n_outerSmallHoles = nullptr; 
  std::vector<int> *n_outerLargeHoles = nullptr; 
  std::vector<int> *n_extendedSmallHoles = nullptr; 
  std::vector<int> *n_extendedLargeHoles = nullptr; 
  
  // Positive Muon Track variables
  std::vector<bool> *p_isCB = nullptr;
  std::vector<bool> *p_isME = nullptr;
  std::vector<bool> *p_isMSO = nullptr;
  std::vector<bool> *p_isMSOE = nullptr;
  std::vector<bool> *p_isID = nullptr;
  std::vector<float> *p_eta_CB = nullptr;
  std::vector<float> *p_eta_ME = nullptr;
  std::vector<float> *p_eta_MSO = nullptr;
  std::vector<float> *p_eta_MSOE = nullptr;
  std::vector<float> *p_eta_ID = nullptr;
  std::vector<float> *p_phi_CB = nullptr;
  std::vector<float> *p_phi_ME = nullptr;
  std::vector<float> *p_phi_MSO = nullptr;
  std::vector<float> *p_phi_MSOE = nullptr;
  std::vector<float> *p_phi_ID = nullptr;
  std::vector<float> *p_z0_CB = nullptr;
  std::vector<float> *p_z0_ME = nullptr;
  std::vector<float> *p_z0_MSO = nullptr;
  std::vector<float> *p_z0_MSOE = nullptr;
  std::vector<float> *p_z0_ID = nullptr;
  std::vector<float> *p_d0sig_CB = nullptr;
  std::vector<float> *p_d0sig_ME = nullptr;
  std::vector<float> *p_d0sig_MSO = nullptr;
  std::vector<float> *p_d0sig_MSOE = nullptr;
  std::vector<float> *p_d0sig_ID = nullptr;
  std::vector<float> *p_qOverP_CB = nullptr;
  std::vector<float> *p_qOverP_ME = nullptr;
  std::vector<float> *p_qOverP_MSO = nullptr;
  std::vector<float> *p_qOverP_MSOE = nullptr;
  std::vector<float> *p_qOverP_ID = nullptr;
  std::vector<float> *p_dqOverP_CB = nullptr;
  std::vector<float> *p_dqOverP_ME = nullptr;
  std::vector<float> *p_dqOverP_MSO = nullptr;
  std::vector<float> *p_dqOverP_MSOE = nullptr;
  std::vector<float> *p_dqOverP_ID = nullptr;
  std::vector<float> *p_rchi2_CB = nullptr;
  std::vector<float> *p_rchi2_ME = nullptr;
  std::vector<float> *p_rchi2_MSO = nullptr;
  std::vector<float> *p_rchi2_MSOE = nullptr;
  std::vector<float> *p_rchi2_ID = nullptr;
  
  // Negative Muon Track variables
  std::vector<bool> *n_isCB = nullptr;
  std::vector<bool> *n_isME = nullptr;
  std::vector<bool> *n_isMSO = nullptr;
  std::vector<bool> *n_isMSOE = nullptr;
  std::vector<bool> *n_isID = nullptr;
  std::vector<float> *n_eta_CB = nullptr;
  std::vector<float> *n_eta_ME = nullptr;
  std::vector<float> *n_eta_MSO = nullptr;
  std::vector<float> *n_eta_MSOE = nullptr;
  std::vector<float> *n_eta_ID = nullptr;
  std::vector<float> *n_phi_CB = nullptr;
  std::vector<float> *n_phi_ME = nullptr;
  std::vector<float> *n_phi_MSO = nullptr;
  std::vector<float> *n_phi_MSOE = nullptr;
  std::vector<float> *n_phi_ID = nullptr;
  std::vector<float> *n_z0_CB = nullptr;
  std::vector<float> *n_z0_ME = nullptr;
  std::vector<float> *n_z0_MSO = nullptr;
  std::vector<float> *n_z0_MSOE = nullptr;
  std::vector<float> *n_z0_ID = nullptr;
  std::vector<float> *n_d0sig_CB = nullptr;
  std::vector<float> *n_d0sig_ME = nullptr;
  std::vector<float> *n_d0sig_MSO = nullptr;
  std::vector<float> *n_d0sig_MSOE = nullptr;
  std::vector<float> *n_d0sig_ID = nullptr;
  std::vector<float> *n_qOverP_CB = nullptr;
  std::vector<float> *n_qOverP_ME = nullptr;
  std::vector<float> *n_qOverP_MSO = nullptr;
  std::vector<float> *n_qOverP_MSOE = nullptr;
  std::vector<float> *n_qOverP_ID = nullptr;
  std::vector<float> *n_dqOverP_CB = nullptr;
  std::vector<float> *n_dqOverP_ME = nullptr;
  std::vector<float> *n_dqOverP_MSO = nullptr;
  std::vector<float> *n_dqOverP_MSOE = nullptr;
  std::vector<float> *n_dqOverP_ID = nullptr;
  std::vector<float> *n_rchi2_CB = nullptr;
  std::vector<float> *n_rchi2_ME = nullptr;
  std::vector<float> *n_rchi2_MSO = nullptr;
  std::vector<float> *n_rchi2_MSOE = nullptr;
  std::vector<float> *n_rchi2_ID = nullptr;
};

#endif
