#include "MuonAnalysis.h"
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TH1.h>
#include <TH2.h>
#include <TCut.h>
#include <TCanvas.h>
#include <vector>
#include <math.h>

MuonAnalysis::MuonAnalysis(const char *inputTreePath) {
  // Initialize histograms
  h_m = new TH1F("h_m","",40,0,140);
  h_m->SetTitle(";m_{#mu#bar{#mu}} (GeV);Events");
  h_m_nc = (TH1F*)h_m->Clone("h_m_nc");
  h_eta = new TH1F("h_eta","",30,-2.7,2.7);
  h_eta->SetTitle(";#eta;Events");
  h_eta_nc = (TH1F*)h_eta->Clone("h_eta_nc");
  h_d0sig = new TH1F("h_d0sig","",30,-4.5,4.5);
  h_d0sig->SetTitle(";d_{0} Significance;Events");
  h_d0sig_nc = (TH1F*)h_d0sig->Clone("h_d0sig_nc");
  h_z0 = new TH1F("h_z0","",40,-3,3);
  h_z0->SetTitle(";z_{0} (mm);Events");
  h_z0_nc = (TH1F*)h_z0->Clone("h_z0_nc");
  h_z0sintheta = new TH1F("h_z0sintheta","",40,-1,1);
  h_z0sintheta->SetTitle(";z_{0} sin#theta (mm);Events");
  h_z0sintheta_nc = (TH1F*)h_z0sintheta->Clone("h_z0sintheta_nc");

  h_phi_bg = new TH1F("h_phi_bg","",15,-3.15,3.15);
  h_phi_bg->SetTitle("Backgrounds;#phi;Events");
  h_etaeta_bg = new TH1F("h_etaeta","",15,-5,5);
  h_etaeta_bg->SetTitle("Backgrounds;#eta_{#mu_{1}}+#eta_{#mu_{2}};Events");
  h_phiphi_bg = new TH1F("h_phiphi","",15,-2*3.15,2*3.15);
  h_phiphi_bg->SetTitle("Backgrounds;#phi_{#mu_{1}}-#phi_{#mu_{2}};Events");
  h_z0_bg = new TH1F("h_z0_bg","",15,-200,200);
  h_z0_bg->SetTitle("Backgrounds;z_{0} (mm);Events");
  hh_z0d0sig_bg = new TH2F("hh_z0d0sig_bg","",15,-200,200,15,-5,5);
  hh_z0d0sig_bg->SetTitle("Backgrounds;z_{0} (mm);d_{0} Significance");
  hh_phieta_bg = new TH2F("hh_phieta_bg","",15,-2*3.15,2*3.15,15,-5,5);
  hh_phieta_bg->SetTitle("Backgrounds;#phi_{#mu_{1}}-#phi_{#mu_{2}};#eta_{#mu_{1}}+#eta_{#mu_{2}}");

  // Open ntuple
  TFile* file = new TFile(inputTreePath);

  // Create TTreeReaders for branches
  TTreeReader reader("muonAnalysis", file);
  TTreeReaderValue<vector<float>> pt(reader, "Pt");
  TTreeReaderValue<vector<float>> eta(reader, "Eta");
  TTreeReaderValue<vector<float>> phi(reader, "Phi");
  TTreeReaderValue<vector<float>> e(reader, "E");
  TTreeReaderValue<vector<float>> eLoss(reader, "EnergyLoss");
  TTreeReaderValue<vector<float>> d0sig(reader, "d0sig");
  TTreeReaderValue<vector<float>> z0(reader, "z0");
  TTreeReaderValue<vector<float>> charge(reader, "Charge");
  TTreeReaderValue<vector<int>> muonType(reader, "MuonType");
  TTreeReaderValue<vector<int>> quality(reader, "Quality");
  TTreeReaderValue<vector<bool>> passesIDCuts(reader, "PassesIDCuts");
  TTreeReaderValue<vector<bool>> acceptedMuon(reader, "AcceptedMuon");
  
  int cf_total = 0,
      cf_passesIDCuts = 0,
      cf_passesQuality = 0,
      cf_passesEta = 0,
      cf_passesd0sig = 0,
      cf_passesz0sintheta = 0;

  int n_passesIDCuts = 0,
      n_passesIDCutsBoth = 0,
      n_passesIDCutsOne = 0;

  std::cout << "==============================================" << std::endl
            << "TTreeReaders have been initialized. Begin loop." << std::endl;

  while (reader.Next()) {
    if (pt->size() < 2) continue; 
    
    int i_mumin = -1, i_mupos = -1;
    for (int i = 0; i < pt->size(); i++) {
      // If muon
      if (charge->at(i) == -1. && muonType->at(i) == 0) {
        if (i_mumin == -1) {
          i_mumin = i;
        }
        else if (pt->at(i) > pt->at(i)) {
          i_mumin = i;
        }
      }
      // If anti-muon
      if (charge->at(i) == 1. && muonType->at(i) == 0) {
        if (i_mupos == -1) {
          i_mupos = i;
        }
        else if (pt->at(i) > pt->at(i)) {
          i_mupos = i;
        }
      }
    
      if (passesIDCuts->at(i))
        n_passesIDCuts++;
    }
 
    // Check to see if muon pair was selected 
    if (i_mumin == -1 || i_mupos == -1) continue;
    cf_total++;

    // Count selected muons that pass the ID cuts  
    if (passesIDCuts->at(i_mumin) && passesIDCuts->at(i_mupos))
        n_passesIDCutsBoth++;
    else if (passesIDCuts->at(i_mumin) || passesIDCuts->at(i_mumin))
        n_passesIDCutsOne++;

    // Compute invariant mass
    float m2 = 2 * pt->at(i_mumin) * pt->at(i_mupos)
                 * (cosh(eta->at(i_mumin)-eta->at(i_mupos)) - cos(phi->at(i_mumin)-phi->at(i_mupos)));

    // Fill plots for backgrounds
    if (sqrt(m2) < 70) {
      h_phi_bg->Fill(phi->at(i_mumin)); h_phi_bg->Fill(phi->at(i_mupos));
      h_etaeta_bg->Fill(eta->at(i_mumin)+eta->at(i_mupos));
      h_phiphi_bg->Fill(phi->at(i_mumin)-phi->at(i_mupos));
      h_z0_bg->Fill(z0->at(i_mumin)); h_z0_bg->Fill(z0->at(i_mupos));
      hh_z0d0sig_bg->Fill(z0->at(i_mumin),d0sig->at(i_mumin));
      hh_z0d0sig_bg->Fill(z0->at(i_mupos),d0sig->at(i_mupos)); 
      hh_phieta_bg->Fill(phi->at(i_mumin)-phi->at(i_mupos),eta->at(i_mumin)+eta->at(i_mupos));
    }
          
    // Fill histograms with no cuts
    h_m_nc->Fill(sqrt(m2));
    h_eta_nc->Fill(eta->at(i_mumin)); h_eta_nc->Fill(eta->at(i_mupos));
    h_d0sig_nc->Fill(d0sig->at(i_mumin)); h_d0sig_nc->Fill(d0sig->at(i_mupos));
    h_z0_nc->Fill(z0->at(i_mumin)); h_z0_nc->Fill(z0->at(i_mupos));
    h_z0sintheta_nc->Fill(z0->at(i_mumin) * (2*exp(-eta->at(i_mumin))/(exp(-2*eta->at(i_mumin))+1)));
    h_z0sintheta_nc->Fill(z0->at(i_mupos) * (2*exp(-eta->at(i_mupos))/(exp(-2*eta->at(i_mupos))+1)));
 
    // Skip events that fail the selection
    if (!passesIDCuts->at(i_mupos) || !passesIDCuts->at(i_mumin)) continue;
    cf_passesIDCuts++;

    if (quality->at(i_mumin) > 1 || quality->at(i_mupos) > 1) continue;
    cf_passesQuality++;

    if (fabs(eta->at(i_mumin)) > 2.5 || fabs(eta->at(i_mupos)) > 2.5) continue;
    cf_passesEta++;

    if (fabs(d0sig->at(i_mumin)) > 3 || fabs(d0sig->at(i_mupos)) > 3) continue;
    cf_passesd0sig++;

    if (fabs(z0->at(i_mumin) * (2*exp(-eta->at(i_mumin))/(exp(-2*eta->at(i_mumin))+1)+1)) > 0.5 
        || fabs(z0->at(i_mupos) * (2*exp(-eta->at(i_mupos))/(exp(-2*eta->at(i_mupos))+1))) > 0.5) continue;
    cf_passesz0sintheta++;
  
    // Fill histograms with events which pass the cuts
    h_m->Fill(sqrt(m2));
    h_eta->Fill(eta->at(i_mumin)); h_eta->Fill(eta->at(i_mupos));
    h_d0sig->Fill(d0sig->at(i_mumin)); h_d0sig->Fill(d0sig->at(i_mupos));
    h_z0->Fill(z0->at(i_mumin)); h_z0->Fill(z0->at(i_mupos));
    h_z0sintheta->Fill(z0->at(i_mumin) * (2*exp(-eta->at(i_mumin))/(exp(-2*eta->at(i_mumin))+1)));
    h_z0sintheta->Fill(z0->at(i_mupos) * (2*exp(-eta->at(i_mupos))/(exp(-2*eta->at(i_mupos))+1)));
  }
 
  std::cout << "CHECK ID CUT SELECTION" << std::endl
            << "==============================================" << std::endl
            << "Number of muons which pass ID cuts: " << n_passesIDCuts << std::endl
            << "Number of events where both selected muons pass ID cuts: " << n_passesIDCutsBoth << std::endl
            << "Number of events where one of the selected muons passs ID cuts: " << n_passesIDCutsOne << std::endl;
 
  std::cout << "CUTFLOW" << std::endl
            << "==============================================" << std::endl
            << "Total muon pairs selected: " << cf_total << " (" << (float)cf_total/(float)cf_total*100. << "\%)" << std::endl
            << "Pass ID cuts: " << cf_passesIDCuts << " (" << (float)cf_passesIDCuts/(float)cf_total*100. << "\%)"<< std::endl
            << "Pass quality: " << cf_passesQuality << " (" << (float)cf_passesQuality/(float)cf_total*100. << "\%)"<< std::endl
            << "Pass |eta| < 2.5: " << cf_passesEta << " (" << (float)cf_passesEta/(float)cf_total*100. << "\%)"<< std::endl
            << "Pass |d0sig| < 3: " << cf_passesd0sig << " (" << (float)cf_passesd0sig/(float)cf_total*100. << "\%)"<< std::endl
            << "Pass |z0*sin(theta)| < 0.5 mm: " << cf_passesz0sintheta << " (" << (float)cf_passesz0sintheta/(float)cf_total*100. << "\%)"<< std::endl;

  delete file;
}


