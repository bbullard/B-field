#include <TTree.h>
#include <TH1.h>
#include <math.h>
#include <string>
#include <TCanvas.h>
#include <vector>
#include <string.h>
#include <TTreeReader.h>
#include <TFile.h>
#include <TLegend.h>
#include <TF1.h>
#include <TPaveText.h>
#include <TLorentzVector.h>

using namespace std;

/*
 * Function for creating a ratio plot for histograms h_o and h_n, corresponding to the same quantity
 * plotted with the same binning between the old and new b-field reconstruction. Also received a
 * legend and label to display on the plot, as well as the name of the old map and new map to be
 * displayed on the ratio plot y-axis. Receives a flag print, which shows the plots with user-
 * prompted double click to progress through for checking that the format is good. If it is true, 
 * no plotting delay appears and plots are directly printed. Prefix is for flagging data set 
 * comparisons e.g. prefix = "user.00339396.Run1_2016_"
 */
void MakeRatioPlot(TCanvas *c, TH1F* h_o, TH1F* h_n, TLegend *l, TPaveText *p, 
                  string oldMapName, string newMapName, bool print = false, string file_prefix="") {
 
  string name = h_o->GetName();
  name = name.substr(2,name.length()-4);

  c->cd();
  TPad plotPad("plotPad", "plotPad", 0, 0.3, 1, 1.0);
  plotPad.SetBottomMargin(0); 
  plotPad.SetTickx();
  plotPad.SetTicky();                 
  plotPad.Draw();
  plotPad.cd();
  h_o->Draw("hist");
  h_n->Draw("hist same");
  l->Draw(); 
  if (p) 
    p->Draw();
  plotPad.RedrawAxis();
  
  TH1F* ratio = new TH1F(*h_n);
  ratio->Divide(h_o);
  ratio->GetYaxis()->SetTitle(Form("%s/%s", newMapName.c_str(), oldMapName.c_str()));
  ratio->SetLineColor(kBlack);
  ratio->SetMinimum(0.78);
  ratio->SetMaximum(1.24);
  ratio->SetMarkerStyle(20);
  ratio->GetXaxis()->SetTitleSize(0.15);//.2
  ratio->GetXaxis()->SetLabelSize(0.12);//.15
  ratio->GetXaxis()->SetTitleOffset(0.95);
  ratio->GetYaxis()->SetTitleSize(0.1);//.15
  ratio->GetYaxis()->SetLabelSize(0.11);//.15
  ratio->GetYaxis()->SetTitleOffset(0.45);
  ratio->GetYaxis()->SetNdivisions(505);//503  
 
  c->cd();
  TPad ratioPad("ratioPad", "ratioPad", 0, 0.05, 1, 0.3);
  ratioPad.SetTopMargin(0);
  ratioPad.SetBottomMargin(0.4);  
  ratioPad.SetTickx();
  ratioPad.SetTicky(); 
  ratioPad.SetGridy();
  ratioPad.Draw();
  ratioPad.cd();
  ratio->Draw("p");
  ratioPad.RedrawAxis();
  c->cd();

  newMapName.erase (std::remove (newMapName.begin(), newMapName.end(), ' '), newMapName.end());
  oldMapName.erase (std::remove (oldMapName.begin(), oldMapName.end(), ' '), oldMapName.end());

  if (print) c->Print(Form("plots/%s%s_%s_vs_%s.png", file_prefix.c_str(), name.c_str(), oldMapName.c_str(), newMapName.c_str()));
  delete ratio;
 
  ratioPad.Close();
  plotPad.Close();
}

/**************************************************************************************************/

void MakeProfile(TCanvas *c, TProfile *p_o, TProfile *p_n, string oldMapName, string newMapName, 
                  bool print = false, string file_prefix="") {
  gStyle->SetHatchesSpacing(1.5);
  string name = p_o->GetName();
  string type = name.substr(name.length()-3,2);
  size_t pos = name.find("_");
  name = name.substr(2,pos-2);

  std::cout << name << " " << type << std::endl;
  
  p_o->SetLineColor(12);
  p_o->SetLineWidth(2);
  p_o->SetMarkerColor(kBlack);
  p_o->SetMarkerStyle(20);
  p_o->SetFillColor(17);
  p_o->SetFillStyle(1001);

  double maxt = p_o->GetMaximum();
  double mint = p_o->GetMinimum();
  p_o->SetMaximum(maxt+(maxt-mint)/10.5);
  p_o->SetMinimum(mint);
  p_o->GetYaxis()->SetNdivisions(204);
  p_o->GetYaxis()->SetTitleSize(0.06);
  p_o->GetYaxis()->SetLabelSize(0.059);
  p_o->GetYaxis()->SetTitleOffset(0.75);
  p_o->GetYaxis()->SetNdivisions(508);  
  
  p_n->SetLineColor(kAzure-4);
  p_n->SetLineWidth(2);
  p_n->SetMarkerColor(kAzure-2);
  p_n->SetMarkerStyle(20);
  p_n->SetFillColor(38);
  p_n->SetFillStyle(3144);
 
  TLegend *l = new TLegend(.3,.77,.7,.85);
  l->SetNColumns(2);
  l->AddEntry(p_o, Form("%s Map",oldMapName.c_str()), "fp");
  l->AddEntry(p_n, Form("%s Map",newMapName.c_str()), "fp");
 
  c->cd();
  TPad pad("minPad", "minPad", 0, 0.0, 1, 1);
  pad.SetTopMargin(0);
  pad.SetBottomMargin(0.2);
  pad.SetTickx();
  pad.SetTicky();
  pad.SetGridy();
  pad.Draw();
  pad.cd();
  p_o->Draw("e2 9");
  p_n->Draw("e2 9 same");
  pad.RedrawAxis();
  c->cd();

  newMapName.erase (std::remove (newMapName.begin(), newMapName.end(), ' '), newMapName.end());
  oldMapName.erase (std::remove (oldMapName.begin(), oldMapName.end(), ' '), oldMapName.end());
 
  if (print) c->Print(Form("plots/%s%s_%s_Profile_%s_vs_%s.png", file_prefix.c_str(), name.c_str(), type.c_str(), oldMapName.c_str(), newMapName.c_str()));

  delete l;
}

/**************************************************************************************************/

void MakeChargeSeparatedProfile(TCanvas *c, TProfile *p_p_o, TProfile *p_m_o, TProfile *p_p_n, TProfile *p_m_n,
                  string oldMapName, string newMapName, bool print = false, string file_prefix="") {
  gStyle->SetHatchesSpacing(1.5);
  string name = p_p_o->GetName();
  string type = name.substr(name.length()-3,2);
  size_t pos = name.find("_");
  name = name.substr(2,pos-2);

  std::cout << name << " " << type << std::endl;
  
  p_p_o->SetLineColor(12);
  p_p_o->SetLineWidth(2);
  p_p_o->SetMarkerColor(kBlack);
  p_p_o->SetMarkerStyle(20);
  p_p_o->SetFillColor(17);
  p_p_o->SetFillStyle(1001);

  double maxt = p_p_o->GetMaximum();
  double mint = p_p_o->GetMinimum();
  p_p_o->SetMaximum(maxt+(maxt-mint)/10.5);
  p_p_o->SetMinimum(mint);
  p_p_o->GetYaxis()->SetNdivisions(204);
  p_p_o->GetYaxis()->SetTitleSize(0.06);
  p_p_o->GetYaxis()->SetLabelSize(0.059);
  p_p_o->GetYaxis()->SetTitleOffset(0.75);
  p_p_o->GetYaxis()->SetNdivisions(508);  
  
  p_m_o->SetLineColor(12);
  p_m_o->SetLineWidth(2);
  p_m_o->SetMarkerColor(kBlack);
  p_m_o->SetMarkerStyle(20);
  p_m_o->SetFillColor(17);
  p_m_o->SetFillStyle(1001);
  p_m_o->SetMaximum(maxt);
  p_m_o->SetMinimum(mint);
  p_m_o->GetXaxis()->SetTitleSize(0.08);
  p_m_o->GetXaxis()->SetLabelSize(0.06);
  p_m_o->GetXaxis()->SetTitleOffset(0.95);
  p_m_o->GetYaxis()->SetNdivisions(204);
  p_m_o->GetYaxis()->SetTitleSize(0.05);
  p_m_o->GetYaxis()->SetLabelSize(0.05);
  p_m_o->GetYaxis()->SetTitleOffset(0.95);
  p_m_o->GetYaxis()->SetNdivisions(508);  
  
  p_p_n->SetLineColor(kAzure-4);
  p_p_n->SetLineWidth(2);
  p_p_n->SetMarkerColor(kAzure-2);
  p_p_n->SetMarkerStyle(20);
  p_p_n->SetFillColor(38);
  p_p_n->SetFillStyle(3144);
  
  p_m_n->SetLineColor(kAzure-4);
  p_m_n->SetLineWidth(2);
  p_m_n->SetMarkerColor(kAzure-2);
  p_m_n->SetMarkerStyle(20);
  p_m_n->SetFillColor(38);
  p_m_n->SetFillStyle(3144);
 
  TLegend *l = new TLegend(.3,.77,.7,.85);
  l->SetNColumns(2);
  l->AddEntry(p_p_o, Form("%s Map",oldMapName.c_str()), "fp");
  l->AddEntry(p_p_n, Form("%s Map",newMapName.c_str()), "fp");
 
  c->cd();
  TPad plusPad("plusPad", "plusPad", 0, 0.55, 1, 1.0);
  plusPad.SetBottomMargin(0);
  plusPad.SetTickx();
  plusPad.SetTicky();
  plusPad.SetGridy();
  plusPad.Draw();
  plusPad.cd();
  p_p_o->Draw("e2 9");
  p_p_n->Draw("e2 9 same");
  l->Draw();
  plusPad.RedrawAxis();

  c->cd();
  TPad minPad("minPad", "minPad", 0, 0.0, 1, 0.55);
  minPad.SetTopMargin(0);
  minPad.SetBottomMargin(0.2);
  minPad.SetTickx();
  minPad.SetTicky();
  minPad.SetGridy();
  minPad.Draw();
  minPad.cd();
  p_m_o->Draw("e2 9");
  p_m_n->Draw("e2 9 same");
  minPad.RedrawAxis();
  c->cd();

  newMapName.erase (std::remove (newMapName.begin(), newMapName.end(), ' '), newMapName.end());
  oldMapName.erase (std::remove (oldMapName.begin(), oldMapName.end(), ' '), oldMapName.end());
 
  if (print) c->Print(Form("plots/%s%s_%s_ChargeSeparatedProfile_%s_vs_%s.png", file_prefix.c_str(), name.c_str(), type.c_str(), oldMapName.c_str(), newMapName.c_str()));

  delete l;
}

/**************************************************************************************************/

