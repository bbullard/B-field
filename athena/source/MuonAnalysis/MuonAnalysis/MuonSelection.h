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

  // Selected Muons
  mutable const xAOD::Muon *muon_1;
  mutable const xAOD::Muon *muon_2;
  
  // Tree branch addresses
  std::vector<float> *m_pt_CB = nullptr;
  std::vector<float> *m_pt_ME = nullptr;
  std::vector<float> *m_pt_MS = nullptr;
  std::vector<float> *m_pt_ID = nullptr;
  std::vector<float> *m_eta_CB = nullptr;
  std::vector<float> *m_eta_ME = nullptr;
  std::vector<float> *m_eta_MS = nullptr;
  std::vector<float> *m_eta_ID = nullptr;
  std::vector<float> *m_phi_CB = nullptr;
  std::vector<float> *m_phi_ME = nullptr;
  std::vector<float> *m_phi_MS = nullptr;
  std::vector<float> *m_phi_ID = nullptr;
  std::vector<float> *m_z0_CB = nullptr;
  std::vector<float> *m_z0_ME = nullptr;
  std::vector<float> *m_z0_MS = nullptr;
  std::vector<float> *m_z0_ID = nullptr;
  std::vector<float> *m_d0sig_CB = nullptr;
  std::vector<float> *m_d0sig_ME = nullptr;
  std::vector<float> *m_d0sig_MS = nullptr;
  std::vector<float> *m_d0sig_ID = nullptr;
  float m_m_CB = 0.;
  float m_m_ME = 0.;
  float m_m_MS = 0.;
  float m_m_ID = 0.;
  std::vector<float> *m_e_CB = nullptr;
  std::vector<float> *m_e_ME = nullptr;
  std::vector<float> *m_e_MS = nullptr;
  std::vector<float> *m_e_ID = nullptr;
  std::vector<float> *m_charge_CB = nullptr;
  std::vector<float> *m_charge_ME = nullptr;
  std::vector<float> *m_charge_MS = nullptr;
  std::vector<float> *m_charge_ID = nullptr;
  std::vector<float> *m_qOverP_CB = nullptr;
  std::vector<float> *m_qOverP_ME = nullptr;
  std::vector<float> *m_qOverP_MS = nullptr;
  std::vector<float> *m_qOverP_ID = nullptr;
  std::vector<float> *m_dqOverP_CB = nullptr;
  std::vector<float> *m_dqOverP_ME = nullptr;
  std::vector<float> *m_dqOverP_MS = nullptr;
  std::vector<float> *m_dqOverP_ID = nullptr;
  std::vector<float> *m_chi2_CB = nullptr;
  std::vector<float> *m_chi2_ME = nullptr;
  std::vector<float> *m_chi2_MS = nullptr;
  std::vector<float> *m_chi2_ID = nullptr;
  std::vector<float> *m_dof_CB = nullptr;
  std::vector<float> *m_dof_ME = nullptr;
  std::vector<float> *m_dof_MS = nullptr;
  std::vector<float> *m_dof_ID = nullptr;
  std::vector<float> *m_eLoss = nullptr;
  std::vector<int> *m_muonType = nullptr;
  std::vector<int> *m_quality = nullptr;
  std::vector<int> *m_author = nullptr; 
};

#endif