MuonAnalysis::~MuonAnalysis() {
  // delete all private fields
  delete h_m;
  delete h_eta;
  delete h_d0sig;
  delete h_z0;
  delete h_z0sintheta;
  
  delete h_m_nc;
  delete h_eta_nc;
  delete h_d0sig_nc;
  delete h_z0_nc;
  delete h_z0sintheta_nc;

  delete h_etaeta_bg;
  delete h_phiphi_bg;
  delete h_z0_bg;
  delete h_phi_bg;
  delete hh_z0d0sig_bg;
  delete hh_phieta_bg;
}

void MuonAnalysis::FormatHistograms() {
  std::vector<TH1F*>* hists = new vector<TH1F*>();
  hists->push_back(h_m_nc);
  hists->push_back(h_eta_nc);
  hists->push_back(h_d0sig_nc);
  hists->push_back(h_z0_nc);
  hists->push_back(h_z0sintheta_nc);
  hists->push_back(h_etaeta_bg);
  hists->push_back(h_phiphi_bg);
  hists->push_back(h_z0_bg);
  hists->push_back(h_phi_bg);
  
  std::vector<TH1F*>* redHists = new vector<TH1F*>(); 
  redHists->push_back(h_m);
  redHists->push_back(h_eta);
  redHists->push_back(h_d0sig);
  redHists->push_back(h_z0);
  redHists->push_back(h_z0sintheta);

  for (TH1F* hist : *hists) {
    hist->SetLineColor(kBlack);
    hist->SetLineWidth(2);
    hist->SetMinimum(0);
  }
  
  for (TH1F* hist : *redHists) {
    hist->SetLineColor(kRed);
    hist->SetLineWidth(2);
    hist->SetMinimum(0);
  }
}