void MakeProfilePlots(bool print, string file_prefix, string oldFileName, string newFileName) {
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetPadBorderSize(2);
  // canvas for ratio plots
  TCanvas *ratioCan = new TCanvas("ratioCan","",1000,1000);

  const int nBins = 50;

  // invariant mass histograms for 3 track types and 2 files
  TH1F* h_m_CB_o = new TH1F("h_m_CB_o","",nBins,55,110);
  h_m_CB_o->SetTitle(";m_{#mu#bar{#mu}} (GeV);Events");
  TH1F* h_m_ME_o = (TH1F*)h_m_CB_o->Clone("h_m_ME_o");
  TH1F* h_m_MS_o = (TH1F*)h_m_CB_o->Clone("h_m_MS_o");
  TH1F* h_m_ID_o = (TH1F*)h_m_CB_o->Clone("h_m_ID_o");
  
  TH1F* h_m_CB_n = (TH1F*)h_m_CB_o->Clone("h_m_CB_n");
  TH1F* h_m_ME_n = (TH1F*)h_m_CB_o->Clone("h_m_ME_n");
  TH1F* h_m_MS_n = (TH1F*)h_m_CB_o->Clone("h_m_MS_n");
  TH1F* h_m_ID_n = (TH1F*)h_m_CB_o->Clone("h_m_ID_n");

  // pt histograms for 3 track types and 2 files
  TH1F* h_pt_CB_o = new TH1F("h_pt_CB_o","",nBins,0,120);
  h_pt_CB_o->SetTitle(";p_{T} (GeV);Events");
  TH1F* h_pt_ME_o = (TH1F*)h_pt_CB_o->Clone("h_pt_ME_o");
  TH1F* h_pt_MS_o = (TH1F*)h_pt_CB_o->Clone("h_pt_MS_o");
  TH1F* h_pt_ID_o = (TH1F*)h_pt_CB_o->Clone("h_pt_ID_o");

  TH1F* h_pt_CB_n = (TH1F*)h_pt_CB_o->Clone("h_pt_CB_n");
  TH1F* h_pt_ME_n = (TH1F*)h_pt_CB_o->Clone("h_pt_ME_n");
  TH1F* h_pt_MS_n = (TH1F*)h_pt_CB_o->Clone("h_pt_MS_n");
  TH1F* h_pt_ID_n = (TH1F*)h_pt_CB_o->Clone("h_pt_ID_n");
 
  // eta histograms for 3 track types and 2 files
  TH1F* h_eta_CB_o = new TH1F("h_eta_CB_o","",nBins,-2.7,2.7);
  h_eta_CB_o->SetTitle(";#eta;Events");
  TH1F* h_eta_ME_o = (TH1F*)h_eta_CB_o->Clone("h_eta_ME_o");
  TH1F* h_eta_MS_o = (TH1F*)h_eta_CB_o->Clone("h_eta_MS_o");
  TH1F* h_eta_ID_o = (TH1F*)h_eta_CB_o->Clone("h_eta_ID_o");

  TH1F* h_eta_CB_n = (TH1F*)h_eta_CB_o->Clone("h_eta_CB_n");
  TH1F* h_eta_ME_n = (TH1F*)h_eta_CB_o->Clone("h_eta_ME_n");
  TH1F* h_eta_MS_n = (TH1F*)h_eta_CB_o->Clone("h_eta_MS_n");
  TH1F* h_eta_ID_n = (TH1F*)h_eta_CB_o->Clone("h_eta_ID_n");
 
  // phi histograms for 3 track types and 2 files
  TH1F* h_phi_CB_o = new TH1F("h_phi_CB_o","",nBins,-3.15,3.15);
  h_phi_CB_o->SetTitle(";#phi;Events");
  TH1F* h_phi_ME_o = (TH1F*)h_phi_CB_o->Clone("h_phi_ME_o");
  TH1F* h_phi_MS_o = (TH1F*)h_phi_CB_o->Clone("h_phi_MS_o");
  TH1F* h_phi_ID_o = (TH1F*)h_phi_CB_o->Clone("h_phi_ID_o");

  TH1F* h_phi_CB_n = (TH1F*)h_phi_CB_o->Clone("h_phi_CB_n");
  TH1F* h_phi_ME_n = (TH1F*)h_phi_CB_o->Clone("h_phi_ME_n");
  TH1F* h_phi_MS_n = (TH1F*)h_phi_CB_o->Clone("h_phi_MS_n");
  TH1F* h_phi_ID_n = (TH1F*)h_phi_CB_o->Clone("h_phi_ID_n");

  // z0 histograms for 3 track types and 2 files
  TH1F* h_z0_CB_o = new TH1F("h_z0_CB_o","",nBins,-2,2);
  h_z0_CB_o->SetTitle(";z_{0} (mm);Events");
  TH1F* h_z0_ME_o = (TH1F*)h_z0_CB_o->Clone("h_z0_ME_o");
  TH1F* h_z0_MS_o = (TH1F*)h_z0_CB_o->Clone("h_z0_MS_o");
  TH1F* h_z0_ID_o = (TH1F*)h_z0_CB_o->Clone("h_z0_ID_o");

  TH1F* h_z0_CB_n = (TH1F*)h_z0_CB_o->Clone("h_z0_CB_n");
  TH1F* h_z0_ME_n = (TH1F*)h_z0_CB_o->Clone("h_z0_ME_n");
  TH1F* h_z0_MS_n = (TH1F*)h_z0_CB_o->Clone("h_z0_MS_n");
  TH1F* h_z0_ID_n = (TH1F*)h_z0_CB_o->Clone("h_z0_ID_n");

  // d0sig histograms for 3 track types and 2 files
  TH1F* h_d0sig_CB_o = new TH1F("h_d0sig_CB_o","",nBins,-5,5);
  h_d0sig_CB_o->SetTitle(";d_{0} Significance;Events");
  TH1F* h_d0sig_ME_o = (TH1F*)h_d0sig_CB_o->Clone("h_d0sig_ME_o");
  TH1F* h_d0sig_MS_o = (TH1F*)h_d0sig_CB_o->Clone("h_d0sig_MS_o");
  TH1F* h_d0sig_ID_o = (TH1F*)h_d0sig_CB_o->Clone("h_d0sig_ID_o");

  TH1F* h_d0sig_CB_n = (TH1F*)h_d0sig_CB_o->Clone("h_d0sig_CB_n");
  TH1F* h_d0sig_ME_n = (TH1F*)h_d0sig_CB_o->Clone("h_d0sig_ME_n");
  TH1F* h_d0sig_MS_n = (TH1F*)h_d0sig_CB_o->Clone("h_d0sig_MS_n");
  TH1F* h_d0sig_ID_n = (TH1F*)h_d0sig_CB_o->Clone("h_d0sig_ID_n");

  // energy histograms for 3 track types and 2 files
  TH1F* h_e_CB_o = new TH1F("h_e_CB_o","",nBins,0,400);
  h_e_CB_o->SetTitle(";E (GeV);Events");
  TH1F* h_e_ME_o = (TH1F*)h_e_CB_o->Clone("h_e_ME_o");
  TH1F* h_e_MS_o = (TH1F*)h_e_CB_o->Clone("h_e_MS_o");
  TH1F* h_e_ID_o = (TH1F*)h_e_CB_o->Clone("h_e_ID_o");

  TH1F* h_e_CB_n = (TH1F*)h_e_CB_o->Clone("h_e_CB_n");
  TH1F* h_e_ME_n = (TH1F*)h_e_CB_o->Clone("h_e_ME_n");
  TH1F* h_e_MS_n = (TH1F*)h_e_CB_o->Clone("h_e_MS_n");
  TH1F* h_e_ID_n = (TH1F*)h_e_CB_o->Clone("h_e_ID_n");

  // p_z of Z 
  TH1F* h_pzmumu_CB_o = new TH1F("h_pzmumu_CB_o","",nBins,-500,500);
  h_pzmumu_CB_o->SetTitle(";P_{z}^{#mu#mu} (GeV);Events");
  TH1F* h_pzmumu_ME_o = (TH1F*)h_pzmumu_CB_o->Clone("h_pzmumu_ME_o");
  TH1F* h_pzmumu_MS_o = (TH1F*)h_pzmumu_CB_o->Clone("h_pzmumu_MS_o");
  TH1F* h_pzmumu_ID_o = (TH1F*)h_pzmumu_CB_o->Clone("h_pzmumu_ID_o");

  TH1F* h_pzmumu_CB_n = (TH1F*)h_pzmumu_CB_o->Clone("h_pzmumu_CB_n");
  TH1F* h_pzmumu_ME_n = (TH1F*)h_pzmumu_CB_o->Clone("h_pzmumu_ME_n");
  TH1F* h_pzmumu_MS_n = (TH1F*)h_pzmumu_CB_o->Clone("h_pzmumu_MS_n");
  TH1F* h_pzmumu_ID_n = (TH1F*)h_pzmumu_CB_o->Clone("h_pzmumu_ID_n");

  // add histograms to vectors for formatting
  std::vector<TH1F*> *vh_o = new std::vector<TH1F*>();
  std::vector<TH1F*> *vh_n = new std::vector<TH1F*>();
  
  // old histograms
  vh_o->push_back(h_m_CB_o);
  vh_o->push_back(h_m_ME_o);
  vh_o->push_back(h_m_MS_o);
  vh_o->push_back(h_m_ID_o);
  vh_o->push_back(h_pt_CB_o);
  vh_o->push_back(h_pt_ME_o);
  vh_o->push_back(h_pt_MS_o);
  vh_o->push_back(h_pt_ID_o);
  vh_o->push_back(h_eta_CB_o);
  vh_o->push_back(h_eta_ME_o);
  vh_o->push_back(h_eta_MS_o);
  vh_o->push_back(h_eta_ID_o);
  vh_o->push_back(h_phi_CB_o);
  vh_o->push_back(h_phi_ME_o);
  vh_o->push_back(h_phi_MS_o);
  vh_o->push_back(h_phi_ID_o);
  vh_o->push_back(h_z0_CB_o);
  vh_o->push_back(h_z0_ME_o);
  vh_o->push_back(h_z0_MS_o);
  vh_o->push_back(h_z0_ID_o);
  vh_o->push_back(h_d0sig_CB_o);
  vh_o->push_back(h_d0sig_ME_o);
  vh_o->push_back(h_d0sig_MS_o);
  vh_o->push_back(h_d0sig_ID_o);
  vh_o->push_back(h_e_CB_o);
  vh_o->push_back(h_e_ME_o);
  vh_o->push_back(h_e_MS_o);
  vh_o->push_back(h_e_ID_o);
  vh_o->push_back(h_pzmumu_CB_o);
  vh_o->push_back(h_pzmumu_ME_o);
  vh_o->push_back(h_pzmumu_MS_o);
  vh_o->push_back(h_pzmumu_ID_o);

  vh_n->push_back(h_m_CB_n);
  vh_n->push_back(h_m_ME_n);
  vh_n->push_back(h_m_MS_n);
  vh_n->push_back(h_m_ID_n);
  vh_n->push_back(h_pt_CB_n);
  vh_n->push_back(h_pt_ME_n);
  vh_n->push_back(h_pt_MS_n);
  vh_n->push_back(h_pt_ID_n);
  vh_n->push_back(h_eta_CB_n);
  vh_n->push_back(h_eta_ME_n);
  vh_n->push_back(h_eta_MS_n);
  vh_n->push_back(h_eta_ID_n);
  vh_n->push_back(h_phi_CB_n);
  vh_n->push_back(h_phi_ME_n);
  vh_n->push_back(h_phi_MS_n);
  vh_n->push_back(h_phi_ID_n);
  vh_n->push_back(h_z0_CB_n);
  vh_n->push_back(h_z0_ME_n);
  vh_n->push_back(h_z0_MS_n);
  vh_n->push_back(h_z0_ID_n);
  vh_n->push_back(h_d0sig_CB_n);
  vh_n->push_back(h_d0sig_ME_n);
  vh_n->push_back(h_d0sig_MS_n);
  vh_n->push_back(h_d0sig_ID_n);
  vh_n->push_back(h_e_CB_n);
  vh_n->push_back(h_e_ME_n);
  vh_n->push_back(h_e_MS_n);
  vh_n->push_back(h_e_ID_n);
  vh_n->push_back(h_pzmumu_CB_n);
  vh_n->push_back(h_pzmumu_ME_n);
  vh_n->push_back(h_pzmumu_MS_n);
  vh_n->push_back(h_pzmumu_ID_n);
  
  
  // check that the vectors have the same size
  if ( vh_o->size() != vh_n->size()) {
    std::cout << "Warning: histogram vector sizes unequal between two maps" << std::endl;
    if (print) {
      std::cout << "Turning off printing" << std::endl;
      print = false;
    }    
  }

  // format histograms
  for (int i = 0; i < vh_o->size(); i++) {
    vh_o->at(i)->SetLineColor(kBlack); 
    vh_o->at(i)->SetLineWidth(2);
    vh_o->at(i)->SetFillStyle(1001);
    vh_o->at(i)->SetFillColor(18);

    vh_n->at(i)->SetLineColor(kAzure-2);
    vh_n->at(i)->SetLineWidth(2);
    vh_n->at(i)->SetFillStyle(3006);
    vh_n->at(i)->SetFillColor(38);
  }

  // create profiles and format
  TProfile* p_rho_plus_ME_o = new TProfile("p_rho_plus_ME_o","",nBins,-2.5,2.5,-0.5,0.5,"s");
  p_rho_plus_ME_o->SetTitle(";#eta_{ID};#rho_{ME} (#mu^{+})");
  TProfile* p_rho_min_ME_o = (TProfile*)p_rho_plus_ME_o->Clone("p_rho_min_ME_o");
  p_rho_min_ME_o->SetTitle(";#eta_{ID};#rho_{ME} (#mu^{-})");
  TProfile* p_rho_plus_MS_o = (TProfile*)p_rho_plus_ME_o->Clone("p_rho_plus_MS_o");
  p_rho_plus_MS_o->SetTitle(";#eta_{ID};#rho_{MS} (#mu^{+})");
  TProfile* p_rho_min_MS_o = (TProfile*)p_rho_plus_ME_o->Clone("p_rho_min_MS_o");
  p_rho_min_MS_o->SetTitle(";#eta_{ID};#rho_{MS} (#mu^{-})");

  TProfile* p_rho_plus_ME_n = (TProfile*)p_rho_plus_ME_o->Clone("p_rho_plus_ME_n");
  TProfile* p_rho_min_ME_n = (TProfile*)p_rho_plus_ME_o->Clone("p_rho_min_ME_n");
  TProfile* p_rho_plus_MS_n = (TProfile*)p_rho_plus_ME_o->Clone("p_rho_plus_MS_n");
  TProfile* p_rho_min_MS_n = (TProfile*)p_rho_plus_ME_o->Clone("p_rho_min_MS_n");
  
  // load old file and set up reader
  TFile* f_recoOld = new TFile(Form("ntuples_muonSelection/%s", oldFileName.c_str()));
  string oldMapName = "Run I";

  TTreeReader reader_old("SelectedZDecayMuons", f_recoOld);
  TTreeReaderValue<vector<float>> pt_CB_o(reader_old, "Pt_CB");
  TTreeReaderValue<vector<float>> pt_ME_o(reader_old, "Pt_ME");
  TTreeReaderValue<vector<float>> pt_MS_o(reader_old, "Pt_MS");
  TTreeReaderValue<vector<float>> pt_ID_o(reader_old, "Pt_ID");
  TTreeReaderValue<vector<float>> eta_CB_o(reader_old, "Eta_CB");
  TTreeReaderValue<vector<float>> eta_ME_o(reader_old, "Eta_ME");
  TTreeReaderValue<vector<float>> eta_MS_o(reader_old, "Eta_MS");
  TTreeReaderValue<vector<float>> eta_ID_o(reader_old, "Eta_ID");
  TTreeReaderValue<vector<float>> phi_CB_o(reader_old, "Phi_CB");
  TTreeReaderValue<vector<float>> phi_ME_o(reader_old, "Phi_ME");
  TTreeReaderValue<vector<float>> phi_MS_o(reader_old, "Phi_MS");
  TTreeReaderValue<vector<float>> phi_ID_o(reader_old, "Phi_ID");
  TTreeReaderValue<vector<float>> d0sig_CB_o(reader_old, "d0sig_CB");
  TTreeReaderValue<vector<float>> d0sig_ME_o(reader_old, "d0sig_ME");
  TTreeReaderValue<vector<float>> d0sig_MS_o(reader_old, "d0sig_MS");
  TTreeReaderValue<vector<float>> d0sig_ID_o(reader_old, "d0sig_ID");
  TTreeReaderValue<vector<float>> z0_CB_o(reader_old, "z0_CB");
  TTreeReaderValue<vector<float>> z0_ME_o(reader_old, "z0_ME");
  TTreeReaderValue<vector<float>> z0_MS_o(reader_old, "z0_MS");
  TTreeReaderValue<vector<float>> z0_ID_o(reader_old, "z0_ID");
  TTreeReaderValue<float> m_CB_o(reader_old, "m_CB");
  TTreeReaderValue<float> m_ME_o(reader_old, "m_ME");
  TTreeReaderValue<float> m_MS_o(reader_old, "m_MS");
  TTreeReaderValue<float> m_ID_o(reader_old, "m_ID");
  TTreeReaderValue<vector<float>> e_CB_o(reader_old, "E_CB");
  TTreeReaderValue<vector<float>> e_ME_o(reader_old, "E_ME");
  TTreeReaderValue<vector<float>> e_MS_o(reader_old, "E_MS");
  TTreeReaderValue<vector<float>> e_ID_o(reader_old, "E_ID");
  TTreeReaderValue<vector<float>> charge_CB_o(reader_old, "Charge_CB");
  TTreeReaderValue<vector<float>> charge_ME_o(reader_old, "Charge_ME");
  TTreeReaderValue<vector<float>> charge_MS_o(reader_old, "Charge_MS");
  TTreeReaderValue<vector<float>> charge_ID_o(reader_old, "Charge_ID");
  TTreeReaderValue<vector<float>> eLoss_o(reader_old, "EnergyLoss");
  TTreeReaderValue<vector<int>> muonType_o(reader_old, "MuonType");
  TTreeReaderValue<vector<int>> quality_o(reader_old, "Quality");
  TTreeReaderValue<vector<int>> author_o(reader_old, "Author");
  
  // loop through TTreeReader
  while (reader_old.Next()) {
    // fill invariant mass and pz_mumu
    TLorentzVector v1, v2;
    if (*m_CB_o > 0) {
      h_m_CB_o->Fill(*m_CB_o);
      v1.SetPtEtaPhiM(pt_CB_o->at(0), eta_CB_o->at(0), phi_CB_o->at(0), .106);
      v2.SetPtEtaPhiM(pt_CB_o->at(1), eta_CB_o->at(1), phi_CB_o->at(1), .106); 
      h_pzmumu_CB_o->Fill(v1.Pz()+v2.Pz());
    }
    if (*m_ME_o > 0) {
      h_m_ME_o->Fill(*m_ME_o);
      v1.SetPtEtaPhiM(pt_ME_o->at(0), eta_ME_o->at(0), phi_ME_o->at(0), .106);
      v2.SetPtEtaPhiM(pt_ME_o->at(1), eta_ME_o->at(1), phi_ME_o->at(1), .106); 
      h_pzmumu_ME_o->Fill(v1.Pz()+v2.Pz());
    }
    if (*m_MS_o > 0) {
      h_m_MS_o->Fill(*m_MS_o);
      v1.SetPtEtaPhiM(pt_MS_o->at(0), eta_MS_o->at(0), phi_MS_o->at(0), .106);
      v2.SetPtEtaPhiM(pt_MS_o->at(1), eta_MS_o->at(1), phi_MS_o->at(1), .106); 
      h_pzmumu_MS_o->Fill(v1.Pz()+v2.Pz());
    }
    if (*m_ID_o > 0) {
      h_m_ID_o->Fill(*m_ID_o);
      v1.SetPtEtaPhiM(pt_ID_o->at(0), eta_ID_o->at(0), phi_ID_o->at(0), .106);
      v2.SetPtEtaPhiM(pt_ID_o->at(1), eta_ID_o->at(1), phi_ID_o->at(1), .106); 
      h_pzmumu_ID_o->Fill(v1.Pz()+v2.Pz());
    }
  
    // loop over two muons
    for (int i = 0; i < 2; i++) {
      // if CB muon
      if (pt_CB_o->at(i) != -1) {
        h_pt_CB_o->Fill(pt_CB_o->at(i));
        h_eta_CB_o->Fill(eta_CB_o->at(i));
        h_phi_CB_o->Fill(phi_CB_o->at(i));
        h_z0_CB_o->Fill(z0_CB_o->at(i));
        h_d0sig_CB_o->Fill(d0sig_CB_o->at(i));
        h_e_CB_o->Fill(e_CB_o->at(i));
      }
  
      // if ME muon
      if (pt_ME_o->at(i) != -1) {
        h_pt_ME_o->Fill(pt_ME_o->at(i));
        h_eta_ME_o->Fill(eta_ME_o->at(i));
        h_phi_ME_o->Fill(phi_ME_o->at(i));
        h_z0_ME_o->Fill(z0_ME_o->at(i));
        h_d0sig_ME_o->Fill(d0sig_ME_o->at(i));
        h_e_ME_o->Fill(e_ME_o->at(i));
      }

      // if MS muon
      if (pt_MS_o->at(i) != -1) {
        h_pt_MS_o->Fill(pt_MS_o->at(i));
        h_eta_MS_o->Fill(eta_MS_o->at(i));
        h_phi_MS_o->Fill(phi_MS_o->at(i));
        h_z0_MS_o->Fill(z0_MS_o->at(i));
        h_d0sig_MS_o->Fill(d0sig_MS_o->at(i));
        h_e_MS_o->Fill(e_MS_o->at(i));
      }

      // if ID muon
      if (pt_ID_o->at(i) != -1) {
        h_pt_ID_o->Fill(pt_ID_o->at(i));
        h_eta_ID_o->Fill(eta_ID_o->at(i));
        h_phi_ID_o->Fill(phi_ID_o->at(i));
        h_z0_ID_o->Fill(z0_ID_o->at(i));
        h_d0sig_ID_o->Fill(d0sig_ID_o->at(i));
        h_e_ID_o->Fill(e_ID_o->at(i));
        
        // fill rho ME
        if (pt_ME_o->at(i) != -1) {
          if (charge_ID_o->at(i) == 1)
            p_rho_plus_ME_o->Fill(eta_ID_o->at(i), (pt_ME_o->at(i)-pt_ID_o->at(i))/pt_ID_o->at(i)); 
          if (charge_ID_o->at(i) == -1)
            p_rho_min_ME_o->Fill(eta_ID_o->at(i), (pt_ME_o->at(i)-pt_ID_o->at(i))/pt_ID_o->at(i)); 
        } 
        // fill rho MS
        if (pt_MS_o->at(i) != -1) {
          if (charge_ID_o->at(i) == 1)
            p_rho_plus_MS_o->Fill(eta_ID_o->at(i), (pt_MS_o->at(i)-pt_ID_o->at(i))/pt_ID_o->at(i)); 
          if (charge_ID_o->at(i) == -1)
            p_rho_min_MS_o->Fill(eta_ID_o->at(i), (pt_MS_o->at(i)-pt_ID_o->at(i))/pt_ID_o->at(i)); 
        } 
      }
    }
  }
  delete f_recoOld;

  // load new file and set up reader
  TFile* f_recoNew = new TFile(Form("ntuples_muonSelection/%s", newFileName.c_str()));
  string newMapName = "2016";
 
  TTreeReader reader_new("SelectedZDecayMuons", f_recoNew);
  TTreeReaderValue<vector<float>> pt_CB_n(reader_new, "Pt_CB");
  TTreeReaderValue<vector<float>> pt_ME_n(reader_new, "Pt_ME");
  TTreeReaderValue<vector<float>> pt_MS_n(reader_new, "Pt_MS");
  TTreeReaderValue<vector<float>> pt_ID_n(reader_new, "Pt_ID");
  TTreeReaderValue<vector<float>> eta_CB_n(reader_new, "Eta_CB");
  TTreeReaderValue<vector<float>> eta_ME_n(reader_new, "Eta_ME");
  TTreeReaderValue<vector<float>> eta_MS_n(reader_new, "Eta_MS");
  TTreeReaderValue<vector<float>> eta_ID_n(reader_new, "Eta_ID");
  TTreeReaderValue<vector<float>> phi_CB_n(reader_new, "Phi_CB");
  TTreeReaderValue<vector<float>> phi_ME_n(reader_new, "Phi_ME");
  TTreeReaderValue<vector<float>> phi_MS_n(reader_new, "Phi_MS");
  TTreeReaderValue<vector<float>> phi_ID_n(reader_new, "Phi_ID");
  TTreeReaderValue<vector<float>> d0sig_CB_n(reader_new, "d0sig_CB");
  TTreeReaderValue<vector<float>> d0sig_ME_n(reader_new, "d0sig_ME");
  TTreeReaderValue<vector<float>> d0sig_MS_n(reader_new, "d0sig_MS");
  TTreeReaderValue<vector<float>> d0sig_ID_n(reader_new, "d0sig_ID");
  TTreeReaderValue<vector<float>> z0_CB_n(reader_new, "z0_CB");
  TTreeReaderValue<vector<float>> z0_ME_n(reader_new, "z0_ME");
  TTreeReaderValue<vector<float>> z0_MS_n(reader_new, "z0_MS");
  TTreeReaderValue<vector<float>> z0_ID_n(reader_new, "z0_ID");
  TTreeReaderValue<float> m_CB_n(reader_new, "m_CB");
  TTreeReaderValue<float> m_ME_n(reader_new, "m_ME");
  TTreeReaderValue<float> m_MS_n(reader_new, "m_MS");
  TTreeReaderValue<float> m_ID_n(reader_new, "m_ID");
  TTreeReaderValue<vector<float>> e_CB_n(reader_new, "E_CB");
  TTreeReaderValue<vector<float>> e_ME_n(reader_new, "E_ME");
  TTreeReaderValue<vector<float>> e_MS_n(reader_new, "E_MS");
  TTreeReaderValue<vector<float>> e_ID_n(reader_new, "E_ID");
  TTreeReaderValue<vector<float>> charge_CB_n(reader_new, "Charge_CB");
  TTreeReaderValue<vector<float>> charge_ME_n(reader_new, "Charge_ME");
  TTreeReaderValue<vector<float>> charge_MS_n(reader_new, "Charge_MS");
  TTreeReaderValue<vector<float>> charge_ID_n(reader_new, "Charge_ID");
  TTreeReaderValue<vector<float>> eLoss_n(reader_new, "EnergyLoss");
  TTreeReaderValue<vector<int>> muonType_n(reader_new, "MuonType");
  TTreeReaderValue<vector<int>> quality_n(reader_new, "Quality");
  TTreeReaderValue<vector<int>> author_n(reader_new, "Author");
  
  // loop through TTreeReader
  while (reader_new.Next()) {
    // fill invariant mass and pz_mumu
    TLorentzVector v1, v2;
    if (*m_CB_n > 0) {
      h_m_CB_n->Fill(*m_CB_n);
      v1.SetPtEtaPhiM(pt_CB_n->at(0), eta_CB_n->at(0), phi_CB_n->at(0), .106);
      v2.SetPtEtaPhiM(pt_CB_n->at(1), eta_CB_n->at(1), phi_CB_n->at(1), .106); 
      h_pzmumu_CB_n->Fill(v1.Pz()+v2.Pz());
    }
    if (*m_ME_n > 0) {
      h_m_ME_n->Fill(*m_ME_n);
      v1.SetPtEtaPhiM(pt_ME_n->at(0), eta_ME_n->at(0), phi_ME_n->at(0), .106);
      v2.SetPtEtaPhiM(pt_ME_n->at(1), eta_ME_n->at(1), phi_ME_n->at(1), .106); 
      h_pzmumu_ME_n->Fill(v1.Pz()+v2.Pz());
    }
    if (*m_MS_n > 0) {
      h_m_MS_n->Fill(*m_MS_n);
      v1.SetPtEtaPhiM(pt_MS_n->at(0), eta_MS_n->at(0), phi_MS_n->at(0), .106);
      v2.SetPtEtaPhiM(pt_MS_n->at(1), eta_MS_n->at(1), phi_MS_n->at(1), .106); 
      h_pzmumu_MS_n->Fill(v1.Pz()+v2.Pz());
    }
    if (*m_ID_n > 0) {
      h_m_ID_n->Fill(*m_ID_n);
      v1.SetPtEtaPhiM(pt_ID_n->at(0), eta_ID_n->at(0), phi_ID_n->at(0), .106);
      v2.SetPtEtaPhiM(pt_ID_n->at(1), eta_ID_n->at(1), phi_ID_n->at(1), .106); 
      h_pzmumu_ID_n->Fill(v1.Pz()+v2.Pz());
    }
  
    // loop over two muons
    for (int i = 0; i < 2; i++) {
      // if CB muon
      if (pt_CB_n->at(i) != -1) {
        h_pt_CB_n->Fill(pt_CB_n->at(i));
        h_eta_CB_n->Fill(eta_CB_n->at(i));
        h_phi_CB_n->Fill(phi_CB_n->at(i));
        h_z0_CB_n->Fill(z0_CB_n->at(i));
        h_d0sig_CB_n->Fill(d0sig_CB_n->at(i));
        h_e_CB_n->Fill(e_CB_n->at(i));
      }
  
      // if ME muon
      if (pt_ME_n->at(i) != -1) {
        h_pt_ME_n->Fill(pt_ME_n->at(i));
        h_eta_ME_n->Fill(eta_ME_n->at(i));
        h_phi_ME_n->Fill(phi_ME_n->at(i));
        h_z0_ME_n->Fill(z0_ME_n->at(i));
        h_d0sig_ME_n->Fill(d0sig_ME_n->at(i));
        h_e_ME_n->Fill(e_ME_n->at(i));
      }

      // if MS muon
      if (pt_MS_n->at(i) != -1) {
        h_pt_MS_n->Fill(pt_MS_n->at(i));
        h_eta_MS_n->Fill(eta_MS_n->at(i));
        h_phi_MS_n->Fill(phi_MS_n->at(i));
        h_z0_MS_n->Fill(z0_MS_n->at(i));
        h_d0sig_MS_n->Fill(d0sig_MS_n->at(i));
        h_e_MS_n->Fill(e_MS_n->at(i));
      }

      // if ID muon
      if (pt_ID_n->at(i) != -1) {
        h_pt_ID_n->Fill(pt_ID_n->at(i));
        h_eta_ID_n->Fill(eta_ID_n->at(i));
        h_phi_ID_n->Fill(phi_ID_n->at(i));
        h_z0_ID_n->Fill(z0_ID_n->at(i));
        h_d0sig_ID_n->Fill(d0sig_ID_n->at(i));
        h_e_ID_n->Fill(e_ID_n->at(i));
        
        // fill rho ME
        if (pt_ME_n->at(i) != -1) {
          if (charge_ID_n->at(i) == 1)
            p_rho_plus_ME_n->Fill(eta_ID_n->at(i), (pt_ME_n->at(i)-pt_ID_n->at(i))/pt_ID_n->at(i)); 
          if (charge_ID_n->at(i) == -1)
            p_rho_min_ME_n->Fill(eta_ID_n->at(i), (pt_ME_n->at(i)-pt_ID_n->at(i))/pt_ID_n->at(i)); 
        } 
        // fill rho MS
        if (pt_MS_n->at(i) != -1) {
          if (charge_ID_n->at(i) == 1)
            p_rho_plus_MS_n->Fill(eta_ID_n->at(i), (pt_MS_n->at(i)-pt_ID_n->at(i))/pt_ID_n->at(i)); 
          if (charge_ID_n->at(i) == -1)
            p_rho_min_MS_n->Fill(eta_ID_n->at(i), (pt_MS_n->at(i)-pt_ID_n->at(i))/pt_ID_n->at(i)); 
        } 
      }
    }
  }
  delete f_recoNew;

  // plotting for invariant mass
  // gaussian fits
  h_m_CB_o->Fit("gaus","","", 80, 100);
  h_m_ME_o->Fit("gaus","","", 80, 100);
  h_m_MS_o->Fit("gaus","","", 80, 100);
  h_m_ID_o->Fit("gaus","","", 80, 100);
  h_m_CB_n->Fit("gaus","","", 80, 100);
  h_m_ME_n->Fit("gaus","","", 80, 100);
  h_m_MS_n->Fit("gaus","","", 80, 100);
  h_m_ID_n->Fit("gaus","","", 80, 100);

  TF1* f_m_CB_o = h_m_CB_o->GetFunction("gaus");
  TF1* f_m_ME_o = h_m_ME_o->GetFunction("gaus");
  TF1* f_m_MS_o = h_m_MS_o->GetFunction("gaus");
  TF1* f_m_ID_o = h_m_ID_o->GetFunction("gaus");
  TF1* f_m_CB_n = h_m_CB_n->GetFunction("gaus");
  TF1* f_m_ME_n = h_m_ME_n->GetFunction("gaus");
  TF1* f_m_MS_n = h_m_MS_n->GetFunction("gaus");
  TF1* f_m_ID_n = h_m_ID_n->GetFunction("gaus");
  
  // top left legend
  TLegend *l_tl;
  // top left labels
  TPaveText *t_tl_CB, *t_tl_ME, *t_tl_MS, *t_tl_ID;

  // top right legend
  TLegend *l_tr;
  // top right labels
  TPaveText *t_tr_CB, *t_tr_ME, *t_tr_MS, *t_tr_ID;
  
  // top center labels
  TPaveText *t_tc_CB, *t_tc_ME, *t_tc_MS, *t_tc_ID;

  // top left legend
  l_tl = new TLegend(.12,.76,.5,.88);
  l_tl->AddEntry(h_m_CB_o, Form("%s Map",oldMapName.c_str()), "lf");
  l_tl->AddEntry(h_m_CB_n, Form("%s Map",newMapName.c_str()), "lf");

  // top right legend
  l_tr = new TLegend(.66,.76,.89,.88);
  l_tr->AddEntry(h_m_CB_o, Form("%s Map",oldMapName.c_str()), "lf");
  l_tr->AddEntry(h_m_CB_n, Form("%s Map",newMapName.c_str()), "lf");

  // top left labels
  t_tl_CB = new TPaveText(.12,.38,.45,.73,"NDC");
  t_tl_CB->AddText("#it{ATLAS} #bf{Preliminary}");
  t_tl_CB->SetFillColor(0);
  t_tl_CB->SetLineColor(0);
  t_tl_CB->SetBorderSize(0);
  t_tl_CB->SetTextAlign(10);
  t_tl_ME = new TPaveText(*t_tl_CB);
  t_tl_MS = new TPaveText(*t_tl_CB);
  t_tl_ID = new TPaveText(*t_tl_CB);
  
  t_tl_CB->AddText("#bf{Combined Muons}");
  t_tl_CB->AddText("");
  t_tl_CB->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_CB_o->GetParameter(1),f_m_CB_o->GetParError(1)));
  t_tl_CB->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_CB_o->GetParameter(2),f_m_CB_o->GetParError(2)));
  t_tl_CB->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_CB_n->GetParameter(1),f_m_CB_n->GetParError(1)));
  t_tl_CB->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_CB_n->GetParameter(2),f_m_CB_n->GetParError(2)));
  
  t_tl_ME->AddText("#bf{MS Extrapolated Muons}");
  t_tl_ME->AddText("");
  t_tl_ME->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_ME_o->GetParameter(1),f_m_ME_o->GetParError(1)));
  t_tl_ME->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_ME_o->GetParameter(2),f_m_ME_o->GetParError(2)));
  t_tl_ME->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_ME_n->GetParameter(1),f_m_ME_n->GetParError(1)));
  t_tl_ME->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_ME_n->GetParameter(2),f_m_ME_n->GetParError(2)));
  
  t_tl_MS->AddText("#bf{MS Only Muons}");
  t_tl_MS->AddText("");
  t_tl_MS->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_MS_o->GetParameter(1),f_m_MS_o->GetParError(1)));
  t_tl_MS->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_MS_o->GetParameter(2),f_m_MS_o->GetParError(2)));
  t_tl_MS->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_MS_n->GetParameter(1),f_m_MS_n->GetParError(1)));
  t_tl_MS->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_MS_n->GetParameter(2),f_m_MS_n->GetParError(2)));
  
  t_tl_ID->AddText("#bf{ID Muons}");
  t_tl_ID->AddText("");
  t_tl_ID->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_ID_o->GetParameter(1),f_m_ID_o->GetParError(1)));
  t_tl_ID->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_ID_o->GetParameter(2),f_m_ID_o->GetParError(2)));
  t_tl_ID->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_ID_n->GetParameter(1),f_m_ID_n->GetParError(1)));
  t_tl_ID->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_ID_n->GetParameter(2),f_m_ID_n->GetParError(2)));
  
  // top center labels
  t_tc_CB = new TPaveText(.39,.76,.67,.85,"NDC");
  t_tc_CB->AddText("#it{ATLAS} #bf{Preliminary}");
  t_tc_CB->SetFillColor(0);
  t_tc_CB->SetLineColor(0);
  t_tc_CB->SetBorderSize(0);
  t_tc_CB->SetTextAlign(30);
  t_tc_ME = new TPaveText(*t_tc_CB);
  t_tc_MS = new TPaveText(*t_tc_CB);
  t_tc_ID = new TPaveText(*t_tc_CB);
  t_tc_CB->AddText("#bf{Combined Muons}");
  t_tc_ME->AddText("#bf{MS Extrapolated Muons}");
  t_tc_MS->AddText("#bf{MS Only Muons}");
  t_tc_ID->AddText("#bf{ID Muons}");
  
  // top right labels
  t_tr_CB = new TPaveText(.59,.65,.87,.74,"NDC");
  t_tr_CB->AddText("#it{ATLAS} #bf{Preliminary}");
  t_tr_CB->SetFillColor(0);
  t_tr_CB->SetLineColor(0);
  t_tr_CB->SetBorderSize(0);
  t_tr_CB->SetTextAlign(30);
  t_tr_ME = new TPaveText(*t_tr_CB);
  t_tr_MS = new TPaveText(*t_tr_CB);
  t_tr_ID = new TPaveText(*t_tr_CB);
  t_tr_CB->AddText("#bf{Combined Muons}");
  t_tr_ME->AddText("#bf{MS Extrapolated Muons}");
  t_tr_MS->AddText("#bf{MS Only Muons}");
  t_tr_ID->AddText("#bf{ID Muons}");
  
  // vector of legends
  std::vector<TLegend*> *vl = new std::vector<TLegend*>();
  // m
  vl->push_back(l_tl);
  vl->push_back(l_tl);
  vl->push_back(l_tl);
  vl->push_back(l_tl);
  // pt
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  // eta
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  // phi
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  // z0
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  // dosig
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  // e
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  // pzmumu
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  
  vl->push_back(l_tr);  

  // vector of tpavetext labels
  std::vector<TPaveText*> *vt = new std::vector<TPaveText*>();
  // m
  vt->push_back(t_tl_CB);
  vt->push_back(t_tl_ME);
  vt->push_back(t_tl_MS);
  vt->push_back(t_tl_ID);
  // pt
  vt->push_back(t_tr_CB);  
  vt->push_back(t_tr_ME);  
  vt->push_back(t_tr_MS);  
  vt->push_back(t_tr_ID);  
  // eta
  vt->push_back(t_tc_CB);  
  vt->push_back(t_tc_ME);  
  vt->push_back(t_tc_MS);  
  vt->push_back(t_tc_ID);  
  // phi
  vt->push_back(t_tc_CB);  
  vt->push_back(t_tc_ME);  
  vt->push_back(t_tc_MS);  
  vt->push_back(t_tc_ID);  
  // z0
  vt->push_back(t_tc_CB);  
  vt->push_back(t_tc_ME);  
  vt->push_back(t_tc_MS);  
  vt->push_back(t_tc_ID);  
  // dosig
  vt->push_back(t_tr_CB);  
  vt->push_back(t_tr_ME);  
  vt->push_back(t_tr_MS);  
  vt->push_back(t_tr_ID);  
  // e
  vt->push_back(t_tr_CB);  
  vt->push_back(t_tr_ME);  
  vt->push_back(t_tr_MS);  
  vt->push_back(t_tr_ID);  
  // pzmumu
  vt->push_back(t_tr_CB);  
  vt->push_back(t_tr_ME);  
  vt->push_back(t_tr_MS);  
  vt->push_back(t_tr_ID);  

  // currently plotting 8 variables 
  const int Nvar = 8;
  double maxFactor[Nvar] = {};
  maxFactor[0] = 1.05; // m
  maxFactor[1] = 1.05; // pt
  maxFactor[2] = 1.3; // eta
  maxFactor[3] = 1.3; // phi
  maxFactor[4] = 1.3; // z0
  maxFactor[5] = 1.05; // d0sig
  maxFactor[6] = 1.05; // e
  maxFactor[7] = 1.05; // pzmumu

  bool skip[Nvar] = {};
  
  // set histogram max values
  double max = 0, int_o = 0, int_n = 0, int_min = 0;
  for (int i = 0; i < vh_o->size(); i++) {
    int_o = vh_o->at(i)->Integral();
    int_n = vh_n->at(i)->Integral();
    int_min = min(int_n, int_o);
    if (int_o >0 and int_n > 0) {
      vh_o->at(i)->Scale(int_min/int_o);
      vh_n->at(i)->Scale(int_min/int_n);
    }
    else skip[i] = true;

    max = 0;
    max = std::max(vh_o->at(i)->GetMaximum(), vh_n->at(i)->GetMaximum());
    max *= maxFactor[(int)((double)i/4.)];

    vh_o->at(i)->SetMaximum(max);
    vh_n->at(i)->SetMaximum(max);
    vh_o->at(i)->SetMinimum(0);
    vh_n->at(i)->SetMinimum(0);
  }

 
  // plotting
  ratioCan->cd();
  ratioCan->Clear();
  for (int i = 0; i < vh_o->size(); i++)
    if (!skip[(int)((double)i/4.)]) {
      MakeRatioPlot(ratioCan, vh_o->at(i), vh_n->at(i), vl->at(i), vt->at(i), 
                    oldMapName, newMapName, print, file_prefix);
    }
  ratioCan->Close();
  
  // canvas for raw
  TCanvas *profileCan = new TCanvas("profileCan","",1000,1000);
  MakeChargeSeparatedProfile(profileCan, p_rho_plus_ME_o, p_rho_min_ME_o, p_rho_plus_ME_n, p_rho_min_ME_n,
                 oldMapName, newMapName, print, file_prefix);
  MakeChargeSeparatedProfile(profileCan, p_rho_plus_MS_o, p_rho_min_MS_o, p_rho_plus_MS_n, p_rho_min_MS_n,
                 oldMapName, newMapName, print, file_prefix);
  profileCan->Close();

  for (int i = 0; i < vh_o->size(); i++) {
    delete vh_o->at(i);
    delete vh_n->at(i);
  }
  delete vh_o;
  delete vh_n;
  delete p_rho_plus_ME_o;
  delete p_rho_min_ME_o;
  delete p_rho_plus_ME_n;
  delete p_rho_min_ME_n;
  delete p_rho_plus_MS_o;
  delete p_rho_min_MS_o;
  delete p_rho_plus_MS_n;
  delete p_rho_min_MS_n;
  delete l_tl;
  delete t_tl_CB; 
  delete t_tl_ME; 
  delete t_tl_MS; 
  delete t_tl_ID;
  delete t_tr_CB; 
  delete t_tr_ME; 
  delete t_tr_MS; 
  delete t_tr_ID;
  delete t_tc_CB; 
  delete t_tc_ME; 
  delete t_tc_MS; 
  delete t_tc_ID;
  delete vt;
  delete vl;
}
/**************************************************************************************************/

