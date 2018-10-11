#include <TH1.h>
#include <TH2.h>

class MuonAnalysis {
  public:
    MuonAnalysis(const char *inputTreePath);
    ~MuonAnalysis();
    void FormatHistograms();
    void PrintHistograms(bool write = true);

  private:
    TH1F* h_m_nc = nullptr;
    TH1F* h_eta_nc = nullptr;
    TH1F* h_d0sig_nc = nullptr;
    TH1F* h_z0_nc = nullptr;
    TH1F* h_z0sintheta_nc = nullptr;

    TH1F* h_m = nullptr;                                                                    
    TH1F* h_eta = nullptr;
    TH1F* h_d0sig = nullptr;
    TH1F* h_z0 = nullptr;
    TH1F* h_z0sintheta = nullptr; 

    TH1F* h_etaeta_bg = nullptr;
    TH1F* h_phiphi_bg = nullptr;
    TH1F* h_z0_bg = nullptr;
    TH1F* h_phi_bg = nullptr;
    TH2F* hh_phieta_bg = nullptr;
    TH2F* hh_z0d0sig_bg = nullptr;
};