void MuonAnalysis::PrintHistograms(bool write = true) {
  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  
  std::cout << "Write to file: " << (bool)write << std::endl;

  // Create canvas
  TCanvas* can = new TCanvas("can", "",800,800);
  
  // With and without selection
  h_m_nc->Draw("hist"); h_m->Draw("hist same");
  if (write) can->Print("plots/muon_invariantMass_noIDCuts.png");
  h_eta_nc->Draw("hist"); h_eta->Draw("hist same");
  if (write) can->Print("plots/muon_eta_noIDCuts.png");
  h_d0sig_nc->Draw("hist"); h_d0sig->Draw("hist same");
  if (write) can->Print("plots/muon_d0sig_noIDCuts.png");
  h_z0_nc->Draw("hist"); h_z0->Draw("hist same");
  if (write) can->Print("plots/muon_z0_noIDCuts.png");
  h_z0sintheta_nc->Draw("hist"); h_z0sintheta->Draw("hist same");
  if (write) can->Print("plots/muon_z0sintheta_noIDCuts.png");

  // Background plots
  h_etaeta_bg->Draw("hist"); 
  if (write) can->Print("plots/muon_background_h_etaeta.png");
  h_phiphi_bg->Draw("hist");   
  if (write) can->Print("plots/muon_background_h_phiphi.png");
  h_z0_bg->Draw("hist");   
  if (write) can->Print("plots/muon_background_h_z0.png");
  h_phi_bg->Draw("hist");
  if (write) can->Print("plots/muon_background_h_phi.png");
  hh_z0d0sig_bg->Draw("COLZ");   
  if (write) can->Print("plots/muon_background_hh_z0d0sig.png");
  hh_phieta_bg->Draw("COLZ");   
  if (write) can->Print("plots/muon_background_hh_phieta.png");
}
