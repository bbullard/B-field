#include <TTree.h>
#include <TChain.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TFile.h>
#include <string>

using namespace std;

void SkimNtuples(string ntuplesFile, string outputRootFile, bool isAOD=false) {
	// create chain for the input files
  TChain chainedNtuples("RecoMuons");
  std::ifstream ntupleFileStream(ntuplesFile.c_str());
  string fileName;
  while (ntupleFileStream >> fileName) {
    cout << "Adding file to chain: " << fileName << endl;
    chainedNtuples.Add(fileName.c_str());
  }

	// make readers for chain
  TTreeReader reader(&chainedNtuples);
  // event level  
  TTreeReaderValue<bool> passGRL_o(reader, "passGRL");
  TTreeReaderValue<unsigned long long> eventNumber_o(reader, "eventNumber");
  TTreeReaderValue<unsigned int> nPositiveMuons_o(reader, "nPositiveMuons");
  TTreeReaderValue<unsigned int> nNegativeMuons_o(reader, "nNegativeMuons");
  
  // positive muon variables
	TTreeReaderValue<vector<int>> p_pdgID_truth_o(reader, "p_pdgID_truth");
	TTreeReaderValue<vector<double>> p_pt_truth_o(reader, "p_pt_truth");
	TTreeReaderValue<vector<double>> p_eta_truth_o(reader, "p_eta_truth");
	TTreeReaderValue<vector<double>> p_phi_truth_o(reader, "p_phi_truth");
	TTreeReaderValue<vector<double>> p_m_truth_o(reader, "p_m_truth");
  TTreeReaderValue<vector<bool>> p_passIDcuts_o(reader, "p_passIDcuts");
  TTreeReaderValue<vector<bool>> p_passAll_o(reader, "p_passAll");
  TTreeReaderValue<vector<float>> p_eLoss_o(reader, "p_eLoss");
  TTreeReaderValue<vector<float>> p_ptcone40_o(reader, "p_ptcone40");
  TTreeReaderValue<vector<int>> p_muonType_o(reader, "p_muonType");
  TTreeReaderValue<vector<int>> p_quality_o(reader, "p_quality");
  TTreeReaderValue<vector<int>> p_primaryAuthor_o(reader, "p_primaryAuthor");
  TTreeReaderValue<vector<int>> p_authors_o(reader, "p_authors");
  TTreeReaderValue<vector<int>> p_nPrecisionLayers_o(reader, "p_nPrecisionLayers");
  TTreeReaderValue<vector<int>> p_nPrecisionHoleLayers_o(reader, "p_nPrecisionHoleLayers");
  //TTreeReaderValue<vector<int>> p_innerSmallHits_o(reader, "p_innerSmallHits");
  //TTreeReaderValue<vector<int>> p_innerLargeHits_o(reader, "p_innerLargeHits");
  //TTreeReaderValue<vector<int>> p_middleSmallHits_o(reader, "p_middleSmallHits");
  //TTreeReaderValue<vector<int>> p_middleLargeHits_o(reader, "p_middleLargeHits");
  //TTreeReaderValue<vector<int>> p_outerSmallHits_o(reader, "p_outerSmallHits");
  //TTreeReaderValue<vector<int>> p_outerLargeHits_o(reader, "p_outerLargeHits");
  //TTreeReaderValue<vector<int>> p_extendedSmallHits_o(reader, "p_extendedSmallHits");
  //TTreeReaderValue<vector<int>> p_extendedLargeHits_o(reader, "p_extendedLargeHits"); 
  //TTreeReaderValue<vector<int>> p_innerSmallHoles_o(reader, "p_innerSmallHoles");
  //TTreeReaderValue<vector<int>> p_innerLargeHoles_o(reader, "p_innerLargeHoles");
  //TTreeReaderValue<vector<int>> p_middleSmallHoles_o(reader, "p_middleSmallHoles");
  //TTreeReaderValue<vector<int>> p_middleLargeHoles_o(reader, "p_middleLargeHoles");
  //TTreeReaderValue<vector<int>> p_outerSmallHoles_o(reader, "p_outerSmallHoles");
  //TTreeReaderValue<vector<int>> p_outerLargeHoles_o(reader, "p_outerLargeHoles");
  //TTreeReaderValue<vector<int>> p_extendedSmallHoles_o(reader, "p_extendedSmallHoles");
  //TTreeReaderValue<vector<int>> p_extendedLargeHoles_o(reader, "p_extendedLargeHoles");

  // negative muon variables
	TTreeReaderValue<vector<int>> n_pdgID_truth_o(reader, "n_pdgID_truth");
	TTreeReaderValue<vector<double>> n_pt_truth_o(reader, "n_pt_truth");
	TTreeReaderValue<vector<double>> n_eta_truth_o(reader, "n_eta_truth");
	TTreeReaderValue<vector<double>> n_phi_truth_o(reader, "n_phi_truth");
	TTreeReaderValue<vector<double>> n_m_truth_o(reader, "n_m_truth");
  TTreeReaderValue<vector<bool>> n_passIDcuts_o(reader, "n_passIDcuts");
  TTreeReaderValue<vector<bool>> n_passAll_o(reader, "n_passAll");
  TTreeReaderValue<vector<float>> n_eLoss_o(reader, "n_eLoss");
  TTreeReaderValue<vector<float>> n_ptcone40_o(reader, "n_ptcone40");
  TTreeReaderValue<vector<int>> n_muonType_o(reader, "n_muonType");
  TTreeReaderValue<vector<int>> n_quality_o(reader, "n_quality");
  TTreeReaderValue<vector<int>> n_primaryAuthor_o(reader, "n_primaryAuthor");
  TTreeReaderValue<vector<int>> n_authors_o(reader, "n_authors");
  TTreeReaderValue<vector<int>> n_nPrecisionLayers_o(reader, "n_nPrecisionLayers");
  TTreeReaderValue<vector<int>> n_nPrecisionHoleLayers_o(reader, "n_nPrecisionHoleLayers");
  //TTreeReaderValue<vector<int>> n_innerSmallHits_o(reader, "n_innerSmallHits");
  //TTreeReaderValue<vector<int>> n_innerLargeHits_o(reader, "n_innerLargeHits");
  //TTreeReaderValue<vector<int>> n_middleSmallHits_o(reader, "n_middleSmallHits");
  //TTreeReaderValue<vector<int>> n_middleLargeHits_o(reader, "n_middleLargeHits");
  //TTreeReaderValue<vector<int>> n_outerSmallHits_o(reader, "n_outerSmallHits");
  //TTreeReaderValue<vector<int>> n_outerLargeHits_o(reader, "n_outerLargeHits");
  //TTreeReaderValue<vector<int>> n_extendedSmallHits_o(reader, "n_extendedSmallHits");
  //TTreeReaderValue<vector<int>> n_extendedLargeHits_o(reader, "n_extendedLargeHits"); 
  //TTreeReaderValue<vector<int>> n_innerSmallHoles_o(reader, "n_innerSmallHoles");
  //TTreeReaderValue<vector<int>> n_innerLargeHoles_o(reader, "n_innerLargeHoles");
  //TTreeReaderValue<vector<int>> n_middleSmallHoles_o(reader, "n_middleSmallHoles");
  //TTreeReaderValue<vector<int>> n_middleLargeHoles_o(reader, "n_middleLargeHoles");
  //TTreeReaderValue<vector<int>> n_outerSmallHoles_o(reader, "n_outerSmallHoles");
  //TTreeReaderValue<vector<int>> n_outerLargeHoles_o(reader, "n_outerLargeHoles");
  //TTreeReaderValue<vector<int>> n_extendedSmallHoles_o(reader, "n_extendedSmallHoles");
  //TTreeReaderValue<vector<int>> n_extendedLargeHoles_o(reader, "n_extendedLargeHoles");

  // positive muon track variables
  TTreeReaderValue<vector<bool>> p_isCB_o(reader, "p_isCB");
  TTreeReaderValue<vector<bool>> p_isME_o(reader, "p_isME");
  TTreeReaderValue<vector<bool>> p_isMSO_o(reader, "p_isMSO");
  TTreeReaderValue<vector<bool>> p_isMSOE_o(reader, "p_isMSOE");
  TTreeReaderValue<vector<bool>> p_isID_o(reader, "p_isID");
  TTreeReaderValue<vector<float>> p_eta_CB_o(reader, "p_eta_CB");
  TTreeReaderValue<vector<float>> p_eta_ME_o(reader, "p_eta_ME");
  TTreeReaderValue<vector<float>> p_eta_MSO_o(reader, "p_eta_MSO");
  TTreeReaderValue<vector<float>> p_eta_MSOE_o(reader, "p_eta_MSOE");
  TTreeReaderValue<vector<float>> p_eta_ID_o(reader, "p_eta_ID");
  TTreeReaderValue<vector<float>> p_phi_CB_o(reader, "p_phi_CB");
  TTreeReaderValue<vector<float>> p_phi_ME_o(reader, "p_phi_ME");
  TTreeReaderValue<vector<float>> p_phi_MSO_o(reader, "p_phi_MSO");
  TTreeReaderValue<vector<float>> p_phi_MSOE_o(reader, "p_phi_MSOE");
  TTreeReaderValue<vector<float>> p_phi_ID_o(reader, "p_phi_ID");
  TTreeReaderValue<vector<float>> p_d0sig_CB_o(reader, "p_d0sig_CB");
  TTreeReaderValue<vector<float>> p_d0sig_ME_o(reader, "p_d0sig_ME");
  TTreeReaderValue<vector<float>> p_d0sig_MSO_o(reader, "p_d0sig_MSO");
  TTreeReaderValue<vector<float>> p_d0sig_MSOE_o(reader, "p_d0sig_MSOE");
  TTreeReaderValue<vector<float>> p_d0sig_ID_o(reader, "p_d0sig_ID");
  TTreeReaderValue<vector<float>> p_z0_CB_o(reader, "p_z0_CB");
  TTreeReaderValue<vector<float>> p_z0_ME_o(reader, "p_z0_ME");
  TTreeReaderValue<vector<float>> p_z0_MSO_o(reader, "p_z0_MSO");
  TTreeReaderValue<vector<float>> p_z0_MSOE_o(reader, "p_z0_MSOE");
  TTreeReaderValue<vector<float>> p_z0_ID_o(reader, "p_z0_ID");
  TTreeReaderValue<vector<float>> p_qOverP_CB_o(reader, "p_qOverP_CB");
  TTreeReaderValue<vector<float>> p_qOverP_ME_o(reader, "p_qOverP_ME");
  TTreeReaderValue<vector<float>> p_qOverP_MSO_o(reader, "p_qOverP_MSO");
  TTreeReaderValue<vector<float>> p_qOverP_MSOE_o(reader, "p_qOverP_MSOE");
  TTreeReaderValue<vector<float>> p_qOverP_ID_o(reader, "p_qOverP_ID");
  TTreeReaderValue<vector<float>> p_dqOverP_CB_o(reader, "p_dqOverP_CB");
  TTreeReaderValue<vector<float>> p_dqOverP_ME_o(reader, "p_dqOverP_ME");
  TTreeReaderValue<vector<float>> p_dqOverP_MSO_o(reader, "p_dqOverP_MSO");
  TTreeReaderValue<vector<float>> p_dqOverP_MSOE_o(reader, "p_dqOverP_MSOE");
  TTreeReaderValue<vector<float>> p_dqOverP_ID_o(reader, "p_dqOverP_ID");
  TTreeReaderValue<vector<float>> p_rchi2_CB_o(reader, "p_rchi2_CB");
  TTreeReaderValue<vector<float>> p_rchi2_ME_o(reader, "p_rchi2_ME");
  TTreeReaderValue<vector<float>> p_rchi2_MSO_o(reader, "p_rchi2_MSO");
  TTreeReaderValue<vector<float>> p_rchi2_MSOE_o(reader, "p_rchi2_MSOE");
  TTreeReaderValue<vector<float>> p_rchi2_ID_o(reader, "p_rchi2_ID");
  
  // negative muon track variables
  TTreeReaderValue<vector<bool>> n_isCB_o(reader, "n_isCB");
  TTreeReaderValue<vector<bool>> n_isME_o(reader, "n_isME");
  TTreeReaderValue<vector<bool>> n_isMSO_o(reader, "n_isMSO");
  TTreeReaderValue<vector<bool>> n_isMSOE_o(reader, "n_isMSOE");
  TTreeReaderValue<vector<bool>> n_isID_o(reader, "n_isID");
  TTreeReaderValue<vector<float>> n_eta_CB_o(reader, "n_eta_CB");
  TTreeReaderValue<vector<float>> n_eta_ME_o(reader, "n_eta_ME");
  TTreeReaderValue<vector<float>> n_eta_MSO_o(reader, "n_eta_MSO");
  TTreeReaderValue<vector<float>> n_eta_MSOE_o(reader, "n_eta_MSOE");
  TTreeReaderValue<vector<float>> n_eta_ID_o(reader, "n_eta_ID");
  TTreeReaderValue<vector<float>> n_phi_CB_o(reader, "n_phi_CB");
  TTreeReaderValue<vector<float>> n_phi_ME_o(reader, "n_phi_ME");
  TTreeReaderValue<vector<float>> n_phi_MSO_o(reader, "n_phi_MSO");
  TTreeReaderValue<vector<float>> n_phi_MSOE_o(reader, "n_phi_MSOE");
  TTreeReaderValue<vector<float>> n_phi_ID_o(reader, "n_phi_ID");
  TTreeReaderValue<vector<float>> n_d0sig_CB_o(reader, "n_d0sig_CB");
  TTreeReaderValue<vector<float>> n_d0sig_ME_o(reader, "n_d0sig_ME");
  TTreeReaderValue<vector<float>> n_d0sig_MSO_o(reader, "n_d0sig_MSO");
  TTreeReaderValue<vector<float>> n_d0sig_MSOE_o(reader, "n_d0sig_MSOE");
  TTreeReaderValue<vector<float>> n_d0sig_ID_o(reader, "n_d0sig_ID");
  TTreeReaderValue<vector<float>> n_z0_CB_o(reader, "n_z0_CB");
  TTreeReaderValue<vector<float>> n_z0_ME_o(reader, "n_z0_ME");
  TTreeReaderValue<vector<float>> n_z0_MSO_o(reader, "n_z0_MSO");
  TTreeReaderValue<vector<float>> n_z0_MSOE_o(reader, "n_z0_MSOE");
  TTreeReaderValue<vector<float>> n_z0_ID_o(reader, "n_z0_ID");
  TTreeReaderValue<vector<float>> n_qOverP_CB_o(reader, "n_qOverP_CB");
  TTreeReaderValue<vector<float>> n_qOverP_ME_o(reader, "n_qOverP_ME");
  TTreeReaderValue<vector<float>> n_qOverP_MSO_o(reader, "n_qOverP_MSO");
  TTreeReaderValue<vector<float>> n_qOverP_MSOE_o(reader, "n_qOverP_MSOE");
  TTreeReaderValue<vector<float>> n_qOverP_ID_o(reader, "n_qOverP_ID");
  TTreeReaderValue<vector<float>> n_dqOverP_CB_o(reader, "n_dqOverP_CB");
  TTreeReaderValue<vector<float>> n_dqOverP_ME_o(reader, "n_dqOverP_ME");
  TTreeReaderValue<vector<float>> n_dqOverP_MSO_o(reader, "n_dqOverP_MSO");
  TTreeReaderValue<vector<float>> n_dqOverP_MSOE_o(reader, "n_dqOverP_MSOE");
  TTreeReaderValue<vector<float>> n_dqOverP_ID_o(reader, "n_dqOverP_ID");
  TTreeReaderValue<vector<float>> n_rchi2_CB_o(reader, "n_rchi2_CB");
  TTreeReaderValue<vector<float>> n_rchi2_ME_o(reader, "n_rchi2_ME");
  TTreeReaderValue<vector<float>> n_rchi2_MSO_o(reader, "n_rchi2_MSO");
  TTreeReaderValue<vector<float>> n_rchi2_MSOE_o(reader, "n_rchi2_MSOE");
  TTreeReaderValue<vector<float>> n_rchi2_ID_o(reader, "n_rchi2_ID");
  
	// open file for writing skimmed ntuple
	TFile outFile(outputRootFile.c_str(), "RECREATE");
	TTree* outTree = new TTree("RecoMuons", "Skimmed ntuple for selected muon pairs");
	
	// create branch addresses
	// event level
	bool passGRL = false;
	unsigned long long eventNumber = 0;
	outTree->Branch("passGRL", &passGRL);
	outTree->Branch("eventNumber", &eventNumber);
	
	// positive muon variables
	int p_pdgID_truth = 0;
	double p_pt_truth = 0;
	double p_eta_truth = 0;
	double p_phi_truth = 0;
	double p_m_truth = 0;
	bool p_passIDcuts = false;
	bool p_passAll = false;
	float p_ptcone40 = 0;
	int p_muonType = 0;
	int p_quality = 0;
	int p_primaryAuthor = 0;
	int p_authors = 0;
  int p_nPrecisionLayers = 0;
  int p_nPrecisionHoleLayers = 0;
	outTree->Branch("p_pdgID_truth", &p_pdgID_truth);
	outTree->Branch("p_pt_truth", &p_pt_truth);
	outTree->Branch("p_eta_truth", &p_eta_truth);
	outTree->Branch("p_phi_truth", &p_phi_truth);
	outTree->Branch("p_m_truth", &p_m_truth);
	outTree->Branch("p_passIDcuts", &p_passIDcuts);
	outTree->Branch("p_passAll", &p_passAll);
	outTree->Branch("p_ptcone40", &p_ptcone40);
	outTree->Branch("p_muonType", &p_muonType);
	outTree->Branch("p_quality", &p_quality);
	outTree->Branch("p_primaryAuthor", &p_primaryAuthor);
	outTree->Branch("p_authors", &p_authors);
	outTree->Branch("p_nPrecisionLayers", &p_nPrecisionLayers);

  // positive muon track variables
  bool p_isCB = false;
  bool p_isME = false;
  bool p_isMSO = false;
  bool p_isMSOE = false;
  bool p_isID = false;
  float p_eta_CB = 0;
  float p_eta_ME = 0;
  float p_eta_MSO = 0;
  float p_eta_MSOE = 0;
  float p_eta_ID = 0;
  float p_phi_CB = 0;
  float p_phi_ME = 0;
  float p_phi_MSO = 0;
  float p_phi_MSOE = 0;
  float p_phi_ID = 0;
  float p_z0_CB = 0;
  float p_z0_ME = 0;
  float p_z0_MSO = 0;
  float p_z0_MSOE = 0;
  float p_z0_ID = 0;
  float p_d0sig_CB = 0;
  float p_d0sig_ME = 0;
  float p_d0sig_MSO = 0;
  float p_d0sig_MSOE = 0;
  float p_d0sig_ID = 0;
  float p_qOverP_CB = 0;
  float p_qOverP_ME = 0;
  float p_qOverP_MSO = 0;
  float p_qOverP_MSOE = 0;
  float p_qOverP_ID = 0;
  float p_dqOverP_CB = 0;
  float p_dqOverP_ME = 0;
  float p_dqOverP_MSO = 0;
  float p_dqOverP_MSOE = 0;
  float p_dqOverP_ID = 0;
  float p_rchi2_CB = 0;
  float p_rchi2_ME = 0;
  float p_rchi2_MSO = 0;
  float p_rchi2_MSOE = 0;
  float p_rchi2_ID = 0;
	outTree->Branch("p_isCB", &p_isCB);
	outTree->Branch("p_isME", &p_isME);
	outTree->Branch("p_isMSO", &p_isMSO);
	outTree->Branch("p_isMSOE", &p_isMSOE);
	outTree->Branch("p_isID", &p_isID);
	outTree->Branch("p_eta_CB", &p_eta_CB);
	outTree->Branch("p_eta_ME", &p_eta_ME);
	outTree->Branch("p_eta_MSO", &p_eta_MSO);
	outTree->Branch("p_eta_MSOE", &p_eta_MSOE);
	outTree->Branch("p_eta_ID", &p_eta_ID);
	outTree->Branch("p_phi_CB", &p_phi_CB);
	outTree->Branch("p_phi_ME", &p_phi_ME);
	outTree->Branch("p_phi_MSO", &p_phi_MSO);
	outTree->Branch("p_phi_MSOE", &p_phi_MSOE);
	outTree->Branch("p_phi_ID", &p_phi_ID);
	outTree->Branch("p_z0_CB", &p_z0_CB);
	outTree->Branch("p_z0_ME", &p_z0_ME);
	outTree->Branch("p_z0_MSO", &p_z0_MSO);
	outTree->Branch("p_z0_MSOE", &p_z0_MSOE);
	outTree->Branch("p_z0_ID", &p_z0_ID);
	outTree->Branch("p_d0sig_CB", &p_d0sig_CB);
	outTree->Branch("p_d0sig_ME", &p_d0sig_ME);
	outTree->Branch("p_d0sig_MSO", &p_d0sig_MSO);
	outTree->Branch("p_d0sig_MSOE", &p_d0sig_MSOE);
	outTree->Branch("p_d0sig_ID", &p_d0sig_ID);
	outTree->Branch("p_qOverP_CB", &p_qOverP_CB);
	outTree->Branch("p_qOverP_ME", &p_qOverP_ME);
	outTree->Branch("p_qOverP_MSO", &p_qOverP_MSO);
	outTree->Branch("p_qOverP_MSOE", &p_qOverP_MSOE);
	outTree->Branch("p_qOverP_ID", &p_qOverP_ID);
	outTree->Branch("p_dqOverP_CB", &p_dqOverP_CB);
	outTree->Branch("p_dqOverP_ME", &p_dqOverP_ME);
	outTree->Branch("p_dqOverP_MSO", &p_dqOverP_MSO);
	outTree->Branch("p_dqOverP_MSOE", &p_dqOverP_MSOE);
	outTree->Branch("p_dqOverP_ID", &p_dqOverP_ID);
	outTree->Branch("p_rchi2_CB", &p_rchi2_CB);
	outTree->Branch("p_rchi2_ME", &p_rchi2_ME);
	outTree->Branch("p_rchi2_MSO", &p_rchi2_MSO);
	outTree->Branch("p_rchi2_MSOE", &p_rchi2_MSOE);
	outTree->Branch("p_rchi2_ID", &p_rchi2_ID);

	// negative muon variables
	int n_pdgID_truth = 0;
	double n_pt_truth = 0;
	double n_eta_truth = 0;
	double n_phi_truth = 0;
	double n_m_truth = 0;
	bool n_passIDcuts = false;
	bool n_passAll = false;
	float n_ptcone40 = 0;
	int n_muonType = 0;
	int n_quality = 0;
	int n_primaryAuthor = 0;
	int n_authors = 0;
  int n_nPrecisionLayers = 0;
  int n_nPrecisionHoleLayers = 0;
	outTree->Branch("n_pdgID_truth", &n_pdgID_truth);
	outTree->Branch("n_pt_truth", &n_pt_truth);
	outTree->Branch("n_eta_truth", &n_eta_truth);
	outTree->Branch("n_phi_truth", &n_phi_truth);
	outTree->Branch("n_m_truth", &n_m_truth);
	outTree->Branch("n_passIDcuts", &n_passIDcuts);
	outTree->Branch("n_passAll", &n_passAll);
	outTree->Branch("n_ptcone40", &n_ptcone40);
	outTree->Branch("n_muonType", &n_muonType);
	outTree->Branch("n_quality", &n_quality);
	outTree->Branch("n_primaryAuthor", &n_primaryAuthor);
	outTree->Branch("n_authors", &n_authors);
	outTree->Branch("n_authors", &p_authors);
	outTree->Branch("n_nPrecisionLayers", &p_nPrecisionLayers);

  // negative muon track variables
  bool n_isCB = false;
  bool n_isME = false;
  bool n_isMSO = false;
  bool n_isMSOE = false;
  bool n_isID = false;
  float n_eta_CB = 0;
  float n_eta_ME = 0;
  float n_eta_MSO = 0;
  float n_eta_MSOE = 0;
  float n_eta_ID = 0;
  float n_phi_CB = 0;
  float n_phi_ME = 0;
  float n_phi_MSO = 0;
  float n_phi_MSOE = 0;
  float n_phi_ID = 0;
  float n_z0_CB = 0;
  float n_z0_ME = 0;
  float n_z0_MSO = 0;
  float n_z0_MSOE = 0;
  float n_z0_ID = 0;
  float n_d0sig_CB = 0;
  float n_d0sig_ME = 0;
  float n_d0sig_MSO = 0;
  float n_d0sig_MSOE = 0;
  float n_d0sig_ID = 0;
  float n_qOverP_CB = 0;
  float n_qOverP_ME = 0;
  float n_qOverP_MSO = 0;
  float n_qOverP_MSOE = 0;
  float n_qOverP_ID = 0;
  float n_dqOverP_CB = 0;
  float n_dqOverP_ME = 0;
  float n_dqOverP_MSO = 0;
  float n_dqOverP_MSOE = 0;
  float n_dqOverP_ID = 0;
  float n_rchi2_CB = 0;
  float n_rchi2_ME = 0;
  float n_rchi2_MSO = 0;
  float n_rchi2_MSOE = 0;
  float n_rchi2_ID = 0;
	outTree->Branch("n_isCB", &n_isCB);
	outTree->Branch("n_isME", &n_isME);
	outTree->Branch("n_isMSO", &n_isMSO);
	outTree->Branch("n_isMSOE", &n_isMSOE);
	outTree->Branch("n_isID", &n_isID);
	outTree->Branch("n_eta_CB", &n_eta_CB);
	outTree->Branch("n_eta_ME", &n_eta_ME);
	outTree->Branch("n_eta_MSO", &n_eta_MSO);
	outTree->Branch("n_eta_MSOE", &n_eta_MSOE);
	outTree->Branch("n_eta_ID", &n_eta_ID);
	outTree->Branch("n_phi_CB", &n_phi_CB);
	outTree->Branch("n_phi_ME", &n_phi_ME);
	outTree->Branch("n_phi_MSO", &n_phi_MSO);
	outTree->Branch("n_phi_MSOE", &n_phi_MSOE);
	outTree->Branch("n_phi_ID", &n_phi_ID);
	outTree->Branch("n_z0_CB", &n_z0_CB);
	outTree->Branch("n_z0_ME", &n_z0_ME);
	outTree->Branch("n_z0_MSO", &n_z0_MSO);
	outTree->Branch("n_z0_MSOE", &n_z0_MSOE);
	outTree->Branch("n_z0_ID", &n_z0_ID);
	outTree->Branch("n_d0sig_CB", &n_d0sig_CB);
	outTree->Branch("n_d0sig_ME", &n_d0sig_ME);
	outTree->Branch("n_d0sig_MSO", &n_d0sig_MSO);
	outTree->Branch("n_d0sig_MSOE", &n_d0sig_MSOE);
	outTree->Branch("n_d0sig_ID", &n_d0sig_ID);
	outTree->Branch("n_qOverP_CB", &n_qOverP_CB);
	outTree->Branch("n_qOverP_ME", &n_qOverP_ME);
	outTree->Branch("n_qOverP_MSO", &n_qOverP_MSO);
	outTree->Branch("n_qOverP_MSOE", &n_qOverP_MSOE);
	outTree->Branch("n_qOverP_ID", &n_qOverP_ID);
	outTree->Branch("n_dqOverP_CB", &n_dqOverP_CB);
	outTree->Branch("n_dqOverP_ME", &n_dqOverP_ME);
	outTree->Branch("n_dqOverP_MSO", &n_dqOverP_MSO);
	outTree->Branch("n_dqOverP_MSOE", &n_dqOverP_MSOE);
	outTree->Branch("n_dqOverP_ID", &n_dqOverP_ID);
	outTree->Branch("n_rchi2_CB", &n_rchi2_CB);
	outTree->Branch("n_rchi2_ME", &n_rchi2_ME);
	outTree->Branch("n_rchi2_MSO", &n_rchi2_MSO);
	outTree->Branch("n_rchi2_MSOE", &n_rchi2_MSOE);
	outTree->Branch("n_rchi2_ID", &n_rchi2_ID);

  // loop through chained ntuples
  while (reader.Next()) {
    // skip events without a candidate zmumu event (only for official AODs)
		if (isAOD) {
      vector<TLorentzVector> iso_muons;
      TLorentzVector mu_4vec;
      double pt = 0, eta = 0, phi = 0;
      for (int i = 0; i < *nPositiveMuons_o; i++) {
        if (p_isCB_o->at(i)) {
          pt = 1 / cosh(p_eta_CB_o->at(i)) / abs(p_qOverP_CB_o->at(i));
          eta = p_eta_CB_o->at(i);
          phi = p_phi_CB_o->at(i);
        }
        else if (p_isME_o->at(i)) {
          pt = 1 / cosh(p_eta_ME_o->at(i)) / abs(p_qOverP_ME_o->at(i));
          eta = p_eta_ME_o->at(i);
          phi = p_phi_ME_o->at(i);
        }
        else {
          if (!p_isID_o->at(i)) cout << "WARNING: muon is not ID" << endl;
          pt = 1 / cosh(p_eta_ID_o->at(i)) / abs(p_qOverP_ID_o->at(i));
          eta = p_eta_ID_o->at(i);
          phi = p_phi_ID_o->at(i);
        }
        if (pt < 20) continue; 
        if (p_ptcone40_o->at(i)/pt >= 0.3) continue; 
        mu_4vec.SetPtEtaPhiM(pt, eta, phi, .10566);
        iso_muons.push_back(mu_4vec);  
      }

      for (int i = 0; i < *nNegativeMuons_o; i++) {
        if (n_isCB_o->at(i)) {
          pt = 1 / cosh(n_eta_CB_o->at(i)) / abs(n_qOverP_CB_o->at(i));
          eta = n_eta_CB_o->at(i);
          phi = n_phi_CB_o->at(i);
        }
        else if (n_isME_o->at(i)) {
          pt = 1 / cosh(n_eta_ME_o->at(i)) / abs(n_qOverP_ME_o->at(i));
          eta = n_eta_ME_o->at(i);
          phi = n_phi_ME_o->at(i);
        }
        else {
          if (!n_isID_o->at(i)) cout << "WARNING: muon is not ID" << endl;
          pt = 1 / cosh(n_eta_ID_o->at(i)) / abs(n_qOverP_ID_o->at(i));
          eta = n_eta_ID_o->at(i);
          phi = n_phi_ID_o->at(i);
        }
        if (pt < 20) continue; 
        if (n_ptcone40_o->at(i)/pt >= 0.3) continue; 
        mu_4vec.SetPtEtaPhiM(pt, eta, phi, .10566);
        iso_muons.push_back(mu_4vec);  
      }
      
      bool pass = false;
      for (int i = 0; i < iso_muons.size(); i++) {
        if (pass) break;
        for (int j = 0; j < i; j++) {
          if (iso_muons.at(i).Pt() < 25. && iso_muons.at(j).Pt() < 25.) continue;
          double m  = (iso_muons.at(i) + iso_muons.at(j)).M();
          if (70 < m && m < 110) {pass = true; break;}
        }
      }
      if (!pass) continue;    

		}
   
    // skip events without opposite sign pair
    if (*nPositiveMuons_o < 1 or *nNegativeMuons_o < 1) continue;

    // event level
	  passGRL = *passGRL_o;
	  eventNumber = *eventNumber_o;
	
  	// positive muon variables
  	p_pdgID_truth           = p_pdgID_truth_o->at(0);
  	p_pt_truth              = p_pt_truth_o->at(0);
  	p_eta_truth             = p_eta_truth_o->at(0);
	  p_phi_truth             = p_phi_truth_o->at(0);
	  p_m_truth               = p_m_truth_o->at(0);
	  p_passIDcuts            = p_passIDcuts_o->at(0);
	  p_passAll               = p_passAll_o->at(0);
  	p_ptcone40              = p_ptcone40_o->at(0);
  	p_muonType              = p_muonType_o->at(0);
  	p_quality               = p_quality_o->at(0);
  	p_primaryAuthor         = p_primaryAuthor_o->at(0);
  	p_authors               = p_authors_o->at(0);
    p_nPrecisionLayers      = p_nPrecisionLayers_o->at(0);
    p_nPrecisionHoleLayers  = p_nPrecisionHoleLayers_o->at(0);
    
    p_isCB            = p_isCB_o->at(0);
    p_isME            = p_isME_o->at(0);
    p_isMSO           = p_isMSO_o->at(0);
    p_isMSOE          = p_isMSOE_o->at(0);
    p_isID            = p_isID_o->at(0);
    p_eta_CB          = p_eta_CB_o->at(0);
    p_eta_ME          = p_eta_ME_o->at(0);
    p_eta_MSO         = p_eta_MSO_o->at(0);
    p_eta_MSOE        = p_eta_MSOE_o->at(0);
    p_eta_ID          = p_eta_ID_o->at(0);
    p_phi_CB          = p_phi_CB_o->at(0);
    p_phi_ME          = p_phi_ME_o->at(0);
    p_phi_MSO         = p_phi_MSO_o->at(0);
    p_phi_MSOE        = p_phi_MSOE_o->at(0);
    p_phi_ID          = p_phi_ID_o->at(0);
    p_z0_CB           = p_z0_CB_o->at(0);
    p_z0_ME           = p_z0_ME_o->at(0);
    p_z0_MSO          = p_z0_MSO_o->at(0);
    p_z0_MSOE         = p_z0_MSOE_o->at(0);
    p_z0_ID           = p_z0_ID_o->at(0);
    p_d0sig_CB        = p_d0sig_CB_o->at(0);
    p_d0sig_ME        = p_d0sig_ME_o->at(0);
    p_d0sig_MSO       = p_d0sig_MSO_o->at(0);
    p_d0sig_MSOE      = p_d0sig_MSOE_o->at(0);
    p_d0sig_ID        = p_d0sig_ID_o->at(0);
    p_qOverP_CB       = p_qOverP_CB_o->at(0);
    p_qOverP_ME       = p_qOverP_ME_o->at(0);
    p_qOverP_MSO      = p_qOverP_MSO_o->at(0);
    p_qOverP_MSOE     = p_qOverP_MSOE_o->at(0);
    p_qOverP_ID       = p_qOverP_ID_o->at(0);
    p_dqOverP_CB      = p_dqOverP_CB_o->at(0);
    p_dqOverP_ME      = p_dqOverP_ME_o->at(0);
    p_dqOverP_MSO     = p_dqOverP_MSO_o->at(0);
    p_dqOverP_MSOE    = p_dqOverP_MSOE_o->at(0);
    p_dqOverP_ID      = p_dqOverP_ID_o->at(0);
    p_rchi2_CB        = p_rchi2_CB_o->at(0);
    p_rchi2_ME        = p_rchi2_ME_o->at(0);
    p_rchi2_MSO       = p_rchi2_MSO_o->at(0);
    p_rchi2_MSOE      = p_rchi2_MSOE_o->at(0);
    p_rchi2_ID        = p_rchi2_ID_o->at(0);

  	// negative muon variables
  	n_pdgID_truth           = n_pdgID_truth_o->at(0);
  	n_pt_truth              = n_pt_truth_o->at(0);
  	n_eta_truth             = n_eta_truth_o->at(0);
	  n_phi_truth             = n_phi_truth_o->at(0);
	  n_m_truth               = n_m_truth_o->at(0);
	  n_passIDcuts            = n_passIDcuts_o->at(0);
	  n_passAll               = n_passAll_o->at(0);
  	n_ptcone40              = n_ptcone40_o->at(0);
  	n_muonType              = n_muonType_o->at(0);
  	n_quality               = n_quality_o->at(0);
  	n_primaryAuthor         = n_primaryAuthor_o->at(0);
  	n_authors               = n_authors_o->at(0);
    n_nPrecisionLayers      = n_nPrecisionLayers_o->at(0);
    n_nPrecisionHoleLayers  = n_nPrecisionHoleLayers_o->at(0);
    
    n_isCB            = n_isCB_o->at(0);
    n_isME            = n_isME_o->at(0);
    n_isMSO           = n_isMSO_o->at(0);
    n_isMSOE          = n_isMSOE_o->at(0);
    n_isID            = n_isID_o->at(0);
    n_eta_CB          = n_eta_CB_o->at(0);
    n_eta_ME          = n_eta_ME_o->at(0);
    n_eta_MSO         = n_eta_MSO_o->at(0);
    n_eta_MSOE        = n_eta_MSOE_o->at(0);
    n_eta_ID          = n_eta_ID_o->at(0);
    n_phi_CB          = n_phi_CB_o->at(0);
    n_phi_ME          = n_phi_ME_o->at(0);
    n_phi_MSO         = n_phi_MSO_o->at(0);
    n_phi_MSOE        = n_phi_MSOE_o->at(0);
    n_phi_ID          = n_phi_ID_o->at(0);
    n_z0_CB           = n_z0_CB_o->at(0);
    n_z0_ME           = n_z0_ME_o->at(0);
    n_z0_MSO          = n_z0_MSO_o->at(0);
    n_z0_MSOE         = n_z0_MSOE_o->at(0);
    n_z0_ID           = n_z0_ID_o->at(0);
    n_d0sig_CB        = n_d0sig_CB_o->at(0);
    n_d0sig_ME        = n_d0sig_ME_o->at(0);
    n_d0sig_MSO       = n_d0sig_MSO_o->at(0);
    n_d0sig_MSOE      = n_d0sig_MSOE_o->at(0);
    n_d0sig_ID        = n_d0sig_ID_o->at(0);
    n_qOverP_CB       = n_qOverP_CB_o->at(0);
    n_qOverP_ME       = n_qOverP_ME_o->at(0);
    n_qOverP_MSO      = n_qOverP_MSO_o->at(0);
    n_qOverP_MSOE     = n_qOverP_MSOE_o->at(0);
    n_qOverP_ID       = n_qOverP_ID_o->at(0);
    n_dqOverP_CB      = n_dqOverP_CB_o->at(0);
    n_dqOverP_ME      = n_dqOverP_ME_o->at(0);
    n_dqOverP_MSO     = n_dqOverP_MSO_o->at(0);
    n_dqOverP_MSOE    = n_dqOverP_MSOE_o->at(0);
    n_dqOverP_ID      = n_dqOverP_ID_o->at(0);
    n_rchi2_CB        = n_rchi2_CB_o->at(0);
    n_rchi2_ME        = n_rchi2_ME_o->at(0);
    n_rchi2_MSO       = n_rchi2_MSO_o->at(0);
    n_rchi2_MSOE      = n_rchi2_MSOE_o->at(0);
    n_rchi2_ID        = n_rchi2_ID_o->at(0);
    
    outTree->Fill();
	}
	outTree->Write("RecoMuons");
	delete outTree;
	outFile.Close();
}

void DoSkimming() {
  string RunIntuples = "AODtestfiles.txt";
  string RunIoutputNtuple = "RunI_skimmedNtuple_preselection.root";
  cout << "Skimming ntuples in text file " << RunIntuples 
       << " and saving output to " << RunIoutputNtuple<< endl;
  SkimNtuples(RunIntuples, RunIoutputNtuple, true);
}