void MakeRatioPlots(bool print, string file_prefix, string oldFileName, string newFileName) {
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetPadBorderSize(2);
  // canvas for ratio plots
  TCanvas *ratioCan = new TCanvas("ratioCan","",1000,1000);

  const int nBins = 50;

  // invariant mass histograms for 3 track types and 2 files
  TH1F* h_m_CB_o = new TH1F("h_m_CB_o","",nBins,55,110);
  h_m_CB_o->SetTitle(";m_{#mu#bar{#mu}} (GeV);Events");
  TH1F* h_m_ME_o = (TH1F*)h_m_CB_o->Clone("h_m_ME_o");
  TH1F* h_m_MS_o = (TH1F*)h_m_CB_o->Clone("h_m_MS_o");
  TH1F* h_m_ID_o = (TH1F*)h_m_CB_o->Clone("h_m_ID_o");
  
  TH1F* h_m_CB_n = (TH1F*)h_m_CB_o->Clone("h_m_CB_n");
  TH1F* h_m_ME_n = (TH1F*)h_m_CB_o->Clone("h_m_ME_n");
  TH1F* h_m_MS_n = (TH1F*)h_m_CB_o->Clone("h_m_MS_n");
  TH1F* h_m_ID_n = (TH1F*)h_m_CB_o->Clone("h_m_ID_n");

  // pt histograms for 3 track types and 2 files
  TH1F* h_pt_CB_o = new TH1F("h_pt_CB_o","",nBins,0,120);
  h_pt_CB_o->SetTitle(";p_{T} (GeV);Events");
  TH1F* h_pt_ME_o = (TH1F*)h_pt_CB_o->Clone("h_pt_ME_o");
  TH1F* h_pt_MS_o = (TH1F*)h_pt_CB_o->Clone("h_pt_MS_o");
  TH1F* h_pt_ID_o = (TH1F*)h_pt_CB_o->Clone("h_pt_ID_o");

  TH1F* h_pt_CB_n = (TH1F*)h_pt_CB_o->Clone("h_pt_CB_n");
  TH1F* h_pt_ME_n = (TH1F*)h_pt_CB_o->Clone("h_pt_ME_n");
  TH1F* h_pt_MS_n = (TH1F*)h_pt_CB_o->Clone("h_pt_MS_n");
  TH1F* h_pt_ID_n = (TH1F*)h_pt_CB_o->Clone("h_pt_ID_n");
 
  // eta histograms for 3 track types and 2 files
  TH1F* h_eta_CB_o = new TH1F("h_eta_CB_o","",nBins,-2.7,2.7);
  h_eta_CB_o->SetTitle(";#eta;Events");
  TH1F* h_eta_ME_o = (TH1F*)h_eta_CB_o->Clone("h_eta_ME_o");
  TH1F* h_eta_MS_o = (TH1F*)h_eta_CB_o->Clone("h_eta_MS_o");
  TH1F* h_eta_ID_o = (TH1F*)h_eta_CB_o->Clone("h_eta_ID_o");

  TH1F* h_eta_CB_n = (TH1F*)h_eta_CB_o->Clone("h_eta_CB_n");
  TH1F* h_eta_ME_n = (TH1F*)h_eta_CB_o->Clone("h_eta_ME_n");
  TH1F* h_eta_MS_n = (TH1F*)h_eta_CB_o->Clone("h_eta_MS_n");
  TH1F* h_eta_ID_n = (TH1F*)h_eta_CB_o->Clone("h_eta_ID_n");
 
  // phi histograms for 3 track types and 2 files
  TH1F* h_phi_CB_o = new TH1F("h_phi_CB_o","",nBins,-3.15,3.15);
  h_phi_CB_o->SetTitle(";#phi;Events");
  TH1F* h_phi_ME_o = (TH1F*)h_phi_CB_o->Clone("h_phi_ME_o");
  TH1F* h_phi_MS_o = (TH1F*)h_phi_CB_o->Clone("h_phi_MS_o");
  TH1F* h_phi_ID_o = (TH1F*)h_phi_CB_o->Clone("h_phi_ID_o");

  TH1F* h_phi_CB_n = (TH1F*)h_phi_CB_o->Clone("h_phi_CB_n");
  TH1F* h_phi_ME_n = (TH1F*)h_phi_CB_o->Clone("h_phi_ME_n");
  TH1F* h_phi_MS_n = (TH1F*)h_phi_CB_o->Clone("h_phi_MS_n");
  TH1F* h_phi_ID_n = (TH1F*)h_phi_CB_o->Clone("h_phi_ID_n");

  // z0 histograms for 3 track types and 2 files
  TH1F* h_z0_CB_o = new TH1F("h_z0_CB_o","",nBins,-2,2);
  h_z0_CB_o->SetTitle(";z_{0} (mm);Events");
  TH1F* h_z0_ME_o = (TH1F*)h_z0_CB_o->Clone("h_z0_ME_o");
  TH1F* h_z0_MS_o = (TH1F*)h_z0_CB_o->Clone("h_z0_MS_o");
  TH1F* h_z0_ID_o = (TH1F*)h_z0_CB_o->Clone("h_z0_ID_o");

  TH1F* h_z0_CB_n = (TH1F*)h_z0_CB_o->Clone("h_z0_CB_n");
  TH1F* h_z0_ME_n = (TH1F*)h_z0_CB_o->Clone("h_z0_ME_n");
  TH1F* h_z0_MS_n = (TH1F*)h_z0_CB_o->Clone("h_z0_MS_n");
  TH1F* h_z0_ID_n = (TH1F*)h_z0_CB_o->Clone("h_z0_ID_n");

  // d0sig histograms for 3 track types and 2 files
  TH1F* h_d0sig_CB_o = new TH1F("h_d0sig_CB_o","",nBins,-5,5);
  h_d0sig_CB_o->SetTitle(";d_{0} Significance;Events");
  TH1F* h_d0sig_ME_o = (TH1F*)h_d0sig_CB_o->Clone("h_d0sig_ME_o");
  TH1F* h_d0sig_MS_o = (TH1F*)h_d0sig_CB_o->Clone("h_d0sig_MS_o");
  TH1F* h_d0sig_ID_o = (TH1F*)h_d0sig_CB_o->Clone("h_d0sig_ID_o");

  TH1F* h_d0sig_CB_n = (TH1F*)h_d0sig_CB_o->Clone("h_d0sig_CB_n");
  TH1F* h_d0sig_ME_n = (TH1F*)h_d0sig_CB_o->Clone("h_d0sig_ME_n");
  TH1F* h_d0sig_MS_n = (TH1F*)h_d0sig_CB_o->Clone("h_d0sig_MS_n");
  TH1F* h_d0sig_ID_n = (TH1F*)h_d0sig_CB_o->Clone("h_d0sig_ID_n");

  // energy histograms for 3 track types and 2 files
  TH1F* h_e_CB_o = new TH1F("h_e_CB_o","",nBins,0,400);
  h_e_CB_o->SetTitle(";E (GeV);Events");
  TH1F* h_e_ME_o = (TH1F*)h_e_CB_o->Clone("h_e_ME_o");
  TH1F* h_e_MS_o = (TH1F*)h_e_CB_o->Clone("h_e_MS_o");
  TH1F* h_e_ID_o = (TH1F*)h_e_CB_o->Clone("h_e_ID_o");

  TH1F* h_e_CB_n = (TH1F*)h_e_CB_o->Clone("h_e_CB_n");
  TH1F* h_e_ME_n = (TH1F*)h_e_CB_o->Clone("h_e_ME_n");
  TH1F* h_e_MS_n = (TH1F*)h_e_CB_o->Clone("h_e_MS_n");
  TH1F* h_e_ID_n = (TH1F*)h_e_CB_o->Clone("h_e_ID_n");

  // p_z of Z 
  TH1F* h_pzmumu_CB_o = new TH1F("h_pzmumu_CB_o","",nBins,-500,500);
  h_pzmumu_CB_o->SetTitle(";P_{z}^{#mu#mu} (GeV);Events");
  TH1F* h_pzmumu_ME_o = (TH1F*)h_pzmumu_CB_o->Clone("h_pzmumu_ME_o");
  TH1F* h_pzmumu_MS_o = (TH1F*)h_pzmumu_CB_o->Clone("h_pzmumu_MS_o");
  TH1F* h_pzmumu_ID_o = (TH1F*)h_pzmumu_CB_o->Clone("h_pzmumu_ID_o");

  TH1F* h_pzmumu_CB_n = (TH1F*)h_pzmumu_CB_o->Clone("h_pzmumu_CB_n");
  TH1F* h_pzmumu_ME_n = (TH1F*)h_pzmumu_CB_o->Clone("h_pzmumu_ME_n");
  TH1F* h_pzmumu_MS_n = (TH1F*)h_pzmumu_CB_o->Clone("h_pzmumu_MS_n");
  TH1F* h_pzmumu_ID_n = (TH1F*)h_pzmumu_CB_o->Clone("h_pzmumu_ID_n");
 
  // rho 
  TH1F* h_rho_ME_o = new TH1F("h_rho_ME_o","",nBins,-.35,.35);
  h_rho_ME_o->SetTitle(";#rho_{ME};Events");
  TH1F* h_rho_MS_o = (TH1F*)h_rho_ME_o->Clone("h_rho_MS_o");
  h_rho_MS_o->SetTitle(";#rho_{MS};Events");

  TH1F* h_rho_ME_n = (TH1F*)h_rho_ME_o->Clone("h_rho_ME_n");
  TH1F* h_rho_MS_n = (TH1F*)h_rho_ME_o->Clone("h_rho_MS_n");

  // add histograms to vectors for formatting
  std::vector<TH1F*> *vh_o = new std::vector<TH1F*>();
  std::vector<TH1F*> *vh_n = new std::vector<TH1F*>();
  
  // old histograms
  vh_o->push_back(h_m_CB_o);
  vh_o->push_back(h_m_ME_o);
  vh_o->push_back(h_m_MS_o);
  vh_o->push_back(h_m_ID_o);
  vh_o->push_back(h_pt_CB_o);
  vh_o->push_back(h_pt_ME_o);
  vh_o->push_back(h_pt_MS_o);
  vh_o->push_back(h_pt_ID_o);
  vh_o->push_back(h_eta_CB_o);
  vh_o->push_back(h_eta_ME_o);
  vh_o->push_back(h_eta_MS_o);
  vh_o->push_back(h_eta_ID_o);
  vh_o->push_back(h_phi_CB_o);
  vh_o->push_back(h_phi_ME_o);
  vh_o->push_back(h_phi_MS_o);
  vh_o->push_back(h_phi_ID_o);
  vh_o->push_back(h_z0_CB_o);
  vh_o->push_back(h_z0_ME_o);
  vh_o->push_back(h_z0_MS_o);
  vh_o->push_back(h_z0_ID_o);
  vh_o->push_back(h_d0sig_CB_o);
  vh_o->push_back(h_d0sig_ME_o);
  vh_o->push_back(h_d0sig_MS_o);
  vh_o->push_back(h_d0sig_ID_o);
  vh_o->push_back(h_e_CB_o);
  vh_o->push_back(h_e_ME_o);
  vh_o->push_back(h_e_MS_o);
  vh_o->push_back(h_e_ID_o);
  vh_o->push_back(h_pzmumu_CB_o);
  vh_o->push_back(h_pzmumu_ME_o);
  vh_o->push_back(h_pzmumu_MS_o);
  vh_o->push_back(h_pzmumu_ID_o);
  vh_o->push_back(h_rho_ME_o);
  vh_o->push_back(h_rho_MS_o);

  vh_n->push_back(h_m_CB_n);
  vh_n->push_back(h_m_ME_n);
  vh_n->push_back(h_m_MS_n);
  vh_n->push_back(h_m_ID_n);
  vh_n->push_back(h_pt_CB_n);
  vh_n->push_back(h_pt_ME_n);
  vh_n->push_back(h_pt_MS_n);
  vh_n->push_back(h_pt_ID_n);
  vh_n->push_back(h_eta_CB_n);
  vh_n->push_back(h_eta_ME_n);
  vh_n->push_back(h_eta_MS_n);
  vh_n->push_back(h_eta_ID_n);
  vh_n->push_back(h_phi_CB_n);
  vh_n->push_back(h_phi_ME_n);
  vh_n->push_back(h_phi_MS_n);
  vh_n->push_back(h_phi_ID_n);
  vh_n->push_back(h_z0_CB_n);
  vh_n->push_back(h_z0_ME_n);
  vh_n->push_back(h_z0_MS_n);
  vh_n->push_back(h_z0_ID_n);
  vh_n->push_back(h_d0sig_CB_n);
  vh_n->push_back(h_d0sig_ME_n);
  vh_n->push_back(h_d0sig_MS_n);
  vh_n->push_back(h_d0sig_ID_n);
  vh_n->push_back(h_e_CB_n);
  vh_n->push_back(h_e_ME_n);
  vh_n->push_back(h_e_MS_n);
  vh_n->push_back(h_e_ID_n);
  vh_n->push_back(h_pzmumu_CB_n);
  vh_n->push_back(h_pzmumu_ME_n);
  vh_n->push_back(h_pzmumu_MS_n);
  vh_n->push_back(h_pzmumu_ID_n);
  vh_n->push_back(h_rho_ME_n);
  vh_n->push_back(h_rho_MS_n);
  
  // check that the vectors have the same size
  if ( vh_o->size() != vh_n->size()) {
    std::cout << "Warning: histogram vector sizes unequal between two maps" << std::endl;
    if (print) {
      std::cout << "Turning off printing" << std::endl;
      print = false;
    }    
  }

  // format histograms
  for (int i = 0; i < vh_o->size(); i++) {
    vh_o->at(i)->SetLineColor(kBlack); 
    vh_o->at(i)->SetLineWidth(2);
    vh_o->at(i)->SetFillStyle(1001);
    vh_o->at(i)->SetFillColor(18);

    vh_n->at(i)->SetLineColor(kAzure-2);
    vh_n->at(i)->SetLineWidth(2);
    vh_n->at(i)->SetFillStyle(3006);
    vh_n->at(i)->SetFillColor(38);
  }

  // load old file and set up reader
  TFile* f_recoOld = new TFile(Form("ntuples_muonSelection/%s", oldFileName.c_str()));
  if (!f_recoOld) cout << "Warning: could not open file " << oldFileName << endl;
  string oldMapName = "Run I";

  TTreeReader reader_old("SelectedZDecayMuons", f_recoOld);
  TTreeReaderValue<vector<float>> pt_CB_o(reader_old, "Pt_CB");
  TTreeReaderValue<vector<float>> pt_ME_o(reader_old, "Pt_ME");
  TTreeReaderValue<vector<float>> pt_MS_o(reader_old, "Pt_MS");
  TTreeReaderValue<vector<float>> pt_ID_o(reader_old, "Pt_ID");
  TTreeReaderValue<vector<float>> eta_CB_o(reader_old, "Eta_CB");
  TTreeReaderValue<vector<float>> eta_ME_o(reader_old, "Eta_ME");
  TTreeReaderValue<vector<float>> eta_MS_o(reader_old, "Eta_MS");
  TTreeReaderValue<vector<float>> eta_ID_o(reader_old, "Eta_ID");
  TTreeReaderValue<vector<float>> phi_CB_o(reader_old, "Phi_CB");
  TTreeReaderValue<vector<float>> phi_ME_o(reader_old, "Phi_ME");
  TTreeReaderValue<vector<float>> phi_MS_o(reader_old, "Phi_MS");
  TTreeReaderValue<vector<float>> phi_ID_o(reader_old, "Phi_ID");
  TTreeReaderValue<vector<float>> d0sig_CB_o(reader_old, "d0sig_CB");
  TTreeReaderValue<vector<float>> d0sig_ME_o(reader_old, "d0sig_ME");
  TTreeReaderValue<vector<float>> d0sig_MS_o(reader_old, "d0sig_MS");
  TTreeReaderValue<vector<float>> d0sig_ID_o(reader_old, "d0sig_ID");
  TTreeReaderValue<vector<float>> z0_CB_o(reader_old, "z0_CB");
  TTreeReaderValue<vector<float>> z0_ME_o(reader_old, "z0_ME");
  TTreeReaderValue<vector<float>> z0_MS_o(reader_old, "z0_MS");
  TTreeReaderValue<vector<float>> z0_ID_o(reader_old, "z0_ID");
  TTreeReaderValue<float> m_CB_o(reader_old, "m_CB");
  TTreeReaderValue<float> m_ME_o(reader_old, "m_ME");
  TTreeReaderValue<float> m_MS_o(reader_old, "m_MS");
  TTreeReaderValue<float> m_ID_o(reader_old, "m_ID");
  TTreeReaderValue<vector<float>> e_CB_o(reader_old, "E_CB");
  TTreeReaderValue<vector<float>> e_ME_o(reader_old, "E_ME");
  TTreeReaderValue<vector<float>> e_MS_o(reader_old, "E_MS");
  TTreeReaderValue<vector<float>> e_ID_o(reader_old, "E_ID");
  TTreeReaderValue<vector<float>> charge_CB_o(reader_old, "Charge_CB");
  TTreeReaderValue<vector<float>> charge_ME_o(reader_old, "Charge_ME");
  TTreeReaderValue<vector<float>> charge_MS_o(reader_old, "Charge_MS");
  TTreeReaderValue<vector<float>> charge_ID_o(reader_old, "Charge_ID");
  TTreeReaderValue<vector<float>> eLoss_o(reader_old, "EnergyLoss");
  TTreeReaderValue<vector<int>> muonType_o(reader_old, "MuonType");
  TTreeReaderValue<vector<int>> quality_o(reader_old, "Quality");
  TTreeReaderValue<vector<int>> author_o(reader_old, "Author");
  
  // loop through TTreeReader
  while (reader_old.Next()) {
    // fill invariant mass and pz_mumu
    TLorentzVector v1, v2;
    if (*m_CB_o > 0) {
      h_m_CB_o->Fill(*m_CB_o);
      v1.SetPtEtaPhiM(pt_CB_o->at(0), eta_CB_o->at(0), phi_CB_o->at(0), .106);
      v2.SetPtEtaPhiM(pt_CB_o->at(1), eta_CB_o->at(1), phi_CB_o->at(1), .106); 
      h_pzmumu_CB_o->Fill(v1.Pz()+v2.Pz());
    }
    if (*m_ME_o > 0) {
      h_m_ME_o->Fill(*m_ME_o);
      v1.SetPtEtaPhiM(pt_ME_o->at(0), eta_ME_o->at(0), phi_ME_o->at(0), .106);
      v2.SetPtEtaPhiM(pt_ME_o->at(1), eta_ME_o->at(1), phi_ME_o->at(1), .106); 
      h_pzmumu_ME_o->Fill(v1.Pz()+v2.Pz());
    }
    if (*m_MS_o > 0) {
      h_m_MS_o->Fill(*m_MS_o);
      v1.SetPtEtaPhiM(pt_MS_o->at(0), eta_MS_o->at(0), phi_MS_o->at(0), .106);
      v2.SetPtEtaPhiM(pt_MS_o->at(1), eta_MS_o->at(1), phi_MS_o->at(1), .106); 
      h_pzmumu_MS_o->Fill(v1.Pz()+v2.Pz());
    }
    if (*m_ID_o > 0) {
      h_m_ID_o->Fill(*m_ID_o);
      v1.SetPtEtaPhiM(pt_ID_o->at(0), eta_ID_o->at(0), phi_ID_o->at(0), .106);
      v2.SetPtEtaPhiM(pt_ID_o->at(1), eta_ID_o->at(1), phi_ID_o->at(1), .106); 
      h_pzmumu_ID_o->Fill(v1.Pz()+v2.Pz());
    }
  
    // loop over two muons
    for (int i = 0; i < 2; i++) {
      // if CB muon
      if (pt_CB_o->at(i) != -1) {
        h_pt_CB_o->Fill(pt_CB_o->at(i));
        h_eta_CB_o->Fill(eta_CB_o->at(i));
        h_phi_CB_o->Fill(phi_CB_o->at(i));
        h_z0_CB_o->Fill(z0_CB_o->at(i));
        h_d0sig_CB_o->Fill(d0sig_CB_o->at(i));
        h_e_CB_o->Fill(e_CB_o->at(i));
      }
  
      // if ME muon
      if (pt_ME_o->at(i) != -1) {
        h_pt_ME_o->Fill(pt_ME_o->at(i));
        h_eta_ME_o->Fill(eta_ME_o->at(i));
        h_phi_ME_o->Fill(phi_ME_o->at(i));
        h_z0_ME_o->Fill(z0_ME_o->at(i));
        h_d0sig_ME_o->Fill(d0sig_ME_o->at(i));
        h_e_ME_o->Fill(e_ME_o->at(i));
      }

      // if MS muon
      if (pt_MS_o->at(i) != -1) {
        h_pt_MS_o->Fill(pt_MS_o->at(i));
        h_eta_MS_o->Fill(eta_MS_o->at(i));
        h_phi_MS_o->Fill(phi_MS_o->at(i));
        h_z0_MS_o->Fill(z0_MS_o->at(i));
        h_d0sig_MS_o->Fill(d0sig_MS_o->at(i));
        h_e_MS_o->Fill(e_MS_o->at(i));
      }

      // if ID muon
      if (pt_ID_o->at(i) != -1) {
        h_pt_ID_o->Fill(pt_ID_o->at(i));
        h_eta_ID_o->Fill(eta_ID_o->at(i));
        h_phi_ID_o->Fill(phi_ID_o->at(i));
        h_z0_ID_o->Fill(z0_ID_o->at(i));
        h_d0sig_ID_o->Fill(d0sig_ID_o->at(i));
        h_e_ID_o->Fill(e_ID_o->at(i));
        
        // fill rho ME
        if (pt_ME_o->at(i) != -1) 
          h_rho_ME_o->Fill((pt_ME_o->at(i)-pt_ID_o->at(i))/pt_ID_o->at(i)); 
        // fill rho MS
        if (pt_MS_o->at(i) != -1) 
          h_rho_MS_o->Fill((pt_MS_o->at(i)-pt_ID_o->at(i))/pt_ID_o->at(i)); 
      }
    }
  }
  delete f_recoOld;

  // load new file and set up reader
  TFile* f_recoNew = new TFile(Form("ntuples_muonSelection/%s", newFileName.c_str()));
  if (!f_recoNew) cout << "Warning: could not open file " << newFileName << endl;
  string newMapName = "2016";
 
  TTreeReader reader_new("SelectedZDecayMuons", f_recoNew);
  TTreeReaderValue<vector<float>> pt_CB_n(reader_new, "Pt_CB");
  TTreeReaderValue<vector<float>> pt_ME_n(reader_new, "Pt_ME");
  TTreeReaderValue<vector<float>> pt_MS_n(reader_new, "Pt_MS");
  TTreeReaderValue<vector<float>> pt_ID_n(reader_new, "Pt_ID");
  TTreeReaderValue<vector<float>> eta_CB_n(reader_new, "Eta_CB");
  TTreeReaderValue<vector<float>> eta_ME_n(reader_new, "Eta_ME");
  TTreeReaderValue<vector<float>> eta_MS_n(reader_new, "Eta_MS");
  TTreeReaderValue<vector<float>> eta_ID_n(reader_new, "Eta_ID");
  TTreeReaderValue<vector<float>> phi_CB_n(reader_new, "Phi_CB");
  TTreeReaderValue<vector<float>> phi_ME_n(reader_new, "Phi_ME");
  TTreeReaderValue<vector<float>> phi_MS_n(reader_new, "Phi_MS");
  TTreeReaderValue<vector<float>> phi_ID_n(reader_new, "Phi_ID");
  TTreeReaderValue<vector<float>> d0sig_CB_n(reader_new, "d0sig_CB");
  TTreeReaderValue<vector<float>> d0sig_ME_n(reader_new, "d0sig_ME");
  TTreeReaderValue<vector<float>> d0sig_MS_n(reader_new, "d0sig_MS");
  TTreeReaderValue<vector<float>> d0sig_ID_n(reader_new, "d0sig_ID");
  TTreeReaderValue<vector<float>> z0_CB_n(reader_new, "z0_CB");
  TTreeReaderValue<vector<float>> z0_ME_n(reader_new, "z0_ME");
  TTreeReaderValue<vector<float>> z0_MS_n(reader_new, "z0_MS");
  TTreeReaderValue<vector<float>> z0_ID_n(reader_new, "z0_ID");
  TTreeReaderValue<float> m_CB_n(reader_new, "m_CB");
  TTreeReaderValue<float> m_ME_n(reader_new, "m_ME");
  TTreeReaderValue<float> m_MS_n(reader_new, "m_MS");
  TTreeReaderValue<float> m_ID_n(reader_new, "m_ID");
  TTreeReaderValue<vector<float>> e_CB_n(reader_new, "E_CB");
  TTreeReaderValue<vector<float>> e_ME_n(reader_new, "E_ME");
  TTreeReaderValue<vector<float>> e_MS_n(reader_new, "E_MS");
  TTreeReaderValue<vector<float>> e_ID_n(reader_new, "E_ID");
  TTreeReaderValue<vector<float>> charge_CB_n(reader_new, "Charge_CB");
  TTreeReaderValue<vector<float>> charge_ME_n(reader_new, "Charge_ME");
  TTreeReaderValue<vector<float>> charge_MS_n(reader_new, "Charge_MS");
  TTreeReaderValue<vector<float>> charge_ID_n(reader_new, "Charge_ID");
  TTreeReaderValue<vector<float>> eLoss_n(reader_new, "EnergyLoss");
  TTreeReaderValue<vector<int>> muonType_n(reader_new, "MuonType");
  TTreeReaderValue<vector<int>> quality_n(reader_new, "Quality");
  TTreeReaderValue<vector<int>> author_n(reader_new, "Author");
  
  // loop through TTreeReader
  while (reader_new.Next()) {
    // fill invariant mass and pz_mumu
    TLorentzVector v1, v2;
    if (*m_CB_n > 0) {
      h_m_CB_n->Fill(*m_CB_n);
      v1.SetPtEtaPhiM(pt_CB_n->at(0), eta_CB_n->at(0), phi_CB_n->at(0), .106);
      v2.SetPtEtaPhiM(pt_CB_n->at(1), eta_CB_n->at(1), phi_CB_n->at(1), .106); 
      h_pzmumu_CB_n->Fill(v1.Pz()+v2.Pz());
    }
    if (*m_ME_n > 0) {
      h_m_ME_n->Fill(*m_ME_n);
      v1.SetPtEtaPhiM(pt_ME_n->at(0), eta_ME_n->at(0), phi_ME_n->at(0), .106);
      v2.SetPtEtaPhiM(pt_ME_n->at(1), eta_ME_n->at(1), phi_ME_n->at(1), .106); 
      h_pzmumu_ME_n->Fill(v1.Pz()+v2.Pz());
    }
    if (*m_MS_n > 0) {
      h_m_MS_n->Fill(*m_MS_n);
      v1.SetPtEtaPhiM(pt_MS_n->at(0), eta_MS_n->at(0), phi_MS_n->at(0), .106);
      v2.SetPtEtaPhiM(pt_MS_n->at(1), eta_MS_n->at(1), phi_MS_n->at(1), .106); 
      h_pzmumu_MS_n->Fill(v1.Pz()+v2.Pz());
    }
    if (*m_ID_n > 0) {
      h_m_ID_n->Fill(*m_ID_n);
      v1.SetPtEtaPhiM(pt_ID_n->at(0), eta_ID_n->at(0), phi_ID_n->at(0), .106);
      v2.SetPtEtaPhiM(pt_ID_n->at(1), eta_ID_n->at(1), phi_ID_n->at(1), .106); 
      h_pzmumu_ID_n->Fill(v1.Pz()+v2.Pz());
    }
  
    // loop over two muons
    for (int i = 0; i < 2; i++) {
      // if CB muon
      if (pt_CB_n->at(i) != -1) {
        h_pt_CB_n->Fill(pt_CB_n->at(i));
        h_eta_CB_n->Fill(eta_CB_n->at(i));
        h_phi_CB_n->Fill(phi_CB_n->at(i));
        h_z0_CB_n->Fill(z0_CB_n->at(i));
        h_d0sig_CB_n->Fill(d0sig_CB_n->at(i));
        h_e_CB_n->Fill(e_CB_n->at(i));
      }
  
      // if ME muon
      if (pt_ME_n->at(i) != -1) {
        h_pt_ME_n->Fill(pt_ME_n->at(i));
        h_eta_ME_n->Fill(eta_ME_n->at(i));
        h_phi_ME_n->Fill(phi_ME_n->at(i));
        h_z0_ME_n->Fill(z0_ME_n->at(i));
        h_d0sig_ME_n->Fill(d0sig_ME_n->at(i));
        h_e_ME_n->Fill(e_ME_n->at(i));
      }

      // if MS muon
      if (pt_MS_n->at(i) != -1) {
        h_pt_MS_n->Fill(pt_MS_n->at(i));
        h_eta_MS_n->Fill(eta_MS_n->at(i));
        h_phi_MS_n->Fill(phi_MS_n->at(i));
        h_z0_MS_n->Fill(z0_MS_n->at(i));
        h_d0sig_MS_n->Fill(d0sig_MS_n->at(i));
        h_e_MS_n->Fill(e_MS_n->at(i));
      }

      // if ID muon
      if (pt_ID_n->at(i) != -1) {
        h_pt_ID_n->Fill(pt_ID_n->at(i));
        h_eta_ID_n->Fill(eta_ID_n->at(i));
        h_phi_ID_n->Fill(phi_ID_n->at(i));
        h_z0_ID_n->Fill(z0_ID_n->at(i));
        h_d0sig_ID_n->Fill(d0sig_ID_n->at(i));
        h_e_ID_n->Fill(e_ID_n->at(i));
        
        // fill rho ME
        if (pt_ME_n->at(i) != -1) 
          h_rho_ME_n->Fill((pt_ME_n->at(i)-pt_ID_n->at(i))/pt_ID_n->at(i)); 
        // fill rho MS
        if (pt_MS_n->at(i) != -1) 
          h_rho_MS_n->Fill((pt_MS_n->at(i)-pt_ID_n->at(i))/pt_ID_n->at(i)); 
      }
    }
  }
  delete f_recoNew;

  // plotting for invariant mass
  // gaussian fits
  h_m_CB_o->Fit("gaus","","", 80, 100);
  h_m_ME_o->Fit("gaus","","", 80, 100);
  h_m_MS_o->Fit("gaus","","", 80, 100);
  h_m_ID_o->Fit("gaus","","", 80, 100);
  h_m_CB_n->Fit("gaus","","", 80, 100);
  h_m_ME_n->Fit("gaus","","", 80, 100);
  h_m_MS_n->Fit("gaus","","", 80, 100);
  h_m_ID_n->Fit("gaus","","", 80, 100);

  TF1* f_m_CB_o = h_m_CB_o->GetFunction("gaus");
  TF1* f_m_ME_o = h_m_ME_o->GetFunction("gaus");
  TF1* f_m_MS_o = h_m_MS_o->GetFunction("gaus");
  TF1* f_m_ID_o = h_m_ID_o->GetFunction("gaus");
  TF1* f_m_CB_n = h_m_CB_n->GetFunction("gaus");
  TF1* f_m_ME_n = h_m_ME_n->GetFunction("gaus");
  TF1* f_m_MS_n = h_m_MS_n->GetFunction("gaus");
  TF1* f_m_ID_n = h_m_ID_n->GetFunction("gaus");
  
  // top left legend
  TLegend *l_tl;
  // top left labels
  TPaveText *t_tl_CB, *t_tl_ME, *t_tl_MS, *t_tl_ID;

  // top right legend
  TLegend *l_tr;
  // top right labels
  TPaveText *t_tr_CB, *t_tr_ME, *t_tr_MS, *t_tr_ID;
  
  // top center labels
  TPaveText *t_tc_CB, *t_tc_ME, *t_tc_MS, *t_tc_ID;

  // top left legend
  l_tl = new TLegend(.12,.76,.5,.88);
  l_tl->AddEntry(h_m_CB_o, Form("%s Map",oldMapName.c_str()), "lf");
  l_tl->AddEntry(h_m_CB_n, Form("%s Map",newMapName.c_str()), "lf");

  // top right legend
  l_tr = new TLegend(.66,.76,.89,.88);
  l_tr->AddEntry(h_m_CB_o, Form("%s Map",oldMapName.c_str()), "lf");
  l_tr->AddEntry(h_m_CB_n, Form("%s Map",newMapName.c_str()), "lf");

  // top left labels
  t_tl_CB = new TPaveText(.12,.38,.45,.73,"NDC");
  t_tl_CB->AddText("#it{ATLAS} #bf{Preliminary}");
  t_tl_CB->SetFillColor(0);
  t_tl_CB->SetLineColor(0);
  t_tl_CB->SetBorderSize(0);
  t_tl_CB->SetTextAlign(10);
  t_tl_ME = new TPaveText(*t_tl_CB);
  t_tl_MS = new TPaveText(*t_tl_CB);
  t_tl_ID = new TPaveText(*t_tl_CB);
  
  t_tl_CB->AddText("#bf{Combined Muons}");
  t_tl_CB->AddText("");
  t_tl_CB->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_CB_o->GetParameter(1),f_m_CB_o->GetParError(1)));
  t_tl_CB->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_CB_o->GetParameter(2),f_m_CB_o->GetParError(2)));
  t_tl_CB->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_CB_n->GetParameter(1),f_m_CB_n->GetParError(1)));
  t_tl_CB->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_CB_n->GetParameter(2),f_m_CB_n->GetParError(2)));
  
  t_tl_ME->AddText("#bf{MS Extrapolated Muons}");
  t_tl_ME->AddText("");
  t_tl_ME->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_ME_o->GetParameter(1),f_m_ME_o->GetParError(1)));
  t_tl_ME->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_ME_o->GetParameter(2),f_m_ME_o->GetParError(2)));
  t_tl_ME->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_ME_n->GetParameter(1),f_m_ME_n->GetParError(1)));
  t_tl_ME->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_ME_n->GetParameter(2),f_m_ME_n->GetParError(2)));
  
  t_tl_MS->AddText("#bf{MS Only Muons}");
  t_tl_MS->AddText("");
  t_tl_MS->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_MS_o->GetParameter(1),f_m_MS_o->GetParError(1)));
  t_tl_MS->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_MS_o->GetParameter(2),f_m_MS_o->GetParError(2)));
  t_tl_MS->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_MS_n->GetParameter(1),f_m_MS_n->GetParError(1)));
  t_tl_MS->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_MS_n->GetParameter(2),f_m_MS_n->GetParError(2)));
  
  t_tl_ID->AddText("#bf{ID Muons}");
  t_tl_ID->AddText("");
  t_tl_ID->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_ID_o->GetParameter(1),f_m_ID_o->GetParError(1)));
  t_tl_ID->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_ID_o->GetParameter(2),f_m_ID_o->GetParError(2)));
  t_tl_ID->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_ID_n->GetParameter(1),f_m_ID_n->GetParError(1)));
  t_tl_ID->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_ID_n->GetParameter(2),f_m_ID_n->GetParError(2)));
  
  // top center labels
  t_tc_CB = new TPaveText(.39,.76,.67,.85,"NDC");
  t_tc_CB->AddText("#it{ATLAS} #bf{Preliminary}");
  t_tc_CB->SetFillColor(0);
  t_tc_CB->SetLineColor(0);
  t_tc_CB->SetBorderSize(0);
  t_tc_CB->SetTextAlign(30);
  t_tc_ME = new TPaveText(*t_tc_CB);
  t_tc_MS = new TPaveText(*t_tc_CB);
  t_tc_ID = new TPaveText(*t_tc_CB);
  t_tc_CB->AddText("#bf{Combined Muons}");
  t_tc_ME->AddText("#bf{MS Extrapolated Muons}");
  t_tc_MS->AddText("#bf{MS Only Muons}");
  t_tc_ID->AddText("#bf{ID Muons}");
  
  // top right labels
  t_tr_CB = new TPaveText(.59,.65,.87,.74,"NDC");
  t_tr_CB->AddText("#it{ATLAS} #bf{Preliminary}");
  t_tr_CB->SetFillColor(0);
  t_tr_CB->SetLineColor(0);
  t_tr_CB->SetBorderSize(0);
  t_tr_CB->SetTextAlign(30);
  t_tr_ME = new TPaveText(*t_tr_CB);
  t_tr_MS = new TPaveText(*t_tr_CB);
  t_tr_ID = new TPaveText(*t_tr_CB);
  t_tr_CB->AddText("#bf{Combined Muons}");
  t_tr_ME->AddText("#bf{MS Extrapolated Muons}");
  t_tr_MS->AddText("#bf{MS Only Muons}");
  t_tr_ID->AddText("#bf{ID Muons}");
  
  // vector of legends
  std::vector<TLegend*> *vl = new std::vector<TLegend*>();
  // m
  vl->push_back(l_tl); vl->push_back(l_tl); vl->push_back(l_tl); vl->push_back(l_tl);
  // pt
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr);  
  // eta
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr);  
  // phi
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr);  
  // z0
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr);  
  // dosig
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr);  
  // e
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr);  
  // pzmumu
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr);  
  // rho
  vl->push_back(l_tr); vl->push_back(l_tr);

  // vector of tpavetext labels
  std::vector<TPaveText*> *vt = new std::vector<TPaveText*>();
  // m
  vt->push_back(t_tl_CB); vt->push_back(t_tl_ME); vt->push_back(t_tl_MS); vt->push_back(t_tl_ID);
  // pt
  vt->push_back(t_tr_CB); vt->push_back(t_tr_ME); vt->push_back(t_tr_MS); vt->push_back(t_tr_ID);  
  // eta
  vt->push_back(t_tc_CB); vt->push_back(t_tc_ME); vt->push_back(t_tc_MS); vt->push_back(t_tc_ID);  
  // phi
  vt->push_back(t_tc_CB); vt->push_back(t_tc_ME); vt->push_back(t_tc_MS); vt->push_back(t_tc_ID);  
  // z0
  vt->push_back(t_tc_CB); vt->push_back(t_tc_ME); vt->push_back(t_tc_MS); vt->push_back(t_tc_ID);  
  // dosig
  vt->push_back(t_tr_CB); vt->push_back(t_tr_ME); vt->push_back(t_tr_MS); vt->push_back(t_tr_ID);  
  // e
  vt->push_back(t_tr_CB); vt->push_back(t_tr_ME); vt->push_back(t_tr_MS); vt->push_back(t_tr_ID);  
  // pzmumu
  vt->push_back(t_tr_CB); vt->push_back(t_tr_ME); vt->push_back(t_tr_MS); vt->push_back(t_tr_ID);  
  // rho
  vt->push_back(nullptr); vt->push_back(nullptr);

  // currently plotting 8 variables 
  const int Nvar = 8*4+2;
  double maxFactor[Nvar] = {};
  for (int i = 0; i < 4; i++) {
    maxFactor[0*4+i] = 1.05; // m
    maxFactor[1*4+i] = 1.05; // pt
    maxFactor[2*4+i] = 1.3; // eta
    maxFactor[3*4+i] = 1.3; // phi
    maxFactor[4*4+i] = 1.3; // z0
    maxFactor[5*4+i] = 1.05; // d0sig
    maxFactor[6*4+i] = 1.05; // e
    maxFactor[7*4+i] = 1.05; // pzmumu
  }
  maxFactor[32] = 1.05;
  maxFactor[33] = 1.05;

  bool skip[Nvar] = {};
  
  // set histogram max values
  double max = 0, int_o = 0, int_n = 0, int_min = 0;
  for (int i = 0; i < vh_o->size(); i++) {
    int_o = vh_o->at(i)->Integral();
    int_n = vh_n->at(i)->Integral();
    int_min = min(int_n, int_o);
    if (int_o >0 and int_n > 0) {
      vh_o->at(i)->Scale(int_min/int_o);
      vh_n->at(i)->Scale(int_min/int_n);
    }
    else skip[i] = true;

    max = 0;
    max = std::max(vh_o->at(i)->GetMaximum(), vh_n->at(i)->GetMaximum());
    max *= maxFactor[i];

    vh_o->at(i)->SetMaximum(max);
    vh_n->at(i)->SetMaximum(max);
    vh_o->at(i)->SetMinimum(0);
    vh_n->at(i)->SetMinimum(0);
  }
 
  // plotting
  ratioCan->cd();
  ratioCan->Clear();
  for (int i = 0; i < vh_o->size(); i++) {
    if (!skip[i]) 
      MakeRatioPlot(ratioCan, vh_o->at(i), vh_n->at(i), vl->at(i), vt->at(i), 
                    oldMapName, newMapName, print, file_prefix);
  }
  ratioCan->Close();
  
  for (int i = 0; i < vh_o->size(); i++) {
    delete vh_o->at(i);
    delete vh_n->at(i);
  }
  delete vh_o;
  delete vh_n;
  delete l_tl;
  delete t_tl_CB; 
  delete t_tl_ME; 
  delete t_tl_MS; 
  delete t_tl_ID;
  delete t_tr_CB; 
  delete t_tr_ME; 
  delete t_tr_MS; 
  delete t_tr_ID;
  delete t_tc_CB; 
  delete t_tc_ME; 
  delete t_tc_MS; 
  delete t_tc_ID;
  delete vt;
  delete vl;
}

/**************************************************************************************************/

void GeneratePlots() {
  // declare and print file prefix
  //string file_prefix = "00339396.recon.DRAW_ZMUMU.stefano_";
  string file_prefix = "TEST_";
  string oldFileName = "bfield_map_stefano_RunI.outputs.root";
  string newFileName = "bfield_map_stefano_2016.outputs.root";
  bool print =  true;

  cout << "Old map reconstruction: " << oldFileName << endl;
  cout << "New map reconstruction: " << newFileName << endl;
  if (print)
    cout << "Preparing output with prefix " << file_prefix.c_str() << endl; 
  
  MakeRatioPlots(print, file_prefix, oldFileName, newFileName);
  //MakeProfilePlots(print, file_prefix, oldFileName, newFileName);
}
