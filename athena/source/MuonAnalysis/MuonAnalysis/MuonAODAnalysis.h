#ifndef MuonAnalysis_MuonAODAnalysis_H
#define MuonAnalysis_MuonAODAnalysis_H

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <MuonAnalysisInterfaces/IMuonSelectionTool.h>
#include <AsgAnalysisInterfaces/IGoodRunsListSelectionTool.h>
#include <AsgTools/AnaToolHandle.h>
#include <TTree.h>
#include <vector>

class MuonAODAnalysis : public EL::AnaAlgorithm
{
public:
  // this is a standard algorithm constructor
  MuonAODAnalysis (const std::string& name, ISvcLocator* pSvcLocator);
  ~MuonAODAnalysis();

  // these are the functions inherited from Algorithm
  virtual StatusCode initialize () override;
  virtual StatusCode execute () override;
  virtual StatusCode finalize () override;

private:
  // Good Runs List tool
  asg::AnaToolHandle<IGoodRunsListSelectionTool> m_grl; //!
  // MuonSelectionTool
  asg::AnaToolHandle<CP::IMuonSelectionTool> m_muonSelection; //!

  // Tree branch addresses
  std::vector<float> *m_pt = nullptr;
  std::vector<float> *m_eta = nullptr;
  std::vector<float> *m_phi = nullptr;
  std::vector<float> *m_e = nullptr;
  std::vector<float> *m_charge = nullptr;
  std::vector<float> *m_eLoss = nullptr;
  std::vector<float> *m_z0 = nullptr;
  std::vector<float> *m_d0sig = nullptr;
  std::vector<int> *m_muonType = nullptr;
  std::vector<int> *m_quality = nullptr;
  std::vector<int> *m_author = nullptr;
  std::vector<bool> *m_passesIDCuts = nullptr;
  std::vector<bool> *m_acceptedMuon = nullptr;
};

#endif
