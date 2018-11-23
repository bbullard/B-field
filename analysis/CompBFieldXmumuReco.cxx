#include <TFile.h>
#include <TTreeReader.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TLorentzVector.h>
#include <TLatex.h>
#include <math.h>
#include <string>
#include <vector>
#include <string.h>
#include <fstream>

#include "PlottingHelpers.cxx"

using namespace std;

void FillPlots(string file_prefix, string oldFileName, string newFileName) {
  // style options
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetPadBorderSize(2);
  
  // canvas for ratio plots
  TCanvas *ratioCan = new TCanvas("ratioCan","",1000,1000);

  const int nBins = 50;

  // invariant mass
  TH1F* h_m_CB_o = new TH1F("h_m_CB_o","",nBins,55,110);
  h_m_CB_o->SetTitle(";m_{#mu#bar{#mu}} (GeV);Events");
  TH1F* h_m_ME_o = (TH1F*)h_m_CB_o->Clone("h_m_ME_o");
  TH1F* h_m_MSO_o = (TH1F*)h_m_CB_o->Clone("h_m_MSO_o");
  TH1F* h_m_MSOE_o = (TH1F*)h_m_CB_o->Clone("h_m_MSOE_o");
  TH1F* h_m_ID_o = (TH1F*)h_m_CB_o->Clone("h_m_ID_o");
  
  TH1F* h_m_CB_n = (TH1F*)h_m_CB_o->Clone("h_m_CB_n");
  TH1F* h_m_ME_n = (TH1F*)h_m_CB_o->Clone("h_m_ME_n");
  TH1F* h_m_MSO_n = (TH1F*)h_m_CB_o->Clone("h_m_MSO_n");
  TH1F* h_m_MSOE_n = (TH1F*)h_m_CB_o->Clone("h_m_MSOE_n");
  TH1F* h_m_ID_n = (TH1F*)h_m_CB_o->Clone("h_m_ID_n");

  // leading pt
  TH1F* h_pt1_CB_o = new TH1F("h_pt1_CB_o","",nBins,0,120);
  h_pt1_CB_o->SetTitle(";Leading p_{T} (GeV);Events");
  TH1F* h_pt1_ME_o = (TH1F*)h_pt1_CB_o->Clone("h_pt1_ME_o");
  TH1F* h_pt1_MSO_o = (TH1F*)h_pt1_CB_o->Clone("h_pt1_MSO_o");
  TH1F* h_pt1_MSOE_o = (TH1F*)h_pt1_CB_o->Clone("h_pt1_MSOE_o");
  TH1F* h_pt1_ID_o = (TH1F*)h_pt1_CB_o->Clone("h_pt1_ID_o");

  TH1F* h_pt1_CB_n = (TH1F*)h_pt1_CB_o->Clone("h_pt1_CB_n");
  TH1F* h_pt1_ME_n = (TH1F*)h_pt1_CB_o->Clone("h_pt1_ME_n");
  TH1F* h_pt1_MSO_n = (TH1F*)h_pt1_CB_o->Clone("h_pt1_MSO_n");
  TH1F* h_pt1_MSOE_n = (TH1F*)h_pt1_CB_o->Clone("h_pt1_MSOE_n");
  TH1F* h_pt1_ID_n = (TH1F*)h_pt1_CB_o->Clone("h_pt1_ID_n");
 
  // subleading pt 
  TH1F* h_pt2_CB_o = new TH1F("h_pt2_CB_o","",nBins,0,120);
  h_pt2_CB_o->SetTitle(";Subleading p_{T} (GeV);Events");
  TH1F* h_pt2_ME_o = (TH1F*)h_pt2_CB_o->Clone("h_pt2_ME_o");
  TH1F* h_pt2_MSO_o = (TH1F*)h_pt2_CB_o->Clone("h_pt2_MSO_o");
  TH1F* h_pt2_MSOE_o = (TH1F*)h_pt2_CB_o->Clone("h_pt2_MSOE_o");
  TH1F* h_pt2_ID_o = (TH1F*)h_pt2_CB_o->Clone("h_pt2_ID_o");

  TH1F* h_pt2_CB_n = (TH1F*)h_pt2_CB_o->Clone("h_pt2_CB_n");
  TH1F* h_pt2_ME_n = (TH1F*)h_pt2_CB_o->Clone("h_pt2_ME_n");
  TH1F* h_pt2_MSO_n = (TH1F*)h_pt2_CB_o->Clone("h_pt2_MSO_n");
  TH1F* h_pt2_MSOE_n = (TH1F*)h_pt2_CB_o->Clone("h_pt2_MSOE_n");
  TH1F* h_pt2_ID_n = (TH1F*)h_pt2_CB_o->Clone("h_pt2_ID_n");
  
  // leading eta 
  TH1F* h_eta1_CB_o = new TH1F("h_eta1_CB_o","",nBins,-2.5,2.5);
  h_eta1_CB_o->SetTitle(";Leading Muon #eta;Events");
  TH1F* h_eta1_ME_o = (TH1F*)h_eta1_CB_o->Clone("h_eta1_ME_o");
  TH1F* h_eta1_MSO_o = (TH1F*)h_eta1_CB_o->Clone("h_eta1_MSO_o");
  TH1F* h_eta1_MSOE_o = (TH1F*)h_eta1_CB_o->Clone("h_eta1_MSOE_o");
  TH1F* h_eta1_ID_o = (TH1F*)h_eta1_CB_o->Clone("h_eta1_ID_o");

  TH1F* h_eta1_CB_n = (TH1F*)h_eta1_CB_o->Clone("h_eta1_CB_n");
  TH1F* h_eta1_ME_n = (TH1F*)h_eta1_CB_o->Clone("h_eta1_ME_n");
  TH1F* h_eta1_MSO_n = (TH1F*)h_eta1_CB_o->Clone("h_eta1_MSO_n");
  TH1F* h_eta1_MSOE_n = (TH1F*)h_eta1_CB_o->Clone("h_eta1_MSOE_n");
  TH1F* h_eta1_ID_n = (TH1F*)h_eta1_CB_o->Clone("h_eta1_ID_n");
 
  // subleading eta 
  TH1F* h_eta2_CB_o = new TH1F("h_eta2_CB_o","",nBins,-2.5,2.5);
  h_eta2_CB_o->SetTitle(";Subleading Muon #eta;Events");
  TH1F* h_eta2_ME_o = (TH1F*)h_eta2_CB_o->Clone("h_eta2_ME_o");
  TH1F* h_eta2_MSO_o = (TH1F*)h_eta2_CB_o->Clone("h_eta2_MSO_o");
  TH1F* h_eta2_MSOE_o = (TH1F*)h_eta2_CB_o->Clone("h_eta2_MSOE_o");
  TH1F* h_eta2_ID_o = (TH1F*)h_eta2_CB_o->Clone("h_eta2_ID_o");

  TH1F* h_eta2_CB_n = (TH1F*)h_eta2_CB_o->Clone("h_eta2_CB_n");
  TH1F* h_eta2_ME_n = (TH1F*)h_eta2_CB_o->Clone("h_eta2_ME_n");
  TH1F* h_eta2_MSO_n = (TH1F*)h_eta2_CB_o->Clone("h_eta2_MSO_n");
  TH1F* h_eta2_MSOE_n = (TH1F*)h_eta2_CB_o->Clone("h_eta2_MSOE_n");
  TH1F* h_eta2_ID_n = (TH1F*)h_eta2_CB_o->Clone("h_eta2_ID_n");
  
  // author
  TH1F* h_author1_o = new TH1F("h_author1_o","",6,0,6);
  h_author1_o->SetTitle(";Leading Muon Author;Events");
  TH1F* h_author2_o = new TH1F("h_author2_o","",6,0,6);
  h_author2_o->SetTitle(";Subleading Muon Author;Events");

  TH1F* h_author1_n = (TH1F*)h_author1_o->Clone("h_author1_n");
  TH1F* h_author2_n = (TH1F*)h_author2_o->Clone("h_author2_n");

  // quality
  TH1F* h_quality_o = new TH1F("h_quality_o","",4,0,4);
  h_quality_o->SetTitle(";Quality;Events");
  
  TH1F* h_quality_n = (TH1F*)h_quality_o->Clone("h_quality_n");
  
  // number of precision layers
  TH1F* h_nPrecLay_o = new TH1F("h_nPrecLay_o","",8,0,8);
  h_nPrecLay_o->SetTitle(";Number of Precision Layers;Events");
  
  TH1F* h_nPrecLay_n = (TH1F*)h_nPrecLay_o->Clone("h_nPrecLay_n");
  
  // number of precision hole layers
  TH1F* h_nPrecHoleLay_o = new TH1F("h_nPrecHoleLay_o","",8,0,8);
  h_nPrecHoleLay_o->SetTitle(";Number of Precision Hole Layers;Events");
  
  TH1F* h_nPrecHoleLay_n = (TH1F*)h_nPrecHoleLay_o->Clone("h_nPrecHoleLay_n");
  
  // rho leading 
  TH1F* h_rho1_ME_o = new TH1F("h_rho1_ME_o","",nBins,-.4,.4);
  h_rho1_ME_o->SetTitle(";Leading Muon #rho_{ME};Events");
  TH1F* h_rho1_MSO_o = (TH1F*)h_rho1_ME_o->Clone("h_rho1_MSO_o");
  h_rho1_MSO_o->SetTitle(";Leading Muon #rho_{MS};Events");

  TH1F* h_rho1_ME_n = (TH1F*)h_rho1_ME_o->Clone("h_rho1_ME_n");
  TH1F* h_rho1_MSO_n = (TH1F*)h_rho1_MSO_o->Clone("h_rho1_MSO_n");
  
  // rho subleading
  TH1F* h_rho2_ME_o = new TH1F("h_rho2_ME_o","",nBins,-.4,.4);
  h_rho2_ME_o->SetTitle(";Subleading Muon #rho_{ME};Events");
  TH1F* h_rho2_MSO_o = (TH1F*)h_rho2_ME_o->Clone("h_rho2_MSO_o");
  h_rho2_MSO_o->SetTitle(";Subleading Muon #rho_{MS};Events");

  TH1F* h_rho2_ME_n = (TH1F*)h_rho2_ME_o->Clone("h_rho2_ME_n");
  TH1F* h_rho2_MSO_n = (TH1F*)h_rho2_MSO_o->Clone("h_rho2_MSO_n");
  
  // dR (MSOE or ME) and ID leading
  TH1F* h_dR1_ME_o = new TH1F("h_dR1_ME_o","",nBins,0,.01);
  h_dR1_ME_o->SetTitle(";Leading Muon #Delta R_{ME,ID};Events");
  TH1F* h_dR1_MSOE_o = new TH1F("h_dR1_MSOE_o","",nBins,0,0.05);
  h_dR1_MSOE_o->SetTitle(";Leading Muon #Delta R_{MSOE,ID};Events");

  TH1F* h_dR1_ME_n = (TH1F*)h_dR1_ME_o->Clone("h_dR1_ME_n");
  TH1F* h_dR1_MSOE_n = (TH1F*)h_dR1_MSOE_o->Clone("h_dR1_MSOE_n");

  // dR (MSOE or ME) and ID subleading
  TH1F* h_dR2_ME_o = new TH1F("h_dR2_ME_o","",nBins,0,.01);
  h_dR2_ME_o->SetTitle(";Subleading Muon #Delta R_{ME,ID};Events");
  TH1F* h_dR2_MSOE_o = new TH1F("h_dR2_MSOE_o","",nBins,0,.05);
  h_dR2_MSOE_o->SetTitle(";Subleading Muon #Delta R_{MSOE,ID};Events");

  TH1F* h_dR2_ME_n = (TH1F*)h_dR2_ME_o->Clone("h_dR2_ME_n");
  TH1F* h_dR2_MSOE_n = (TH1F*)h_dR2_MSOE_o->Clone("h_dR2_MSOE_n");
 
  // variables as function of eta
  // pt
  TH1F* h_ptXeta_ID_o = new TH1F("h_ptXeta_ID_o","",nBins,-2.5,2.5);
  h_ptXeta_ID_o->SetTitle(";#eta;p_{T} (GeV)");
  TH1F* h_ptXeta_ME_o = (TH1F*)h_ptXeta_ID_o->Clone("h_ptXeta_ME_o");
  TH1F* h_ptXeta_MSO_o = (TH1F*)h_ptXeta_ID_o->Clone("h_ptXeta_MSO_o");

  TH1F* h_ptXeta_ID_n = new TH1F("h_ptXeta_ID_n","",nBins,-2.5,2.5);
  h_ptXeta_ID_n->SetTitle(";#eta;p_{T} (GeV)");
  TH1F* h_ptXeta_ME_n = (TH1F*)h_ptXeta_ID_n->Clone("h_ptXeta_ME_n");
  TH1F* h_ptXeta_MSO_n = (TH1F*)h_ptXeta_ID_n->Clone("h_ptXeta_MSO_n");
  
  TH1F* h_dptXeta_ID_o = new TH1F("h_dptXeta_ID_o","",nBins,-2.5,2.5);
  h_dptXeta_ID_o->SetTitle(";#eta;#sigma(p_{T}) (GeV)");
  TH1F* h_dptXeta_ME_o = (TH1F*)h_dptXeta_ID_o->Clone("h_dptXeta_ME_o");
  TH1F* h_dptXeta_MSO_o = (TH1F*)h_dptXeta_ID_o->Clone("h_dptXeta_MSO_o");

  TH1F* h_dptXeta_ID_n = new TH1F("h_dptXeta_ID_n","",nBins,-2.5,2.5);
  h_dptXeta_ID_n->SetTitle(";#eta;#sigma(p_{T}) (GeV)");
  TH1F* h_dptXeta_ME_n = (TH1F*)h_dptXeta_ID_n->Clone("h_dptXeta_ME_n");
  TH1F* h_dptXeta_MSO_n = (TH1F*)h_dptXeta_ID_n->Clone("h_dptXeta_MSO_n");
 
  TProfile* p_pt_ID_o = new TProfile("p_pt_ID_o","",nBins,-2.5,2.5,"s");
  p_pt_ID_o->SetMinimum(0); 
  p_pt_ID_o->SetMaximum(120); 
  
  TProfile* p_pt_ME_o = (TProfile*)p_pt_ID_o->Clone("p_pt_ME_o");
  TProfile* p_pt_MSO_o = (TProfile*)p_pt_ID_o->Clone("p_pt_MSO_o");
  TProfile* p_pt_ID_n = (TProfile*)p_pt_ID_o->Clone("p_pt_ID_n");
  TProfile* p_pt_ME_n = (TProfile*)p_pt_ID_o->Clone("p_pt_ME_n");
  TProfile* p_pt_MSO_n = (TProfile*)p_pt_ID_o->Clone("p_pt_MSO_n");
  
  // rho
  TH1F* h_rhoXeta_ME_o = new TH1F("h_rhoXeta_ME_o","",nBins,-2.5,2.5);
  h_rhoXeta_ME_o->SetTitle(";#eta_{ID};#rho (ME-ID)");
  TH1F* h_rhoXeta_MSO_o = (TH1F*)h_rhoXeta_ME_o->Clone("h_rhoXeta_MSO_o");
  h_rhoXeta_MSO_o->SetTitle(";#eta_{ID};#rho (MS-ID)");

  TH1F* h_rhoXeta_ME_n = new TH1F("h_rhoXeta_ME_n","",nBins,-2.5,2.5);
  h_rhoXeta_ME_n->SetTitle(";#eta_{ID};#rho (ME-ID)");
  TH1F* h_rhoXeta_MSO_n = (TH1F*)h_rhoXeta_ME_n->Clone("h_rhoXeta_MSO_n");
  h_rhoXeta_MSO_n->SetTitle(";#eta_{ID};#rho (MS-ID)");
  
  TH1F* h_dXeta_ME_o = new TH1F("h_drhoXeta_ME_o","",nBins,-2.5,2.5);
  h_drhoXeta_ME_o->SetTitle(";#eta_{ID};#sigma(#rho) (ME-ID)");
  TH1F* h_drhoXeta_MSO_o = (TH1F*)h_drhoXeta_ME_o->Clone("h_drhoXeta_MSO_o");
  h_drhoXeta_MSO_o->SetTitle(";#eta_{ID};#sigma(#rho) (MS-ID)");

  TH1F* h_drhoXeta_ME_n = new TH1F("h_drhoXeta_ME_n","",nBins,-2.5,2.5);
  h_drhoXeta_ME_n->SetTitle(";#eta_{ID};#sigma(#rho) (ME-ID)");
  TH1F* h_drhoXeta_MSO_n = (TH1F*)h_drhoXeta_ME_n->Clone("h_drhoXeta_MSO_n");
  h_drhoXeta_MSO_n->SetTitle(";#eta_{ID};#sigma(#rho) (MS-ID)");
 
  TProfile* p_rho_ME_o = new TProfile("p_rho_ME_o","",nBins,-2.5,2.5,"s");
  p_rho_ME_o->SetMinimum(-.5); 
  p_rho_ME_o->SetMaximum(.5); 
  
  TProfile* p_rho_MSO_o = (TProfile*)p_rho_ME_o->Clone("p_rho_MSO_o");
  TProfile* p_rho_ME_n = (TProfile*)p_rho_ME_o->Clone("p_rho_ME_n");
  TProfile* p_rho_MSO_n = (TProfile*)p_rho_ME_o->Clone("p_rho_MSO_n");
  
  // dR
  TH1F* h_dRXeta_ME_o = new TH1F("h_dRXeta_ME_o","",nBins,-2.5,2.5);
  h_dRXeta_ME_o->SetTitle(";#eta_{ID};#Delta R (ME-ID)");
  TH1F* h_dRXeta_MSOE_o = (TH1F*)h_dRXeta_ME_o->Clone("h_dRXeta_MSOE_o");
  h_dRXeta_MSOE_o->SetTitle(";#eta_{ID};#Delta R (MSOE-ID)");

  TH1F* h_dRXeta_ME_n = new TH1F("h_dRXeta_ME_n","",nBins,-2.5,2.5);
  h_dRXeta_ME_n->SetTitle(";#eta_{ID};#Delta R (ME-ID)");
  TH1F* h_dRXeta_MSOE_n = (TH1F*)h_dRXeta_ME_n->Clone("h_dRXeta_MSOE_n");
  h_dRXeta_MSOE_n->SetTitle(";#eta_{ID};#Delta R (MSOE-ID)");
  
  TH1F* h_ddRXeta_ME_o = new TH1F("h_ddRXeta_ME_o","",nBins,-2.5,2.5);
  h_ddRXeta_ME_o->SetTitle(";#eta_{ID};#sigma(#Delta R) (ME-ID)");
  TH1F* h_ddRXeta_MSOE_o = (TH1F*)h_ddRXeta_ME_o->Clone("h_ddRXeta_MSOE_o");
  h_ddRXeta_MSOE_o->SetTitle(";#eta_{ID};#sigma(#Delta R) (MSOE-ID)");

  TH1F* h_ddRXeta_ME_n = new TH1F("h_ddRXeta_ME_n","",nBins,-2.5,2.5);
  h_ddRXeta_ME_n->SetTitle(";#eta_{ID};#sigma(#Delta R) (ME-ID)");
  TH1F* h_ddRXeta_MSOE_n = (TH1F*)h_ddRXeta_ME_n->Clone("h_ddRXeta_MSOE_n");
  h_ddRXeta_MSOE_n->SetTitle(";#eta_{ID};#sigma(#Delta R) (MSOE-ID)");
 
  TProfile* p_dR_ME_o = new TProfile("p_dR_ME_o","",nBins,-2.5,2.5,"s");
  p_dR_ME_o->SetMinimum(0); 
  p_dR_ME_o->SetMaximum(0.6); 
  
  TProfile* p_dR_MSOE_o = (TProfile*)p_dR_ME_o->Clone("p_dR_MSOE_o");
  TProfile* p_dR_ME_n = (TProfile*)p_dR_ME_o->Clone("p_dR_ME_n");
  TProfile* p_dR_MSOE_n = (TProfile*)p_dR_ME_o->Clone("p_dR_MSOE_n");
  
  // add histograms to vectors for formatting
  std::vector<TH1F*> *vh_o = new std::vector<TH1F*>();
  std::vector<TH1F*> *vh_n = new std::vector<TH1F*>();
  std::vector<double> *maxFactor = new std::vector<double>();

  // push back vector of histograms and plot range scalings            
  vh_o->push_back(h_m_CB_o);            vh_n->push_back(h_m_CB_n);          maxFactor->push_back(1.05);
  vh_o->push_back(h_m_ME_o);            vh_n->push_back(h_m_ME_n);          maxFactor->push_back(1.05);
  vh_o->push_back(h_m_MSO_o);           vh_n->push_back(h_m_MSO_n);         maxFactor->push_back(1.05);
  vh_o->push_back(h_m_MSOE_o);          vh_n->push_back(h_m_MSOE_n);        maxFactor->push_back(1.05);
  vh_o->push_back(h_m_ID_o);            vh_n->push_back(h_m_ID_n);          maxFactor->push_back(1.05);
  vh_o->push_back(h_pt1_CB_o);          vh_n->push_back(h_pt1_CB_n);        maxFactor->push_back(1.05);
  vh_o->push_back(h_pt1_ME_o);          vh_n->push_back(h_pt1_ME_n);        maxFactor->push_back(1.05);
  vh_o->push_back(h_pt1_MSO_o);         vh_n->push_back(h_pt1_MSO_n);       maxFactor->push_back(1.05);
  vh_o->push_back(h_pt1_MSOE_o);        vh_n->push_back(h_pt1_MSOE_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_pt1_ID_o);          vh_n->push_back(h_pt1_ID_n);        maxFactor->push_back(1.05);
  vh_o->push_back(h_pt2_CB_o);          vh_n->push_back(h_pt2_CB_n);        maxFactor->push_back(1.05);
  vh_o->push_back(h_pt2_ME_o);          vh_n->push_back(h_pt2_ME_n);        maxFactor->push_back(1.05);
  vh_o->push_back(h_pt2_MSO_o);         vh_n->push_back(h_pt2_MSO_n);       maxFactor->push_back(1.05);
  vh_o->push_back(h_pt2_MSOE_o);        vh_n->push_back(h_pt2_MSOE_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_pt2_ID_o);          vh_n->push_back(h_pt2_ID_n);        maxFactor->push_back(1.05);
  vh_o->push_back(h_eta1_CB_o);         vh_n->push_back(h_eta1_CB_n);       maxFactor->push_back(1.3);
  vh_o->push_back(h_eta1_ME_o);         vh_n->push_back(h_eta1_ME_n);       maxFactor->push_back(1.3);
  vh_o->push_back(h_eta1_MSO_o);        vh_n->push_back(h_eta1_MSO_n);      maxFactor->push_back(1.3);
  vh_o->push_back(h_eta1_MSOE_o);       vh_n->push_back(h_eta1_MSOE_n);     maxFactor->push_back(1.3);
  vh_o->push_back(h_eta1_ID_o);         vh_n->push_back(h_eta1_ID_n);       maxFactor->push_back(1.3);
  vh_o->push_back(h_eta2_CB_o);         vh_n->push_back(h_eta2_CB_n);       maxFactor->push_back(1.3);
  vh_o->push_back(h_eta2_ME_o);         vh_n->push_back(h_eta2_ME_n);       maxFactor->push_back(1.3);
  vh_o->push_back(h_eta2_MSO_o);        vh_n->push_back(h_eta2_MSO_n);      maxFactor->push_back(1.3);
  vh_o->push_back(h_eta2_MSOE_o);       vh_n->push_back(h_eta2_MSOE_n);     maxFactor->push_back(1.3);
  vh_o->push_back(h_eta2_ID_o);         vh_n->push_back(h_eta2_ID_n);       maxFactor->push_back(1.3);
  vh_o->push_back(h_author1_o);         vh_n->push_back(h_author1_n);       maxFactor->push_back(1.05);
  vh_o->push_back(h_author2_o);         vh_n->push_back(h_author2_n);       maxFactor->push_back(1.05);
  vh_o->push_back(h_quality_o);         vh_n->push_back(h_quality_n);       maxFactor->push_back(1.05);
  vh_o->push_back(h_nPrecLay_o);        vh_n->push_back(h_nPrecLay_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_nPrecHoleLay_o);    vh_n->push_back(h_nPrecHoleLay_n);  maxFactor->push_back(1.05);
  vh_o->push_back(h_rho1_ME_o);         vh_n->push_back(h_rho1_ME_n);       maxFactor->push_back(1.05);
  vh_o->push_back(h_rho1_MSO_o);        vh_n->push_back(h_rho1_MSO_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_rho2_ME_o);         vh_n->push_back(h_rho2_ME_n);       maxFactor->push_back(1.05);
  vh_o->push_back(h_rho2_MSO_o);        vh_n->push_back(h_rho2_MSO_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_dR1_ME_o);          vh_n->push_back(h_dR1_ME_n);        maxFactor->push_back(1.05);
  vh_o->push_back(h_dR1_MSOE_o);        vh_n->push_back(h_dR1_MSOE_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_dR2_ME_o);          vh_n->push_back(h_dR2_ME_n);        maxFactor->push_back(1.05);
  vh_o->push_back(h_dR2_MSOE_o);        vh_n->push_back(h_dR2_MSOE_n);      maxFactor->push_back(1.05);
  
  vh_o->push_back(h_ptXeta_ME_o);       vh_n->push_back(h_ptXeta_ME_n);     maxFactor->push_back(1.3);
  vh_o->push_back(h_ptXeta_MSO_o);      vh_n->push_back(h_ptXeta_MSO_n);    maxFactor->push_back(1.3);
  vh_o->push_back(h_ptXeta_ID_o);       vh_n->push_back(h_ptXeta_ID_n);     maxFactor->push_back(1.3);
  vh_o->push_back(h_dptXeta_ME_o);      vh_n->push_back(h_dptXeta_ME_n);    maxFactor->push_back(1.3);
  vh_o->push_back(h_dptXeta_MSO_o);     vh_n->push_back(h_dptXeta_MSO_n);   maxFactor->push_back(1.3);
  vh_o->push_back(h_dptXeta_ID_o);      vh_n->push_back(h_dptXeta_ID_n);    maxFactor->push_back(1.3);
  vh_o->push_back(h_rhoXeta_ME_o);      vh_n->push_back(h_rhoXeta_ME_n);    maxFactor->push_back(1.3);
  vh_o->push_back(h_rhoXeta_MSO_o);     vh_n->push_back(h_rhoXeta_MSO_n);   maxFactor->push_back(1.3);
  vh_o->push_back(h_drhoXeta_ME_o);     vh_n->push_back(h_drhoXeta_ME_n);   maxFactor->push_back(1.3);
  vh_o->push_back(h_drhoXeta_MSO_o);    vh_n->push_back(h_drhoXeta_MSO_n);  maxFactor->push_back(1.3);
  vh_o->push_back(h_dRXeta_ME_o);       vh_n->push_back(h_dRXeta_ME_n);     maxFactor->push_back(1.3);
  vh_o->push_back(h_dRXeta_MSOE_o);     vh_n->push_back(h_dRXeta_MSOE_n);   maxFactor->push_back(1.3);
  vh_o->push_back(h_ddRXeta_ME_o);      vh_n->push_back(h_ddRXeta_ME_n);    maxFactor->push_back(1.3);
  vh_o->push_back(h_ddRXeta_MSOE_o);    vh_n->push_back(h_ddRXeta_MSOE_n);  maxFactor->push_back(1.3);
  
  // check that the vectors have the same size
  if ( vh_o->size() != vh_n->size() || vh_o->size() != maxFactor->size()) 
    std::cout << "Warning: histogram vector sizes unequal between two maps" << std::endl;
  
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
  //string oldMapName = "Official AOD";
  string oldMapName = "Run I";

  // event level  
  TTreeReader reader_old("RecoMuons", f_recoOld);
  TTreeReaderValue<bool> passGRL_o(reader_old, "passGRL");
  TTreeReaderValue<unsigned long long> eventNumber_o(reader_old, "eventNumber");
  
  // positive muon variables
  //TTreeReaderValue<float> p_pdgID_truth_o(reader_old, "p_pdgID_truth");
  //TTreeReaderValue<float> p_pt_truth_o(reader_old, "p_pt_truth");
 	//TTreeReaderValue<float> p_eta_truth_o(reader_old, "p_eta_truth");
  //TTreeReaderValue<float> p_phi_truth_o(reader_old, "p_phi_truth");
  //TTreeReaderValue<float> p_m_truth_o(reader_old, "p_m_truth");
  TTreeReaderValue<bool> p_passIDcuts_o(reader_old, "p_passIDcuts");
  TTreeReaderValue<bool> p_passAll_o(reader_old, "p_passAll");
  TTreeReaderValue<float> p_ptcone40_o(reader_old, "p_ptcone40");
  TTreeReaderValue<int> p_muonType_o(reader_old, "p_muonType");
  TTreeReaderValue<int> p_quality_o(reader_old, "p_quality");
  TTreeReaderValue<int> p_primaryAuthor_o(reader_old, "p_primaryAuthor");
  TTreeReaderValue<int> p_authors_o(reader_old, "p_authors");
  TTreeReaderValue<int> p_nPrecisionLayers_o(reader_old, "p_nPrecisionLayers");
  TTreeReaderValue<int> p_nPrecisionHoleLayers_o(reader_old, "p_nPrecisionHoleLayers");

  // negative muon variables
  //TTreeReaderValue<float> n_pdgID_truth_o(reader_old, "n_pdgID_truth");
  //TTreeReaderValue<float> n_pt_truth_o(reader_old, "n_pt_truth");
 	//TTreeReaderValue<float> n_eta_truth_o(reader_old, "n_eta_truth");
  //TTreeReaderValue<float> n_phi_truth_o(reader_old, "n_phi_truth");
  //TTreeReaderValue<float> n_m_truth_o(reader_old, "n_m_truth");
  TTreeReaderValue<bool> n_passIDcuts_o(reader_old, "n_passIDcuts");
  TTreeReaderValue<bool> n_passAll_o(reader_old, "n_passAll");
  TTreeReaderValue<float> n_eLoss_o(reader_old, "n_eLoss");
  TTreeReaderValue<float> n_ptcone40_o(reader_old, "n_ptcone40");
  TTreeReaderValue<int> n_muonType_o(reader_old, "n_muonType");
  TTreeReaderValue<int> n_quality_o(reader_old, "n_quality");
  TTreeReaderValue<int> n_primaryAuthor_o(reader_old, "n_primaryAuthor");
  TTreeReaderValue<int> n_authors_o(reader_old, "n_authors");
  TTreeReaderValue<int> n_nPrecisionLayers_o(reader_old, "n_nPrecisionLayers");
  TTreeReaderValue<int> n_nPrecisionHoleLayers_o(reader_old, "n_nPrecisionHoleLayers");

  // positive muon track variables
  TTreeReaderValue<bool> p_isCB_o(reader_old, "p_isCB");
  TTreeReaderValue<bool> p_isME_o(reader_old, "p_isME");
  TTreeReaderValue<bool> p_isMSO_o(reader_old, "p_isMSO");
  TTreeReaderValue<bool> p_isMSOE_o(reader_old, "p_isMSOE");
  TTreeReaderValue<bool> p_isID_o(reader_old, "p_isID");
  TTreeReaderValue<float> p_eta_CB_o(reader_old, "p_eta_CB");
  TTreeReaderValue<float> p_eta_ME_o(reader_old, "p_eta_ME");
  TTreeReaderValue<float> p_eta_MSO_o(reader_old, "p_eta_MSO");
  TTreeReaderValue<float> p_eta_MSOE_o(reader_old, "p_eta_MSOE");
  TTreeReaderValue<float> p_eta_ID_o(reader_old, "p_eta_ID");
  TTreeReaderValue<float> p_phi_CB_o(reader_old, "p_phi_CB");
  TTreeReaderValue<float> p_phi_ME_o(reader_old, "p_phi_ME");
  TTreeReaderValue<float> p_phi_MSO_o(reader_old, "p_phi_MSO");
  TTreeReaderValue<float> p_phi_MSOE_o(reader_old, "p_phi_MSOE");
  TTreeReaderValue<float> p_phi_ID_o(reader_old, "p_phi_ID");
  TTreeReaderValue<float> p_d0sig_CB_o(reader_old, "p_d0sig_CB");
  TTreeReaderValue<float> p_d0sig_ME_o(reader_old, "p_d0sig_ME");
  TTreeReaderValue<float> p_d0sig_MSO_o(reader_old, "p_d0sig_MSO");
  TTreeReaderValue<float> p_d0sig_MSOE_o(reader_old, "p_d0sig_MSOE");
  TTreeReaderValue<float> p_d0sig_ID_o(reader_old, "p_d0sig_ID");
  TTreeReaderValue<float> p_z0_CB_o(reader_old, "p_z0_CB");
  TTreeReaderValue<float> p_z0_ME_o(reader_old, "p_z0_ME");
  TTreeReaderValue<float> p_z0_MSO_o(reader_old, "p_z0_MSO");
  TTreeReaderValue<float> p_z0_MSOE_o(reader_old, "p_z0_MSOE");
  TTreeReaderValue<float> p_z0_ID_o(reader_old, "p_z0_ID");
  TTreeReaderValue<float> p_qOverP_CB_o(reader_old, "p_qOverP_CB");
  TTreeReaderValue<float> p_qOverP_ME_o(reader_old, "p_qOverP_ME");
  TTreeReaderValue<float> p_qOverP_MSO_o(reader_old, "p_qOverP_MSO");
  TTreeReaderValue<float> p_qOverP_MSOE_o(reader_old, "p_qOverP_MSOE");
  TTreeReaderValue<float> p_qOverP_ID_o(reader_old, "p_qOverP_ID");
  TTreeReaderValue<float> p_dqOverP_CB_o(reader_old, "p_dqOverP_CB");
  TTreeReaderValue<float> p_dqOverP_ME_o(reader_old, "p_dqOverP_ME");
  TTreeReaderValue<float> p_dqOverP_MSO_o(reader_old, "p_dqOverP_MSO");
  TTreeReaderValue<float> p_dqOverP_MSOE_o(reader_old, "p_dqOverP_MSOE");
  TTreeReaderValue<float> p_dqOverP_ID_o(reader_old, "p_dqOverP_ID");
  TTreeReaderValue<float> p_rchi2_CB_o(reader_old, "p_rchi2_CB");
  TTreeReaderValue<float> p_rchi2_ME_o(reader_old, "p_rchi2_ME");
  TTreeReaderValue<float> p_rchi2_MSO_o(reader_old, "p_rchi2_MSO");
  TTreeReaderValue<float> p_rchi2_MSOE_o(reader_old, "p_rchi2_MSOE");
  TTreeReaderValue<float> p_rchi2_ID_o(reader_old, "p_rchi2_ID");
  
  // negative muon track variables
  TTreeReaderValue<bool> n_isCB_o(reader_old, "n_isCB");
  TTreeReaderValue<bool> n_isME_o(reader_old, "n_isME");
  TTreeReaderValue<bool> n_isMSO_o(reader_old, "n_isMSO");
  TTreeReaderValue<bool> n_isMSOE_o(reader_old, "n_isMSOE");
  TTreeReaderValue<bool> n_isID_o(reader_old, "n_isID");
  TTreeReaderValue<float> n_eta_CB_o(reader_old, "n_eta_CB");
  TTreeReaderValue<float> n_eta_ME_o(reader_old, "n_eta_ME");
  TTreeReaderValue<float> n_eta_MSO_o(reader_old, "n_eta_MSO");
  TTreeReaderValue<float> n_eta_MSOE_o(reader_old, "n_eta_MSOE");
  TTreeReaderValue<float> n_eta_ID_o(reader_old, "n_eta_ID");
  TTreeReaderValue<float> n_phi_CB_o(reader_old, "n_phi_CB");
  TTreeReaderValue<float> n_phi_ME_o(reader_old, "n_phi_ME");
  TTreeReaderValue<float> n_phi_MSO_o(reader_old, "n_phi_MSO");
  TTreeReaderValue<float> n_phi_MSOE_o(reader_old, "n_phi_MSOE");
  TTreeReaderValue<float> n_phi_ID_o(reader_old, "n_phi_ID");
  TTreeReaderValue<float> n_d0sig_CB_o(reader_old, "n_d0sig_CB");
  TTreeReaderValue<float> n_d0sig_ME_o(reader_old, "n_d0sig_ME");
  TTreeReaderValue<float> n_d0sig_MSO_o(reader_old, "n_d0sig_MSO");
  TTreeReaderValue<float> n_d0sig_MSOE_o(reader_old, "n_d0sig_MSOE");
  TTreeReaderValue<float> n_d0sig_ID_o(reader_old, "n_d0sig_ID");
  TTreeReaderValue<float> n_z0_CB_o(reader_old, "n_z0_CB");
  TTreeReaderValue<float> n_z0_ME_o(reader_old, "n_z0_ME");
  TTreeReaderValue<float> n_z0_MSO_o(reader_old, "n_z0_MSO");
  TTreeReaderValue<float> n_z0_MSOE_o(reader_old, "n_z0_MSOE");
  TTreeReaderValue<float> n_z0_ID_o(reader_old, "n_z0_ID");
  TTreeReaderValue<float> n_qOverP_CB_o(reader_old, "n_qOverP_CB");
  TTreeReaderValue<float> n_qOverP_ME_o(reader_old, "n_qOverP_ME");
  TTreeReaderValue<float> n_qOverP_MSO_o(reader_old, "n_qOverP_MSO");
  TTreeReaderValue<float> n_qOverP_MSOE_o(reader_old, "n_qOverP_MSOE");
  TTreeReaderValue<float> n_qOverP_ID_o(reader_old, "n_qOverP_ID");
  TTreeReaderValue<float> n_dqOverP_CB_o(reader_old, "n_dqOverP_CB");
  TTreeReaderValue<float> n_dqOverP_ME_o(reader_old, "n_dqOverP_ME");
  TTreeReaderValue<float> n_dqOverP_MSO_o(reader_old, "n_dqOverP_MSO");
  TTreeReaderValue<float> n_dqOverP_MSOE_o(reader_old, "n_dqOverP_MSOE");
  TTreeReaderValue<float> n_dqOverP_ID_o(reader_old, "n_dqOverP_ID");
  TTreeReaderValue<float> n_rchi2_CB_o(reader_old, "n_rchi2_CB");
  TTreeReaderValue<float> n_rchi2_ME_o(reader_old, "n_rchi2_ME");
  TTreeReaderValue<float> n_rchi2_MSO_o(reader_old, "n_rchi2_MSO");
  TTreeReaderValue<float> n_rchi2_MSOE_o(reader_old, "n_rchi2_MSOE");
  TTreeReaderValue<float> n_rchi2_ID_o(reader_old, "n_rchi2_ID");
  
  long int nEvents_o = 0;
  long int nPassGRL_o = 0;
  long int nPassIDcuts_o = 0;
  long int nPassMedium_o = 0;

  // loop through TTreeReader
  while (reader_old.Next()) {
    nEvents_o++;
    h_quality_o->Fill(*p_quality_o);
    h_quality_o->Fill(*n_quality_o);
    h_nPrecLay_o->Fill(*p_nPrecLay_o);
    h_nPrecLay_o->Fill(*n_nPrecLay_o);
    h_nPrecHoleLay_o->Fill(*p_nPrecHoleLay_o);
    h_nPrecHoleLay_o->Fill(*n_nPrecHoleLay_o);
    
    // skip event if event not on GRL
    if (!*passGRL_o) continue;
    nPassGRL_o++;
  
    // skip event if muons are not tight or medium quality 
    if (*p_quality_o > 1 || *n_quality_o > 1) continue;
    nPassIDcuts_o++;
  
    // skip event if muons fail ID cuts
    if (!*p_passIDcuts_o || !*n_passIDcuts_o)  continue;
    nPassMedium_o++;

    // determine charge of leading muon
    // find charge of leading muon
    double pt_p, pt_n;
    if (*p_isCB_o) pt_p = 1 / cosh(*p_eta_CB_o) / abs(*p_qOverP_CB_o);
    else pt_p = 1 / cosh(*p_eta_ME_o) / abs(*p_qOverP_ME_o);
    if (*n_isCB_o) pt_n = 1 / cosh(*n_eta_CB_o) / abs(*n_qOverP_CB_o);
    else pt_n = 1 / cosh(*n_eta_ME_o) / abs(*n_qOverP_ME_o);
    int q_lead = -1;
    if (pt_p > pt_n)
      q_lead = 1;

    // skip events with pt(lead)<30, pt(sublead)<20
    //if (max(pt_p, pt_n) < 30) continue;
    //if (min(pt_p, pt_n) < 20) continue;
    
    // fill primary author
    if (q_lead == 1) {
      h_author1_o->Fill(*p_primaryAuthor_o); 
      h_author2_o->Fill(*n_primaryAuthor_o); 
    }
    else {
      h_author1_o->Fill(*n_primaryAuthor_o); 
      h_author2_o->Fill(*p_primaryAuthor_o); 
    }
  
    // fill invariant mass
    TLorentzVector v1, v2;
    if (*p_isCB_o and *n_isCB_o) {
      v1.SetPtEtaPhiM(1 / cosh(*p_eta_CB_o) / abs(*p_qOverP_CB_o), *p_eta_CB_o, *p_phi_CB_o, .10566);
      v2.SetPtEtaPhiM(1 / cosh(*n_eta_CB_o) / abs(*n_qOverP_CB_o), *n_eta_CB_o, *n_phi_CB_o, .10566);
      h_m_CB_o->Fill((v1+v2).M());
    }
    if (*p_isME_o and *n_isME_o) {
      v1.SetPtEtaPhiM(1 / cosh(*p_eta_ME_o) / abs(*p_qOverP_ME_o), *p_eta_ME_o, *p_phi_ME_o, .10566);
      v2.SetPtEtaPhiM(1 / cosh(*n_eta_ME_o) / abs(*n_qOverP_ME_o), *n_eta_ME_o, *n_phi_ME_o, .10566);
      h_m_ME_o->Fill((v1+v2).M());
    }
    if (*p_isMSO_o and *n_isMSO_o) {
      v1.SetPtEtaPhiM(1 / cosh(*p_eta_MSO_o) / abs(*p_qOverP_MSO_o), *p_eta_MSO_o, *p_phi_MSO_o, .10566);
      v2.SetPtEtaPhiM(1 / cosh(*n_eta_MSO_o) / abs(*n_qOverP_MSO_o), *n_eta_MSO_o, *n_phi_MSO_o, .10566);
      h_m_MSO_o->Fill((v1+v2).M());
    }
    if (*p_isMSOE_o and *n_isMSOE_o) {
      v1.SetPtEtaPhiM(1 / cosh(*p_eta_MSOE_o) / abs(*p_qOverP_MSOE_o), *p_eta_MSOE_o, *p_phi_MSOE_o, .10566);
      v2.SetPtEtaPhiM(1 / cosh(*n_eta_MSOE_o) / abs(*n_qOverP_MSOE_o), *n_eta_MSOE_o, *n_phi_MSOE_o, .10566);
      h_m_MSOE_o->Fill((v1+v2).M());
    }
    if (*p_isID_o and *n_isID_o) {
      v1.SetPtEtaPhiM(1 / cosh(*p_eta_ID_o) / abs(*p_qOverP_ID_o), *p_eta_ID_o, *p_phi_ID_o, .10566);
      v2.SetPtEtaPhiM(1 / cosh(*n_eta_ID_o) / abs(*n_qOverP_ID_o), *n_eta_ID_o, *n_phi_ID_o, .10566); 
      h_m_ID_o->Fill((v1+v2).M());
    }

    // fill pt profiles
    if (*p_isID_o) 
      p_pt_ID_o->Fill(*p_eta_ID_o, 1 / cosh(*p_eta_ID_o) / abs(*p_qOverP_ID_o));
    if (*n_isID_o) 
      p_pt_ID_o->Fill(*n_eta_ID_o, 1 / cosh(*n_eta_ID_o) / abs(*n_qOverP_ID_o));
    if (*p_isME_o) 
      p_pt_ME_o->Fill(*p_eta_ME_o, 1 / cosh(*p_eta_ME_o) / abs(*p_qOverP_ME_o));
    if (*n_isME_o) 
      p_pt_ME_o->Fill(*n_eta_ME_o, 1 / cosh(*n_eta_ME_o) / abs(*n_qOverP_ME_o));
    if (*p_isMSO_o) 
      p_pt_MSO_o->Fill(*p_eta_MSO_o, 1 / cosh(*p_eta_MSO_o) / abs(*p_qOverP_MSO_o));
    if (*n_isMSO_o) 
      p_pt_MSO_o->Fill(*n_eta_MSO_o, 1 / cosh(*n_eta_MSO_o) / abs(*n_qOverP_MSO_o));

    // fill leading/subleading variable histograms
    if (q_lead == 1) {
      if (*p_isCB_o) {
        h_pt1_CB_o->Fill(1 / cosh(*p_eta_CB_o) / abs(*p_qOverP_CB_o)); 
        h_eta1_CB_o->Fill(*p_eta_CB_o);
      }
      if (*n_isCB_o) {
        h_pt2_CB_o->Fill(1 / cosh(*n_eta_CB_o) / abs(*n_qOverP_CB_o)); 
        h_eta2_CB_o->Fill(*n_eta_CB_o);
      }
    
      if (*p_isME_o) {
        h_pt1_ME_o->Fill(1 / cosh(*p_eta_ME_o) / abs(*p_qOverP_ME_o)); 
        h_eta1_ME_o->Fill(*p_eta_ME_o);
      }
      if (*n_isME_o) {
        h_pt2_ME_o->Fill(1 / cosh(*n_eta_ME_o) / abs(*n_qOverP_ME_o)); 
        h_eta2_ME_o->Fill(*n_eta_ME_o);
      }
    
      if (*p_isMSO_o) {
        h_pt1_MSO_o->Fill(1 / cosh(*p_eta_MSO_o) / abs(*p_qOverP_MSO_o)); 
        h_eta1_MSO_o->Fill(*p_eta_MSO_o);
      }
      if (*n_isMSO_o) {
        h_pt2_MSO_o->Fill(1 / cosh(*n_eta_MSO_o) / abs(*n_qOverP_MSO_o)); 
        h_eta2_MSO_o->Fill(*n_eta_MSO_o);
      }
    
      if (*p_isMSOE_o) {
        h_pt1_MSOE_o->Fill(1 / cosh(*p_eta_MSOE_o) / abs(*p_qOverP_MSOE_o)); 
        h_eta1_MSOE_o->Fill(*p_eta_MSOE_o);
      }
      if (*n_isMSOE_o) {
        h_pt2_MSOE_o->Fill(1 / cosh(*n_eta_MSOE_o) / abs(*n_qOverP_MSOE_o)); 
        h_eta2_MSOE_o->Fill(*n_eta_MSOE_o);
      }
    
      if (*p_isID_o) {
        h_pt1_ID_o->Fill(1 / cosh(*p_eta_ID_o) / abs(*p_qOverP_ID_o)); 
        h_eta1_ID_o->Fill(*p_eta_ID_o);
        if (*p_isME_o) {
          double ptID = 1 / cosh(*p_eta_ID_o) / abs(*p_qOverP_ID_o);
          double ptME = 1 / cosh(*p_eta_ME_o) / abs(*p_qOverP_ME_o);
          h_rho1_ME_o->Fill((ptME-ptID)/ptID);
          p_rho_ME_o->Fill(*p_eta_ID_o, (ptME-ptID)/ptID);
          double deta2 = pow(*p_eta_ID_o - *p_eta_ME_o, 2);
          double dphi2 = pow(*p_phi_ID_o - *p_phi_ME_o, 2);
          h_dR1_ME_o->Fill(sqrt(deta2 + dphi2));
          p_dR_ME_o->Fill(*p_eta_ID_o, sqrt(deta2 + dphi2));
        }
        if (*p_isMSO_o) {
          double ptID = 1 / cosh(*p_eta_ID_o) / abs(*p_qOverP_ID_o);
          double ptMSO = 1 / cosh(*p_eta_MSO_o) / abs(*p_qOverP_MSO_o);
          h_rho1_MSO_o->Fill((ptMSO-ptID)/ptID);
          p_rho_MSO_o->Fill(*p_eta_ID_o, (ptMSO-ptID)/ptID);
        }
        if (*p_isMSOE_o)
          double deta2 = pow(*p_eta_ID_o - *p_eta_MSOE_o, 2);
          double dphi2 = pow(*p_phi_ID_o - *p_phi_MSOE_o, 2);
          h_dR1_MSOE_o->Fill(sqrt(deta2 + dphi2));
          p_dR_MSOE_o->Fill(*p_eta_ID_o, sqrt(deta2 + dphi2));
      }
      if (*n_isID_o) {
        h_pt2_ID_o->Fill(1 / cosh(*n_eta_ID_o) / abs(*n_qOverP_ID_o)); 
        h_eta2_ID_o->Fill(*n_eta_ID_o);
        if (*n_isME_o) {
          double ptID = 1 / cosh(*n_eta_ID_o) / abs(*n_qOverP_ID_o);
          double ptME = 1 / cosh(*n_eta_ME_o) / abs(*n_qOverP_ME_o);
          h_rho2_ME_o->Fill((ptME-ptID)/ptID);
          p_rho_ME_o->Fill(*n_eta_ID_o, (ptME-ptID)/ptID);
          double deta2 = pow(*n_eta_ID_o - *n_eta_ME_o, 2);
          double dphi2 = pow(*n_phi_ID_o - *n_phi_ME_o, 2);
          h_dR2_ME_o->Fill(sqrt(deta2 + dphi2));
          p_dR_ME_o->Fill(*n_eta_ID_o, sqrt(deta2 + dphi2));
        }
        if (*n_isMSO_o) {
          double ptID = 1 / cosh(*n_eta_ID_o) / abs(*n_qOverP_ID_o);
          double ptMSO = 1 / cosh(*n_eta_MSO_o) / abs(*n_qOverP_MSO_o);
          h_rho2_MSO_o->Fill((ptMSO-ptID)/ptID);
          p_rho_MSO_o->Fill(*n_eta_ID_o, (ptMSO-ptID)/ptID);
        }
        if (*n_isMSOE_o)
          double deta2 = pow(*n_eta_ID_o - *n_eta_MSOE_o, 2);
          double dphi2 = pow(*n_phi_ID_o - *n_phi_MSOE_o, 2);
          h_dR2_MSOE_o->Fill(sqrt(deta2 + dphi2));
          p_dR_MSOE_o->Fill(*n_eta_ID_o, sqrt(deta2 + dphi2));
      }
    }
    // if negative muon is primary
    else {
      if (*p_isCB_o) {
        h_pt2_CB_o->Fill(1 / cosh(*p_eta_CB_o) / abs(*p_qOverP_CB_o)); 
        h_eta2_CB_o->Fill(*p_eta_CB_o);
      }
      if (*n_isCB_o) {
        h_pt1_CB_o->Fill(1 / cosh(*n_eta_CB_o) / abs(*n_qOverP_CB_o)); 
        h_eta1_CB_o->Fill(*n_eta_CB_o);
      }
    
      if (*p_isME_o) {
        h_pt2_ME_o->Fill(1 / cosh(*p_eta_ME_o) / abs(*p_qOverP_ME_o)); 
        h_eta2_ME_o->Fill(*p_eta_ME_o);
      }
      if (*n_isME_o) {
        h_pt1_ME_o->Fill(1 / cosh(*n_eta_ME_o) / abs(*n_qOverP_ME_o)); 
        h_eta1_ME_o->Fill(*n_eta_ME_o);
      }
    
      if (*p_isMSO_o) {
        h_pt2_MSO_o->Fill(1 / cosh(*p_eta_MSO_o) / abs(*p_qOverP_MSO_o)); 
        h_eta2_MSO_o->Fill(*p_eta_MSO_o);
      }
      if (*n_isMSO_o) {
        h_pt1_MSO_o->Fill(1 / cosh(*n_eta_MSO_o) / abs(*n_qOverP_MSO_o)); 
        h_eta1_MSO_o->Fill(*n_eta_MSO_o);
      }
    
      if (*p_isMSOE_o) {
        h_pt2_MSOE_o->Fill(1 / cosh(*p_eta_MSOE_o) / abs(*p_qOverP_MSOE_o)); 
        h_eta2_MSOE_o->Fill(*p_eta_MSOE_o);
      }
      if (*n_isMSOE_o) {
        h_pt1_MSOE_o->Fill(1 / cosh(*n_eta_MSOE_o) / abs(*n_qOverP_MSOE_o)); 
        h_eta1_MSOE_o->Fill(*n_eta_MSOE_o);
      }
    
      if (*p_isID_o) {
        h_pt2_ID_o->Fill(1 / cosh(*p_eta_ID_o) / abs(*p_qOverP_ID_o)); 
        h_eta2_ID_o->Fill(*p_eta_ID_o);
        if (*p_isME_o) {
          double ptID = 1 / cosh(*p_eta_ID_o) / abs(*p_qOverP_ID_o);
          double ptME = 1 / cosh(*p_eta_ME_o) / abs(*p_qOverP_ME_o);
          h_rho2_ME_o->Fill((ptME-ptID)/ptID);
          p_rho_ME_o->Fill(*p_eta_ID_o, (ptME-ptID)/ptID);
          double deta2 = pow(*p_eta_ID_o - *p_eta_ME_o, 2);
          double dphi2 = pow(*p_phi_ID_o - *p_phi_ME_o, 2);
          h_dR2_ME_o->Fill(sqrt(deta2 + dphi2));
          p_dR_ME_o->Fill(*p_eta_ID_o, sqrt(deta2 + dphi2));
        }
        if (*p_isMSO_o) {
          double ptID = 1 / cosh(*p_eta_ID_o) / abs(*p_qOverP_ID_o);
          double ptMSO = 1 / cosh(*p_eta_MSO_o) / abs(*p_qOverP_MSO_o);
          h_rho2_MSO_o->Fill((ptMSO-ptID)/ptID);
          p_rho_MSO_o->Fill(*p_eta_ID_o, (ptMSO-ptID)/ptID);
        }
        if (*p_isMSOE_o)
          double deta2 = pow(*p_eta_ID_o - *p_eta_MSOE_o, 2);
          double dphi2 = pow(*p_phi_ID_o - *p_phi_MSOE_o, 2);
          h_dR2_MSOE_o->Fill(sqrt(deta2 + dphi2));
          p_dR_MSOE_o->Fill(*p_eta_ID_o, sqrt(deta2 + dphi2));
      }
      if (*n_isID_o) {
        h_pt1_ID_o->Fill(1 / cosh(*n_eta_ID_o) / abs(*n_qOverP_ID_o)); 
        h_eta1_ID_o->Fill(*n_eta_ID_o);
        if (*n_isME_o) {
          double ptID = 1 / cosh(*n_eta_ID_o) / abs(*n_qOverP_ID_o);
          double ptME = 1 / cosh(*n_eta_ME_o) / abs(*n_qOverP_ME_o);
          h_rho1_ME_o->Fill((ptME-ptID)/ptID);
          p_rho_ME_o->Fill(*n_eta_ID_o, (ptME-ptID)/ptID);
          double deta2 = pow(*n_eta_ID_o - *n_eta_ME_o, 2);
          double dphi2 = pow(*n_phi_ID_o - *n_phi_ME_o, 2);
          h_dR1_ME_o->Fill(sqrt(deta2 + dphi2));
          p_dR_ME_o->Fill(*n_eta_ID_o, sqrt(deta2 + dphi2));
        }
        if (*n_isMSO_o) {
          double ptID = 1 / cosh(*n_eta_ID_o) / abs(*n_qOverP_ID_o);
          double ptMSO = 1 / cosh(*n_eta_MSO_o) / abs(*n_qOverP_MSO_o);
          h_rho1_MSO_o->Fill((ptMSO-ptID)/ptID);
          p_rho_MSO_o->Fill(*n_eta_ID_o, (ptMSO-ptID)/ptID);
        }
        if (*n_isMSOE_o)
          double deta2 = pow(*n_eta_ID_o - *n_eta_MSOE_o, 2);
          double dphi2 = pow(*n_phi_ID_o - *n_phi_MSOE_o, 2);
          h_dR1_MSOE_o->Fill(sqrt(deta2 + dphi2));
          p_dR_MSOE_o->Fill(*n_eta_ID_o, sqrt(deta2 + dphi2));
      }
    }
  }
  delete f_recoOld;

  // load old file and set up reader
  TFile* f_recoNew = new TFile(Form("ntuples_muonSelection/%s", newFileName.c_str()));
  if (!f_recoNew) cout << "Warning: could not open file " << newFileName << endl;
  //string newMapName = "Official AOD";
  string newMapName = "2016";

  // event level  
  TTreeReader reader_new("RecoMuons", f_recoNew);
  TTreeReaderValue<bool> passGRL_n(reader_new, "passGRL");
  TTreeReaderValue<unsigned long long> eventNumber_n(reader_new, "eventNumber");
  
  // positive muon variables
  //TTreeReaderValue<float> p_pdgID_truth_o(reader_new, "p_pdgID_truth");
  //TTreeReaderValue<float> p_pt_truth_o(reader_new, "p_pt_truth");
 	//TTreeReaderValue<float> p_eta_truth_o(reader_new, "p_eta_truth");
  //TTreeReaderValue<float> p_phi_truth_o(reader_new, "p_phi_truth");
  //TTreeReaderValue<float> p_m_truth_o(reader_new, "p_m_truth");
  TTreeReaderValue<bool> p_passIDcuts_n(reader_new, "p_passIDcuts");
  TTreeReaderValue<bool> p_passAll_n(reader_new, "p_passAll");
  TTreeReaderValue<float> p_ptcone40_n(reader_new, "p_ptcone40");
  TTreeReaderValue<int> p_muonType_n(reader_new, "p_muonType");
  TTreeReaderValue<int> p_quality_n(reader_new, "p_quality");
  TTreeReaderValue<int> p_primaryAuthor_n(reader_new, "p_primaryAuthor");
  TTreeReaderValue<int> p_authors_n(reader_new, "p_authors");
  TTreeReaderValue<int> p_nPrecisionLayers_n(reader_new, "p_nPrecisionLayers");
  TTreeReaderValue<int> p_nPrecisionHoleLayers_n(reader_new, "p_nPrecisionHoleLayers");

  // negative muon variables
  //TTreeReaderValue<float> n_pdgID_truth_n(reader_new, "n_pdgID_truth");
  //TTreeReaderValue<float> n_pt_truth_n(reader_new, "n_pt_truth");
 	//TTreeReaderValue<float> n_eta_truth_n(reader_new, "n_eta_truth");
  //TTreeReaderValue<float> n_phi_truth_n(reader_new, "n_phi_truth");
  //TTreeReaderValue<float> n_m_truth_n(reader_new, "n_m_truth");
  TTreeReaderValue<bool> n_passIDcuts_n(reader_new, "n_passIDcuts");
  TTreeReaderValue<bool> n_passAll_n(reader_new, "n_passAll");
  TTreeReaderValue<float> n_eLoss_n(reader_new, "n_eLoss");
  TTreeReaderValue<float> n_ptcone40_n(reader_new, "n_ptcone40");
  TTreeReaderValue<int> n_muonType_n(reader_new, "n_muonType");
  TTreeReaderValue<int> n_quality_n(reader_new, "n_quality");
  TTreeReaderValue<int> n_primaryAuthor_n(reader_new, "n_primaryAuthor");
  TTreeReaderValue<int> n_authors_n(reader_new, "n_authors");
  TTreeReaderValue<int> n_nPrecisionLayers_n(reader_new, "n_nPrecisionLayers");
  TTreeReaderValue<int> n_nPrecisionHoleLayers_n(reader_new, "n_nPrecisionHoleLayers");

  // positive muon track variables
  TTreeReaderValue<bool> p_isCB_n(reader_new, "p_isCB");
  TTreeReaderValue<bool> p_isME_n(reader_new, "p_isME");
  TTreeReaderValue<bool> p_isMSO_n(reader_new, "p_isMSO");
  TTreeReaderValue<bool> p_isMSOE_n(reader_new, "p_isMSOE");
  TTreeReaderValue<bool> p_isID_n(reader_new, "p_isID");
  TTreeReaderValue<float> p_eta_CB_n(reader_new, "p_eta_CB");
  TTreeReaderValue<float> p_eta_ME_n(reader_new, "p_eta_ME");
  TTreeReaderValue<float> p_eta_MSO_n(reader_new, "p_eta_MSO");
  TTreeReaderValue<float> p_eta_MSOE_n(reader_new, "p_eta_MSOE");
  TTreeReaderValue<float> p_eta_ID_n(reader_new, "p_eta_ID");
  TTreeReaderValue<float> p_phi_CB_n(reader_new, "p_phi_CB");
  TTreeReaderValue<float> p_phi_ME_n(reader_new, "p_phi_ME");
  TTreeReaderValue<float> p_phi_MSO_n(reader_new, "p_phi_MSO");
  TTreeReaderValue<float> p_phi_MSOE_n(reader_new, "p_phi_MSOE");
  TTreeReaderValue<float> p_phi_ID_n(reader_new, "p_phi_ID");
  TTreeReaderValue<float> p_d0sig_CB_n(reader_new, "p_d0sig_CB");
  TTreeReaderValue<float> p_d0sig_ME_n(reader_new, "p_d0sig_ME");
  TTreeReaderValue<float> p_d0sig_MSO_n(reader_new, "p_d0sig_MSO");
  TTreeReaderValue<float> p_d0sig_MSOE_n(reader_new, "p_d0sig_MSOE");
  TTreeReaderValue<float> p_d0sig_ID_n(reader_new, "p_d0sig_ID");
  TTreeReaderValue<float> p_z0_CB_n(reader_new, "p_z0_CB");
  TTreeReaderValue<float> p_z0_ME_n(reader_new, "p_z0_ME");
  TTreeReaderValue<float> p_z0_MSO_n(reader_new, "p_z0_MSO");
  TTreeReaderValue<float> p_z0_MSOE_n(reader_new, "p_z0_MSOE");
  TTreeReaderValue<float> p_z0_ID_n(reader_new, "p_z0_ID");
  TTreeReaderValue<float> p_qOverP_CB_n(reader_new, "p_qOverP_CB");
  TTreeReaderValue<float> p_qOverP_ME_n(reader_new, "p_qOverP_ME");
  TTreeReaderValue<float> p_qOverP_MSO_n(reader_new, "p_qOverP_MSO");
  TTreeReaderValue<float> p_qOverP_MSOE_n(reader_new, "p_qOverP_MSOE");
  TTreeReaderValue<float> p_qOverP_ID_n(reader_new, "p_qOverP_ID");
  TTreeReaderValue<float> p_dqOverP_CB_n(reader_new, "p_dqOverP_CB");
  TTreeReaderValue<float> p_dqOverP_ME_n(reader_new, "p_dqOverP_ME");
  TTreeReaderValue<float> p_dqOverP_MSO_n(reader_new, "p_dqOverP_MSO");
  TTreeReaderValue<float> p_dqOverP_MSOE_n(reader_new, "p_dqOverP_MSOE");
  TTreeReaderValue<float> p_dqOverP_ID_n(reader_new, "p_dqOverP_ID");
  TTreeReaderValue<float> p_rchi2_CB_n(reader_new, "p_rchi2_CB");
  TTreeReaderValue<float> p_rchi2_ME_n(reader_new, "p_rchi2_ME");
  TTreeReaderValue<float> p_rchi2_MSO_n(reader_new, "p_rchi2_MSO");
  TTreeReaderValue<float> p_rchi2_MSOE_n(reader_new, "p_rchi2_MSOE");
  TTreeReaderValue<float> p_rchi2_ID_n(reader_new, "p_rchi2_ID");
  
  // negative muon track variables
  TTreeReaderValue<bool> n_isCB_n(reader_new, "n_isCB");
  TTreeReaderValue<bool> n_isME_n(reader_new, "n_isME");
  TTreeReaderValue<bool> n_isMSO_n(reader_new, "n_isMSO");
  TTreeReaderValue<bool> n_isMSOE_n(reader_new, "n_isMSOE");
  TTreeReaderValue<bool> n_isID_n(reader_new, "n_isID");
  TTreeReaderValue<float> n_eta_CB_n(reader_new, "n_eta_CB");
  TTreeReaderValue<float> n_eta_ME_n(reader_new, "n_eta_ME");
  TTreeReaderValue<float> n_eta_MSO_n(reader_new, "n_eta_MSO");
  TTreeReaderValue<float> n_eta_MSOE_n(reader_new, "n_eta_MSOE");
  TTreeReaderValue<float> n_eta_ID_n(reader_new, "n_eta_ID");
  TTreeReaderValue<float> n_phi_CB_n(reader_new, "n_phi_CB");
  TTreeReaderValue<float> n_phi_ME_n(reader_new, "n_phi_ME");
  TTreeReaderValue<float> n_phi_MSO_n(reader_new, "n_phi_MSO");
  TTreeReaderValue<float> n_phi_MSOE_n(reader_new, "n_phi_MSOE");
  TTreeReaderValue<float> n_phi_ID_n(reader_new, "n_phi_ID");
  TTreeReaderValue<float> n_d0sig_CB_n(reader_new, "n_d0sig_CB");
  TTreeReaderValue<float> n_d0sig_ME_n(reader_new, "n_d0sig_ME");
  TTreeReaderValue<float> n_d0sig_MSO_n(reader_new, "n_d0sig_MSO");
  TTreeReaderValue<float> n_d0sig_MSOE_n(reader_new, "n_d0sig_MSOE");
  TTreeReaderValue<float> n_d0sig_ID_n(reader_new, "n_d0sig_ID");
  TTreeReaderValue<float> n_z0_CB_n(reader_new, "n_z0_CB");
  TTreeReaderValue<float> n_z0_ME_n(reader_new, "n_z0_ME");
  TTreeReaderValue<float> n_z0_MSO_n(reader_new, "n_z0_MSO");
  TTreeReaderValue<float> n_z0_MSOE_n(reader_new, "n_z0_MSOE");
  TTreeReaderValue<float> n_z0_ID_n(reader_new, "n_z0_ID");
  TTreeReaderValue<float> n_qOverP_CB_n(reader_new, "n_qOverP_CB");
  TTreeReaderValue<float> n_qOverP_ME_n(reader_new, "n_qOverP_ME");
  TTreeReaderValue<float> n_qOverP_MSO_n(reader_new, "n_qOverP_MSO");
  TTreeReaderValue<float> n_qOverP_MSOE_n(reader_new, "n_qOverP_MSOE");
  TTreeReaderValue<float> n_qOverP_ID_n(reader_new, "n_qOverP_ID");
  TTreeReaderValue<float> n_dqOverP_CB_n(reader_new, "n_dqOverP_CB");
  TTreeReaderValue<float> n_dqOverP_ME_n(reader_new, "n_dqOverP_ME");
  TTreeReaderValue<float> n_dqOverP_MSO_n(reader_new, "n_dqOverP_MSO");
  TTreeReaderValue<float> n_dqOverP_MSOE_n(reader_new, "n_dqOverP_MSOE");
  TTreeReaderValue<float> n_dqOverP_ID_n(reader_new, "n_dqOverP_ID");
  TTreeReaderValue<float> n_rchi2_CB_n(reader_new, "n_rchi2_CB");
  TTreeReaderValue<float> n_rchi2_ME_n(reader_new, "n_rchi2_ME");
  TTreeReaderValue<float> n_rchi2_MSO_n(reader_new, "n_rchi2_MSO");
  TTreeReaderValue<float> n_rchi2_MSOE_n(reader_new, "n_rchi2_MSOE");
  TTreeReaderValue<float> n_rchi2_ID_n(reader_new, "n_rchi2_ID");
  
  long int nEvents_n = 0;
  long int nPassGRL_n = 0;
  long int nPassIDcuts_n = 0;
  long int nPassMedium_n = 0;
  
  // loop through TTreeReader
  while (reader_new.Next()) {
    nEvents_n++;
    h_quality_n->Fill(*p_quality_n);
    h_quality_n->Fill(*n_quality_n);
    h_nPrecLay_n->Fill(*p_nPrecLay_n);
    h_nPrecLay_n->Fill(*n_nPrecLay_n);
    h_nPrecHoleLay_n->Fill(*p_nPrecHoleLay_n);
    h_nPrecHoleLay_n->Fill(*n_nPrecHoleLay_n);

    // skip event if event not on GRL
    if (!*passGRL_n) continue;
    nPassGRL_n++;

    // skip event if muons are not tight or medium quality 
    if (*p_quality_n > 1 || *n_quality_n > 1) continue;
    nPassIDcuts_n++;

    // skip event if muons fail ID cuts
    if (!*p_passIDcuts_n || !*n_passIDcuts_n)  continue;
    nPassMedium_n++;

    // determine charge of leading muon
    // find charge of leading muon
    double pt_p, pt_n;
    if (*p_isCB_n) pt_p = 1 / cosh(*p_eta_CB_n) / abs(*p_qOverP_CB_n);
    else pt_p = 1 / cosh(*p_eta_ME_n) / abs(*p_qOverP_ME_n);
    if (*n_isCB_n) pt_n = 1 / cosh(*n_eta_CB_n) / abs(*n_qOverP_CB_n);
    else pt_n = 1 / cosh(*n_eta_ME_n) / abs(*n_qOverP_ME_n);
    int q_lead = -1;
    if (pt_p > pt_n)
      q_lead = 1;

    // skip events with pt(lead)<30, pt(sublead)<20
    //if (max(pt_p, pt_n) < 30) continue;
    //if (min(pt_p, pt_n) < 20) continue;
    
    // fill primary author
    if (q_lead == 1) {
      h_author1_n->Fill(*p_primaryAuthor_n); 
      h_author2_n->Fill(*n_primaryAuthor_n); 
    }
    else {
      h_author1_n->Fill(*n_primaryAuthor_n); 
      h_author2_n->Fill(*p_primaryAuthor_n); 
    }
  
    // fill invariant mass
    TLorentzVector v1, v2;
    if (*p_isCB_n and *n_isCB_n) {
      v1.SetPtEtaPhiM(1 / cosh(*p_eta_CB_n) / abs(*p_qOverP_CB_n), *p_eta_CB_n, *p_phi_CB_n, .10566);
      v2.SetPtEtaPhiM(1 / cosh(*n_eta_CB_n) / abs(*n_qOverP_CB_n), *n_eta_CB_n, *n_phi_CB_n, .10566);
      h_m_CB_o->Fill((v1+v2).M());
    }
    if (*p_isME_n and *n_isME_n) {
      v1.SetPtEtaPhiM(1 / cosh(*p_eta_ME_n) / abs(*p_qOverP_ME_n), *p_eta_ME_n, *p_phi_ME_n, .10566);
      v2.SetPtEtaPhiM(1 / cosh(*n_eta_ME_n) / abs(*n_qOverP_ME_n), *n_eta_ME_n, *n_phi_ME_n, .10566);
      h_m_ME_o->Fill((v1+v2).M());
    }
    if (*p_isMSO_n and *n_isMSO_n) {
      v1.SetPtEtaPhiM(1 / cosh(*p_eta_MSO_n) / abs(*p_qOverP_MSO_n), *p_eta_MSO_n, *p_phi_MSO_n, .10566);
      v2.SetPtEtaPhiM(1 / cosh(*n_eta_MSO_n) / abs(*n_qOverP_MSO_n), *n_eta_MSO_n, *n_phi_MSO_n, .10566);
      h_m_MSO_o->Fill((v1+v2).M());
    }
    if (*p_isMSOE_n and *n_isMSOE_n) {
      v1.SetPtEtaPhiM(1 / cosh(*p_eta_MSOE_n) / abs(*p_qOverP_MSOE_n), *p_eta_MSOE_n, *p_phi_MSOE_n, .10566);
      v2.SetPtEtaPhiM(1 / cosh(*n_eta_MSOE_n) / abs(*n_qOverP_MSOE_n), *n_eta_MSOE_n, *n_phi_MSOE_n, .10566);
      h_m_MSOE_o->Fill((v1+v2).M());
    }
    if (*p_isID_n and *n_isID_n) {
      v1.SetPtEtaPhiM(1 / cosh(*p_eta_ID_n) / abs(*p_qOverP_ID_n), *p_eta_ID_n, *p_phi_ID_n, .10566);
      v2.SetPtEtaPhiM(1 / cosh(*n_eta_ID_n) / abs(*n_qOverP_ID_n), *n_eta_ID_n, *n_phi_ID_n, .10566); 
      h_m_ID_o->Fill((v1+v2).M());
    }

    // fill pt profiles
    if (*p_isID_n) 
      p_pt_ID_n->Fill(*p_eta_ID_n, 1 / cosh(*p_eta_ID_n) / abs(*p_qOverP_ID_n));
    if (*n_isID_n) 
      p_pt_ID_n->Fill(*n_eta_ID_n, 1 / cosh(*n_eta_ID_n) / abs(*n_qOverP_ID_n));
    if (*p_isME_n) 
      p_pt_ME_n->Fill(*p_eta_ME_n, 1 / cosh(*p_eta_ME_n) / abs(*p_qOverP_ME_n));
    if (*n_isME_n) 
      p_pt_ME_n->Fill(*n_eta_ME_n, 1 / cosh(*n_eta_ME_n) / abs(*n_qOverP_ME_n));
    if (*p_isMSO_n) 
      p_pt_MSO_n->Fill(*p_eta_MSO_n, 1 / cosh(*p_eta_MSO_n) / abs(*p_qOverP_MSO_n));
    if (*n_isMSO_n) 
      p_pt_MSO_n->Fill(*n_eta_MSO_n, 1 / cosh(*n_eta_MSO_n) / abs(*n_qOverP_MSO_n));
 
    // fill leading/subleading variable histograms
    if (q_lead == 1) {
      if (*p_isCB_n) {
        h_pt1_CB_n->Fill(1 / cosh(*p_eta_CB_n) / abs(*p_qOverP_CB_n)); 
        h_eta1_CB_n->Fill(*p_eta_CB_n);
      }
      if (*n_isCB_n) {
        h_pt2_CB_n->Fill(1 / cosh(*n_eta_CB_n) / abs(*n_qOverP_CB_n)); 
        h_eta2_CB_n->Fill(*n_eta_CB_n);
      }
    
      if (*p_isME_n) {
        h_pt1_ME_n->Fill(1 / cosh(*p_eta_ME_n) / abs(*p_qOverP_ME_n)); 
        h_eta1_ME_n->Fill(*p_eta_ME_n);
      }
      if (*n_isME_n) {
        h_pt2_ME_n->Fill(1 / cosh(*n_eta_ME_n) / abs(*n_qOverP_ME_n)); 
        h_eta2_ME_n->Fill(*n_eta_ME_n);
      }
    
      if (*p_isMSO_n) {
        h_pt1_MSO_n->Fill(1 / cosh(*p_eta_MSO_n) / abs(*p_qOverP_MSO_n)); 
        h_eta1_MSO_n->Fill(*p_eta_MSO_n);
      }
      if (*n_isMSO_n) {
        h_pt2_MSO_n->Fill(1 / cosh(*n_eta_MSO_n) / abs(*n_qOverP_MSO_n)); 
        h_eta2_MSO_n->Fill(*n_eta_MSO_n);
      }
    
      if (*p_isMSOE_n) {
        h_pt1_MSOE_n->Fill(1 / cosh(*p_eta_MSOE_n) / abs(*p_qOverP_MSOE_n)); 
        h_eta1_MSOE_n->Fill(*p_eta_MSOE_n);
      }
      if (*n_isMSOE_n) {
        h_pt2_MSOE_n->Fill(1 / cosh(*n_eta_MSOE_n) / abs(*n_qOverP_MSOE_n)); 
        h_eta2_MSOE_n->Fill(*n_eta_MSOE_n);
      }
    
      if (*p_isID_n) {
        h_pt1_ID_n->Fill(1 / cosh(*p_eta_ID_n) / abs(*p_qOverP_ID_n)); 
        h_eta1_ID_n->Fill(*p_eta_ID_n);
        if (*p_isME_n) {
          double ptID = 1 / cosh(*p_eta_ID_n) / abs(*p_qOverP_ID_n);
          double ptME = 1 / cosh(*p_eta_ME_n) / abs(*p_qOverP_ME_n);
          h_rho1_ME_n->Fill((ptME-ptID)/ptID);
          p_rho_ME_n->Fill(*p_eta_ID_n, (ptME-ptID)/ptID);
          double deta2 = pow(*p_eta_ID_n - *p_eta_ME_n, 2);
          double dphi2 = pow(*p_phi_ID_n - *p_phi_ME_n, 2);
          h_dR1_ME_n->Fill(sqrt(deta2 + dphi2));
          p_dR_ME_n->Fill(*p_eta_ID_n, sqrt(deta2 + dphi2));
        }
        if (*p_isMSO_n) {
          double ptID = 1 / cosh(*p_eta_ID_n) / abs(*p_qOverP_ID_n);
          double ptMSO = 1 / cosh(*p_eta_MSO_n) / abs(*p_qOverP_MSO_n);
          h_rho1_MSO_n->Fill((ptMSO-ptID)/ptID);
          p_rho_MSO_n->Fill(*p_eta_ID_n, (ptMSO-ptID)/ptID);
        }
        if (*p_isMSOE_n)
          double deta2 = pow(*p_eta_ID_n - *p_eta_MSOE_n, 2);
          double dphi2 = pow(*p_phi_ID_n - *p_phi_MSOE_n, 2);
          h_dR1_MSOE_n->Fill(sqrt(deta2 + dphi2));
          p_dR_MSOE_n->Fill(*p_eta_ID_n, sqrt(deta2 + dphi2));
      }
      if (*n_isID_n) {
        h_pt2_ID_n->Fill(1 / cosh(*n_eta_ID_n) / abs(*n_qOverP_ID_n)); 
        h_eta2_ID_n->Fill(*n_eta_ID_n);
        if (*n_isME_n) {
          double ptID = 1 / cosh(*n_eta_ID_n) / abs(*n_qOverP_ID_n);
          double ptME = 1 / cosh(*n_eta_ME_n) / abs(*n_qOverP_ME_n);
          h_rho2_ME_n->Fill((ptME-ptID)/ptID);
          p_rho_ME_n->Fill(*n_eta_ID_n, (ptME-ptID)/ptID);
          double deta2 = pow(*n_eta_ID_n - *n_eta_ME_n, 2);
          double dphi2 = pow(*n_phi_ID_n - *n_phi_ME_n, 2);
          h_dR2_ME_n->Fill(sqrt(deta2 + dphi2));
          p_dR_ME_n->Fill(*n_eta_ID_n, sqrt(deta2 + dphi2));
        }
        if (*n_isMSO_n) {
          double ptID = 1 / cosh(*n_eta_ID_n) / abs(*n_qOverP_ID_n);
          double ptMSO = 1 / cosh(*n_eta_MSO_n) / abs(*n_qOverP_MSO_n);
          h_rho2_MSO_n->Fill((ptMSO-ptID)/ptID);
          p_rho_MSO_n->Fill(*n_eta_ID_n, (ptMSO-ptID)/ptID);
        }
        if (*n_isMSOE_n)
          double deta2 = pow(*n_eta_ID_n - *n_eta_MSOE_n, 2);
          double dphi2 = pow(*n_phi_ID_n - *n_phi_MSOE_n, 2);
          h_dR2_MSOE_n->Fill(sqrt(deta2 + dphi2));
          p_dR_MSOE_n->Fill(*n_eta_ID_n, sqrt(deta2 + dphi2));
      }
    }
    // if negative muon is primary
    else {
      if (*p_isCB_n) {
        h_pt2_CB_n->Fill(1 / cosh(*p_eta_CB_n) / abs(*p_qOverP_CB_n)); 
        h_eta2_CB_n->Fill(*p_eta_CB_n);
      }
      if (*n_isCB_n) {
        h_pt1_CB_n->Fill(1 / cosh(*n_eta_CB_n) / abs(*n_qOverP_CB_n)); 
        h_eta1_CB_n->Fill(*n_eta_CB_n);
      }
    
      if (*p_isME_n) {
        h_pt2_ME_n->Fill(1 / cosh(*p_eta_ME_n) / abs(*p_qOverP_ME_n)); 
        h_eta2_ME_n->Fill(*p_eta_ME_n);
      }
      if (*n_isME_n) {
        h_pt1_ME_n->Fill(1 / cosh(*n_eta_ME_n) / abs(*n_qOverP_ME_n)); 
        h_eta1_ME_n->Fill(*n_eta_ME_n);
      }
    
      if (*p_isMSO_n) {
        h_pt2_MSO_n->Fill(1 / cosh(*p_eta_MSO_n) / abs(*p_qOverP_MSO_n)); 
        h_eta2_MSO_n->Fill(*p_eta_MSO_n);
      }
      if (*n_isMSO_n) {
        h_pt1_MSO_n->Fill(1 / cosh(*n_eta_MSO_n) / abs(*n_qOverP_MSO_n)); 
        h_eta1_MSO_n->Fill(*n_eta_MSO_n);
      }
    
      if (*p_isMSOE_n) {
        h_pt2_MSOE_n->Fill(1 / cosh(*p_eta_MSOE_n) / abs(*p_qOverP_MSOE_n)); 
        h_eta2_MSOE_n->Fill(*p_eta_MSOE_n);
      }
      if (*n_isMSOE_n) {
        h_pt1_MSOE_n->Fill(1 / cosh(*n_eta_MSOE_n) / abs(*n_qOverP_MSOE_n)); 
        h_eta1_MSOE_n->Fill(*n_eta_MSOE_n);
      }
    
      if (*p_isID_n) {
        h_pt2_ID_n->Fill(1 / cosh(*p_eta_ID_n) / abs(*p_qOverP_ID_n)); 
        h_eta2_ID_n->Fill(*p_eta_ID_n);
        if (*p_isME_n) {
          double ptID = 1 / cosh(*p_eta_ID_n) / abs(*p_qOverP_ID_n);
          double ptME = 1 / cosh(*p_eta_ME_n) / abs(*p_qOverP_ME_n);
          h_rho2_ME_n->Fill((ptME-ptID)/ptID);
          p_rho_ME_n->Fill(*p_eta_ID_n, (ptME-ptID)/ptID);
          double deta2 = pow(*p_eta_ID_n - *p_eta_ME_n, 2);
          double dphi2 = pow(*p_phi_ID_n - *p_phi_ME_n, 2);
          h_dR2_ME_n->Fill(sqrt(deta2 + dphi2));
          p_dR_ME_n->Fill(*p_eta_ID_n, sqrt(deta2 + dphi2));
        }
        if (*p_isMSO_n) {
          double ptID = 1 / cosh(*p_eta_ID_n) / abs(*p_qOverP_ID_n);
          double ptMSO = 1 / cosh(*p_eta_MSO_n) / abs(*p_qOverP_MSO_n);
          h_rho2_MSO_n->Fill((ptMSO-ptID)/ptID);
          p_rho_MSO_n->Fill(*p_eta_ID_n, (ptMSO-ptID)/ptID);
        }
        if (*p_isMSOE_n)
          double deta2 = pow(*p_eta_ID_n - *p_eta_MSOE_n, 2);
          double dphi2 = pow(*p_phi_ID_n - *p_phi_MSOE_n, 2);
          h_dR2_MSOE_n->Fill(sqrt(deta2 + dphi2));
          p_dR_MSOE_n->Fill(*p_eta_ID_n, sqrt(deta2 + dphi2));
      }
      if (*n_isID_n) {
        h_pt1_ID_n->Fill(1 / cosh(*n_eta_ID_n) / abs(*n_qOverP_ID_n)); 
        h_eta1_ID_n->Fill(*n_eta_ID_n);
        if (*n_isME_n) {
          double ptID = 1 / cosh(*n_eta_ID_n) / abs(*n_qOverP_ID_n);
          double ptME = 1 / cosh(*n_eta_ME_n) / abs(*n_qOverP_ME_n);
          h_rho1_ME_n->Fill((ptME-ptID)/ptID);
          p_rho_ME_n->Fill(*n_eta_ID_n, (ptME-ptID)/ptID);
          double deta2 = pow(*n_eta_ID_n - *n_eta_ME_n, 2);
          double dphi2 = pow(*n_phi_ID_n - *n_phi_ME_n, 2);
          h_dR1_ME_n->Fill(sqrt(deta2 + dphi2));
          p_dR_ME_n->Fill(*n_eta_ID_n, sqrt(deta2 + dphi2));
        }
        if (*n_isMSO_n) {
          double ptID = 1 / cosh(*n_eta_ID_n) / abs(*n_qOverP_ID_n);
          double ptMSO = 1 / cosh(*n_eta_MSO_n) / abs(*n_qOverP_MSO_n);
          h_rho1_MSO_n->Fill((ptMSO-ptID)/ptID);
          p_rho_MSO_n->Fill(*n_eta_ID_n, (ptMSO-ptID)/ptID);
        }
        if (*n_isMSOE_n)
          double deta2 = pow(*n_eta_ID_n - *n_eta_MSOE_n, 2);
          double dphi2 = pow(*n_phi_ID_n - *n_phi_MSOE_n, 2);
          h_dR1_MSOE_n->Fill(sqrt(deta2 + dphi2));
          p_dR_MSOE_n->Fill(*n_eta_ID_n, sqrt(deta2 + dphi2));
      }
    }
  }
  delete f_recoNew;
  
  // loop through bins of profiles to fill varXeta histograms
  for (int bin = 1; bin <= nBins; bin++) {
    h_ptXeta_ME_o->SetBinContent(bin, p_pt_ME_o->GetBinContent(bin));
    h_ptXeta_MSO_o->SetBinContent(bin, p_pt_MSO_o->GetBinContent(bin));
    h_ptXeta_ID_o->SetBinContent(bin, p_pt_ID_o->GetBinContent(bin));
    h_dptXeta_ME_o->SetBinContent(bin, p_pt_ME_o->GetBinError(bin));
    h_dptXeta_MSO_o->SetBinContent(bin, p_pt_MSO_o->GetBinError(bin));
    h_dptXeta_ID_o->SetBinContent(bin, p_pt_ID_o->GetBinError(bin));

    h_rhoXeta_ME_o->SetBinContent(bin, p_rho_ME_o->GetBinContent(bin));
    h_rhoXeta_MSO_o->SetBinContent(bin, p_rho_MSO_o->GetBinContent(bin));
    h_drhoXeta_ME_o->SetBinContent(bin, p_rho_ME_o->GetBinError(bin));
    h_drhoXeta_MSO_o->SetBinContent(bin, p_rho_MSO_o->GetBinError(bin));

    h_dRXeta_ME_o->SetBinContent(bin, p_dR_ME_o->GetBinContent(bin));
    h_dRXeta_MSOE_o->SetBinContent(bin, p_dR_MSOE_o->GetBinContent(bin));
    h_ddRXeta_ME_o->SetBinContent(bin, p_dR_ME_o->GetBinError(bin));
    h_ddRXeta_MSOE_o->SetBinContent(bin, p_dR_MSOE_o->GetBinError(bin));

    h_ptXeta_ME_n->SetBinContent(bin, p_pt_ME_n->GetBinContent(bin));
    h_ptXeta_MSO_n->SetBinContent(bin, p_pt_MSO_n->GetBinContent(bin));
    h_ptXeta_ID_n->SetBinContent(bin, p_pt_ID_n->GetBinContent(bin));
    h_dptXeta_ME_n->SetBinContent(bin, p_pt_ME_n->GetBinError(bin));
    h_dptXeta_MSO_n->SetBinContent(bin, p_pt_MSO_n->GetBinError(bin));
    h_dptXeta_ID_n->SetBinContent(bin, p_pt_ID_n->GetBinError(bin));

    h_rhoXeta_ME_n->SetBinContent(bin, p_rho_ME_n->GetBinContent(bin));
    h_rhoXeta_MSO_n->SetBinContent(bin, p_rho_MSO_n->GetBinContent(bin));
    h_drhoXeta_ME_n->SetBinContent(bin, p_rho_ME_n->GetBinError(bin));
    h_drhoXeta_MSO_n->SetBinContent(bin, p_rho_MSO_n->GetBinError(bin));

    h_dRXeta_ME_n->SetBinContent(bin, p_dR_ME_n->GetBinContent(bin));
    h_dRXeta_MSOE_n->SetBinContent(bin, p_dR_MSOE_n->GetBinContent(bin));
    h_ddRXeta_ME_n->SetBinContent(bin, p_dR_ME_n->GetBinError(bin));
    h_ddRXeta_MSOE_n->SetBinContent(bin, p_dR_MSOE_n->GetBinError(bin));
  }

  // plotting for invariant mass
  // gaussian fits
  h_m_CB_o->Fit("gaus","","", 85, 95);
  h_m_ME_o->Fit("gaus","","", 85, 95);
  h_m_MSO_o->Fit("gaus","","", 79, 89);
  h_m_MSOE_o->Fit("gaus","","", 85, 95);
  h_m_ID_o->Fit("gaus","","", 85, 95);
  h_m_CB_n->Fit("gaus","","", 85, 95);
  h_m_ME_n->Fit("gaus","","", 85, 95);
  h_m_MSO_n->Fit("gaus","","", 79, 89);
  h_m_MSOE_n->Fit("gaus","","", 85, 95);
  h_m_ID_n->Fit("gaus","","", 85, 95);

  TF1* f_m_CB_o = h_m_CB_o->GetFunction("gaus");
  TF1* f_m_ME_o = h_m_ME_o->GetFunction("gaus");
  TF1* f_m_MSO_o = h_m_MSO_o->GetFunction("gaus");
  TF1* f_m_MSOE_o = h_m_MSOE_o->GetFunction("gaus");
  TF1* f_m_ID_o = h_m_ID_o->GetFunction("gaus");
  TF1* f_m_CB_n = h_m_CB_n->GetFunction("gaus");
  TF1* f_m_ME_n = h_m_ME_n->GetFunction("gaus");
  TF1* f_m_MSO_n = h_m_MSO_n->GetFunction("gaus");
  TF1* f_m_MSOE_n = h_m_MSOE_n->GetFunction("gaus");
  TF1* f_m_ID_n = h_m_ID_n->GetFunction("gaus");
  
  // top left legend
  TLegend *l_tl;
  // top left labels
  TPaveText *t_tl_CB, *t_tl_ME, *t_tl_MSO, *t_tl_MSOE, *t_tl_ID;

  // top right legend
  TLegend *l_tr;
  // top right labels
  TPaveText *t_tr_CB, *t_tr_ME, *t_tr_MSO, *t_tr_MSOE, *t_tr_ID;
  
  // top center labels
  TPaveText *t_tc_CB, *t_tc_ME, *t_tc_MSO, *t_tc_MSOE, *t_tc_ID;

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
  t_tl_CB->AddText("#it{ATLAS} #bf{Internal}");
  t_tl_CB->SetFillColor(0);
  t_tl_CB->SetLineColor(0);
  t_tl_CB->SetBorderSize(0);
  t_tl_CB->SetTextAlign(10);
  t_tl_ME = new TPaveText(*t_tl_CB);
  t_tl_MSO = new TPaveText(*t_tl_CB);
  t_tl_MSOE = new TPaveText(*t_tl_CB);
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
  
  t_tl_ME->AddText("#bf{ME Muons}");
  t_tl_ME->AddText("");
  t_tl_ME->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_ME_o->GetParameter(1),f_m_ME_o->GetParError(1)));
  t_tl_ME->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_ME_o->GetParameter(2),f_m_ME_o->GetParError(2)));
  t_tl_ME->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_ME_n->GetParameter(1),f_m_ME_n->GetParError(1)));
  t_tl_ME->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_ME_n->GetParameter(2),f_m_ME_n->GetParError(2)));
  
  t_tl_MSO->AddText("#bf{MS Muons}");
  t_tl_MSO->AddText("");
  t_tl_MSO->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_MSO_o->GetParameter(1),f_m_MSO_o->GetParError(1)));
  t_tl_MSO->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_MSO_o->GetParameter(2),f_m_MSO_o->GetParError(2)));
  t_tl_MSO->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_MSO_n->GetParameter(1),f_m_MSO_n->GetParError(1)));
  t_tl_MSO->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_MSO_n->GetParameter(2),f_m_MSO_n->GetParError(2)));
  
  t_tl_MSOE->AddText("#bf{MS Only Extrapolated Muons}");
  t_tl_MSOE->AddText("");
  t_tl_MSOE->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_MSOE_o->GetParameter(1),f_m_MSOE_o->GetParError(1)));
  t_tl_MSOE->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", oldMapName.c_str(),
                    f_m_MSOE_o->GetParameter(2),f_m_MSOE_o->GetParError(2)));
  t_tl_MSOE->AddText(Form("#bf{#mu (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_MSOE_n->GetParameter(1),f_m_MSOE_n->GetParError(1)));
  t_tl_MSOE->AddText(Form("#bf{#sigma (%s): %.2f #pm %.2f}", newMapName.c_str(),
                    f_m_MSOE_n->GetParameter(2),f_m_MSOE_n->GetParError(2)));
  
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
  t_tc_CB->AddText("#it{ATLAS} #bf{Internal}");
  t_tc_CB->SetFillColor(0);
  t_tc_CB->SetLineColor(0);
  t_tc_CB->SetBorderSize(0);
  t_tc_CB->SetTextAlign(30);
  t_tc_ME = new TPaveText(*t_tc_CB);
  t_tc_MSO = new TPaveText(*t_tc_CB);
  t_tc_MSOE = new TPaveText(*t_tc_CB);
  t_tc_ID = new TPaveText(*t_tc_CB);
  t_tc_CB->AddText("#bf{Combined Muons}");
  t_tc_ME->AddText("#bf{ME Muons}");
  t_tc_MSO->AddText("#bf{MS Muons}");
  t_tc_MSOE->AddText("#bf{MS Only Extrapolated Muons}");
  t_tc_ID->AddText("#bf{ID Muons}");
  
  // top right labels
  t_tr_CB = new TPaveText(.59,.65,.87,.74,"NDC");
  t_tr_CB->AddText("#it{ATLAS} #bf{Internal}");
  t_tr_CB->SetFillColor(0);
  t_tr_CB->SetLineColor(0);
  t_tr_CB->SetBorderSize(0);
  t_tr_CB->SetTextAlign(30);
  t_tr_ME = new TPaveText(*t_tr_CB);
  t_tr_MSO = new TPaveText(*t_tr_CB);
  t_tr_MSOE = new TPaveText(*t_tr_CB);
  t_tr_ID = new TPaveText(*t_tr_CB);
  t_tr_CB->AddText("#bf{Combined Muons}");
  t_tr_ME->AddText("#bf{ME Muons}");
  t_tr_MSO->AddText("#bf{MS Muons}");
  t_tr_MSOE->AddText("#bf{MS Only Extrapolated Muons}");
  t_tr_ID->AddText("#bf{ID Muons}");
  
  // vector of legends
  std::vector<TLegend*> *vl = new std::vector<TLegend*>();
  // m
  vl->push_back(l_tl); vl->push_back(l_tl); vl->push_back(l_tl); vl->push_back(l_tl); vl->push_back(l_tl);
  // pt leading
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); 
  // pt subleading
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); 
  // eta leading
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); 
  // eta subleading
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); 
  // author
  vl->push_back(l_tr); vl->push_back(l_tr);
  // quality
  vl->push_back(l_tr);
  // dR leading
  vl->push_back(l_tr); vl->push_back(l_tr);
  // dR subleading
  vl->push_back(l_tr); vl->push_back(l_tr);
  // rho leading
  vl->push_back(l_tr); vl->push_back(l_tr);
  // rho subleading
  vl->push_back(l_tr); vl->push_back(l_tr);
  // ptXeta
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr);
  // dptXeta
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr);
  // rhoXeta
  vl->push_back(l_tr); vl->push_back(l_tr);
  // drhoXeta
  vl->push_back(l_tr); vl->push_back(l_tr);
  // dRXeta
  vl->push_back(l_tr); vl->push_back(l_tr);
  // ddRXeta
  vl->push_back(l_tr); vl->push_back(l_tr);


  // vector of tpavetext labels
  std::vector<TPaveText*> *vt = new std::vector<TPaveText*>();
  // m
  vt->push_back(t_tl_CB); vt->push_back(t_tl_ME); vt->push_back(t_tl_MSO); vt->push_back(t_tl_MSOE); vt->push_back(t_tl_ID);
  // pt leading
  vt->push_back(t_tr_CB); vt->push_back(t_tr_ME); vt->push_back(t_tr_MSO); vt->push_back(t_tr_MSOE); vt->push_back(t_tr_ID);  
  // pt subleading
  vt->push_back(t_tr_CB); vt->push_back(t_tr_ME); vt->push_back(t_tr_MSO); vt->push_back(t_tr_MSOE); vt->push_back(t_tr_ID);  
  // eta leading
  vt->push_back(t_tc_CB); vt->push_back(t_tc_ME); vt->push_back(t_tc_MSO); vt->push_back(t_tc_MSOE); vt->push_back(t_tc_ID);  
  // eta subleading
  vt->push_back(t_tc_CB); vt->push_back(t_tc_ME); vt->push_back(t_tc_MSO); vt->push_back(t_tc_MSOE); vt->push_back(t_tc_ID);  
  // author
  vt->push_back(nullptr); vt->push_back(nullptr);
  // quality
  vt->push_back(nullptr);
  // dR leading
  vt->push_back(nullptr); vt->push_back(nullptr);
  // dR subleading
  vt->push_back(nullptr); vt->push_back(nullptr);
  // rho leading
  vt->push_back(nullptr); vt->push_back(nullptr);
  // rho subleading
  vt->push_back(nullptr); vt->push_back(nullptr);
  // ptXeta
  vl->push_back(l_tr_ME); vl->push_back(l_tr_MSO); vl->push_back(l_tr_ID);
  // dptXeta
  vl->push_back(l_tr_ME); vl->push_back(l_tr_MSO); vl->push_back(l_tr_ID);
  // rhoXeta
  vl->push_back(); vl->push_back();
  // drhoXeta
  vl->push_back(); vl->push_back();
  // dRXeta
  vl->push_back(); vl->push_back();
  // ddRXeta
  vl->push_back(); vl->push_back();

  // set histogram max values
  int n_m_MSO_o = h_m_MSO_o->GetEntries();
  int n_m_MSO_n = h_m_MSO_n->GetEntries();
  double max = 0, n_min = min(n_m_MSO_o, n_m_MSO_n);
  for (int i = 0; i < vh_o->size(); i++) {
    if (n_m_MSO_o > 0 and n_m_MSO_n > 0) {
      vh_o->at(i)->Scale(n_min/n_m_MSO_o);
      vh_n->at(i)->Scale(n_min/n_m_MSO_n);
    }

    max = 0;
    max = std::max(vh_o->at(i)->GetMaximum(), vh_n->at(i)->GetMaximum());
    max *= maxFactor->at(i);

    vh_o->at(i)->SetMaximum(max);
    vh_n->at(i)->SetMaximum(max);
    vh_o->at(i)->SetMinimum(0);
    vh_n->at(i)->SetMinimum(0);
  }
 
  // plotting
  ratioCan->cd();
  ratioCan->Clear();
  for (int i = 0; i < vh_o->size(); i++) {
    PlottingHelpers::MakeRatioPlot(ratioCan, vh_o->at(i), vh_n->at(i), vl->at(i), vt->at(i), 
                  oldMapName, newMapName, file_prefix);
  }
  ratioCan->Close();

  cout << oldMapName << endl
       << "-------------------------------------------------" << endl
       << "Total number of muon pairs: " << nEvents_o << endl
       << "Pass GRL: " << nPassGRL_o << endl
       << "Pass ID cuts: " << nPassIDcuts_o << endl
       << "Pass medium quality requirement: " << nPassMedium_o << endl
       << "Number of MS invariant mass measurements: " << n_m_MSO_o << endl << endl
       << newMapName << endl
       << "-------------------------------------------------" << endl
       << "Total number of muon pairs: " << nEvents_n << endl
       << "Pass GRL: " << nPassGRL_n << endl
       << "Pass ID cuts: " << nPassIDcuts_n << endl
       << "Pass medium quality requirement: " << nPassMedium_n << endl
       << "Number of MS invariant mass measurements: " << n_m_MSO_n << endl << endl;
 
  for (int i = 0; i < vh_o->size(); i++) {
    delete vh_o->at(i);
    delete vh_n->at(i);
  }
  delete vh_o;
  delete vh_n;
  delete maxFactor;
  delete l_tl;
  delete t_tl_CB; 
  delete t_tl_ME; 
  delete t_tl_MSO; 
  delete t_tl_MSOE; 
  delete t_tl_ID;
  delete t_tr_CB; 
  delete t_tr_ME; 
  delete t_tr_MSO; 
  delete t_tr_MSOE; 
  delete t_tr_ID;
  delete t_tc_CB; 
  delete t_tc_ME; 
  delete t_tc_MSO; 
  delete t_tc_MSOE; 
  delete t_tc_ID;
  delete vt;
  delete vl;
}

/**************************************************************************************************/


void GeneratePlots() {
  // declare and print file prefix
  // RunI vs 2016
  string file_prefix = "Zmumu.00340072.noIDalign.RunIvs2016";
  string oldFileName = "ntuples/ntuple_RunI_noIDalign.root";
  string newFileName = "ntuples/ntuple_2016_noIDalign.root";
  // AOD vs RunI
  //string file_prefix = "Zmumu.00340072.noIDalign.AODvsRunI";
  //string oldFileName = "ntuples/ntuple_AOD_noIDalign.root";
  //string newFileName = "ntuples/ntuple_RunI_noIDalign.root";

  cout << "Old map reconstruction: " << oldFileName << endl;
  cout << "New map reconstruction: " << newFileName << endl;
  cout << "Preparing output with prefix " << file_prefix.c_str() << endl; 
  
  FillPlots(file_prefix, oldFileName, newFileName);
}
