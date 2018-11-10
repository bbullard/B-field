#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
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
#include <TLatex.h>
#include <fstream>

using namespace std;

/*
 * Function for creating a ratio plot for histograms h_o and h_n, corresponding to the same quantity
 * plotted with the same binning between the old and new b-field reconstruction. Also received a
 * legend and label to display on the plot, as well as the name of the old map and new map to be
 * displayed on the ratio plot y-axis. Prefix is for flagging data set comparisons 
 * e.g. prefix = "user.00339396.Run1_2016_"
 */
void MakeRatioPlot(TCanvas *c, TH1F* h_o, TH1F* h_n, TLegend *l, TPaveText *p, 
                  string oldMapName, string newMapName, string file_prefix="") {
 
  string name = h_o->GetName();
  name = name.substr(2,name.length()-4);

  if (!p) {
    p = new TPaveText(.59,.69,.87,.74,"NDC");
    p->AddText("#it{ATLAS} #bf{Internal}");
    p->SetFillColor(0);
    p->SetLineColor(0);
    p->SetBorderSize(0);
    p->SetTextAlign(30);
  }

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
  p->Draw();
  plotPad.RedrawAxis();
  
  TH1F* ratio = new TH1F(*h_n);
  ratio->Divide(h_o);
  ratio->GetYaxis()->SetTitle(Form("%s/%s", newMapName.c_str(), oldMapName.c_str()));
  ratio->SetLineColor(kBlack);
  ratio->SetMinimum(0.89);
  ratio->SetMaximum(1.12);
  ratio->SetMarkerStyle(20);
  ratio->SetMarkerColor(kBlack);
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

  c->Print(Form("plots/%s_%s_Ratio_%s_vs_%s.png", file_prefix.c_str(), name.c_str(), oldMapName.c_str(), newMapName.c_str()));
  delete ratio;
  
  ratioPad.Close();
  plotPad.Close();
}

/**************************************************************************************************/

/*
 * Function for creating a perc-diff plot for histograms h_o and h_n, corresponding to the same quantity
 * plotted with the same binning between the old and new b-field reconstruction. Also received a
 * legend and label to display on the plot, as well as the name of the old map and new map to be
 * displayed on the ratio plot y-axis. Prefix is for flagging data set comparisons 
 * e.g. prefix = "user.00339396.Run1_2016_"
 */
void MakePercDiffPlot(TCanvas *c, TH1F* h_o, TH1F* h_n, TLegend *l, TPaveText *p, 
                  string oldMapName, string newMapName, string file_prefix="") {
 
  string name = h_o->GetName();
  name = name.substr(2,name.length()-4);

  if (!p) {
    p = new TPaveText(.59,.69,.87,.74,"NDC");
    p->AddText("#it{ATLAS} #bf{Internal}");
    p->SetFillColor(0);
    p->SetLineColor(0);
    p->SetBorderSize(0);
    p->SetTextAlign(30);
  }

  c->cd();
  TPad plotPad("plotPad", "plotPad", 0, 0.3, 1, 1.0);
  plotPad.SetBottomMargin(0); 
  plotPad.SetTickx();
  plotPad.SetTicky();                 
  plotPad.Draw();
  plotPad.cd();
  h_o->Draw("pl");
  h_n->Draw("pl same");
  l->Draw(); 
  p->Draw();
  plotPad.RedrawAxis();
  
  TH1F* pdiff = new TH1F(*h_n);
  pdiff->Add(h_o, -1);
  pdiff->Divide(h_o);
  pdiff->GetYaxis()->SetTitle(Form("(%s-%s)/%s", newMapName.c_str(), oldMapName.c_str(), oldMapName.c_str()));
  pdiff->SetLineColor(kBlack);
  pdiff->SetMinimum(-0.5);
  pdiff->SetMaximum(0.5);
  pdiff->SetMarkerStyle(20);
  pdiff->SetMarkerColor(kBlack);
  pdiff->GetXaxis()->SetTitleSize(0.15);//.2
  pdiff->GetXaxis()->SetLabelSize(0.12);//.15
  pdiff->GetXaxis()->SetTitleOffset(0.95);
  pdiff->GetYaxis()->SetTitleSize(0.1);//.15
  pdiff->GetYaxis()->SetLabelSize(0.11);//.15
  pdiff->GetYaxis()->SetTitleOffset(0.45);
  pdiff->GetYaxis()->SetNdivisions(505);//503  
 
  c->cd();
  TPad pdiffPad("pdiffPad", "pdiffPad", 0, 0.05, 1, 0.3);
  pdiffPad.SetTopMargin(0);
  pdiffPad.SetBottomMargin(0.4);  
  pdiffPad.SetTickx();
  pdiffPad.SetTicky(); 
  pdiffPad.SetGridy();
  pdiffPad.Draw();
  pdiffPad.cd();
  pdiff->Draw("p");
  pdiffPad.RedrawAxis();
  c->cd();

  newMapName.erase (std::remove (newMapName.begin(), newMapName.end(), ' '), newMapName.end());
  oldMapName.erase (std::remove (oldMapName.begin(), oldMapName.end(), ' '), oldMapName.end());

  c->Print(Form("plots/%s_%s_PercDiff_%s_vs_%s.png", file_prefix.c_str(), name.c_str(), oldMapName.c_str(), newMapName.c_str()));
  delete pdiff;
  
  pdiffPad.Close();
  plotPad.Close();
}

/**************************************************************************************************/
/*
 * Function for creating plotting profiles p_o and p_n in eta, corresponding to the same quantity
 * plotted with the same binning between the old and new b-field reconstruction. Also receives the 
 * name of the old map and new map to be used for print out. Prefix is for flagging data set 
 * comparisons e.g. prefix = "user.00339396.Run1_2016_"
 */
void MakeProfile(TCanvas *c, TProfile *p_o, TProfile *p_n, string oldMapName, string newMapName, 
                  string file_prefix="") {
  gStyle->SetHatchesSpacing(1.5);
  string name = p_o->GetName();
  string type = name.substr(name.length()-4,2);
  name = name.substr(2,name.length());
  size_t pos = name.find("_");
  name = name.substr(0,pos);

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
  p_o->GetYaxis()->SetTitleSize(0.04);
  p_o->GetYaxis()->SetLabelSize(0.033);
  p_o->GetYaxis()->SetTitleOffset(1.28);
  p_o->GetYaxis()->SetNdivisions(508);  
 
  p_o->GetXaxis()->SetNdivisions(508);
  p_o->GetXaxis()->SetTitleSize(0.05);
  p_o->GetXaxis()->SetTitleOffset(0.95);
  p_o->GetXaxis()->SetLabelSize(0.035); 
  
  p_n->SetLineColor(kAzure-4);
  p_n->SetLineWidth(2);
  p_n->SetMarkerColor(kAzure-2);
  p_n->SetMarkerStyle(20);
  p_n->SetFillColor(38);
  p_n->SetFillStyle(3144);
 
  TLegend *l = new TLegend(.67,.77,.87,.87);
  l->AddEntry(p_o, Form("%s Map",oldMapName.c_str()), "fp");
  l->AddEntry(p_n, Form("%s Map",newMapName.c_str()), "fp");
 
  TPaveText *p = new TPaveText(.43,.77,.66,.87,"NDC");
  p->AddText("#it{ATLAS} #bf{Internal}");
  p->AddText("#bf{RMS Bands}");
  p->SetFillColor(0);
  p->SetLineColor(0);
  p->SetBorderSize(0);
  p->SetTextAlign(32);

  TPaveText *p2 = new TPaveText(.14,.83,.43,.87, "NDC");
  if (type.compare("ME")==0) p2->AddText("#bf{MS Extrapolated Muons}");
  if (type.compare("MS")==0) p2->AddText("#bf{MS Only Muons}");
  if (type.compare("ID")==0) p2->AddText("#bf{ID Muons}");
  if (type.compare("CB")==0) p2->AddText("#bf{CB Muons}");
  p2->SetFillColor(0);
  p2->SetLineColor(0);
  p2->SetBorderSize(0);
  p2->SetTextAlign(32);
  
  bool drawP2 = true;
  if (name.compare("rho")==0) drawP2 = false;
  
  c->cd();
  TPad pad("minPad", "minPad", 0, 0.0, 1, 1);
  pad.SetBottomMargin(0.2);
  pad.SetTickx();
  pad.SetTicky();
  pad.Draw();
  pad.cd();
  p_o->Draw("e2 9");
  p_n->Draw("e2 9 same");
  l->Draw();
  p->Draw();
  if (drawP2) p2->Draw();
  pad.RedrawAxis();
  c->cd();

  newMapName.erase (std::remove (newMapName.begin(), newMapName.end(), ' '), newMapName.end());
  oldMapName.erase (std::remove (oldMapName.begin(), oldMapName.end(), ' '), oldMapName.end());
 
  c->Print(Form("plots/%s_%s_%s_Profile_%s_vs_%s.png", file_prefix.c_str(), name.c_str(), type.c_str(), oldMapName.c_str(), newMapName.c_str()));

  delete l;
}

/**************************************************************************************************/

/*
 * Function for plotting charge-separated profiles p_o and p_n in eta, corresponding to the same 
 * quantity plotted with the same binning between the old and new b-field reconstruction. Also 
 * receives the name of the old map and new map to be used for print out. Prefix is for flagging 
 * data set comparisons e.g. prefix = "user.00339396.Run1_2016_"
 */
void MakeCSProfile(TCanvas *c, TProfile *p_p_o, TProfile *p_m_o, TProfile *p_p_n, TProfile *p_m_n,
                  string oldMapName, string newMapName, string file_prefix="") {
  gStyle->SetHatchesSpacing(1.5);
  string name = p_p_o->GetName();
  string type = name.substr(name.length()-4,2);
  name = name.substr(2,name.length());
  size_t pos = name.find("_");
  name = name.substr(0,pos);

  p_p_o->SetLineColor(12);
  p_p_o->SetLineWidth(2);
  p_p_o->SetMarkerColor(kBlack);
  p_p_o->SetMarkerStyle(20);
  p_p_o->SetFillColor(17);
  p_p_o->SetFillStyle(1001);

  double maxt = p_p_o->GetMaximum();
  double mint = p_p_o->GetMinimum();
  p_p_o->SetMaximum(maxt+(maxt-mint)*.3);
  p_p_o->SetMinimum(mint);
  p_p_o->GetYaxis()->SetNdivisions(204);
  p_p_o->GetYaxis()->SetTitleSize(0.054);
  p_p_o->GetYaxis()->SetLabelSize(0.054);
  p_p_o->GetYaxis()->SetTitleOffset(0.85);
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
  p_m_o->GetYaxis()->SetTitleSize(0.06);
  p_m_o->GetYaxis()->SetLabelSize(0.06);
  p_m_o->GetYaxis()->SetTitleOffset(0.77);
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
 
  TLegend *l = new TLegend(.67,.77,.86,.97);
  l->AddEntry(p_p_o, Form("%s Map",oldMapName.c_str()), "fp");
  l->AddEntry(p_p_n, Form("%s Map",newMapName.c_str()), "fp");
 
  TPaveText *p = new TPaveText(.43,.77,.66,.97,"NDC");
  p->AddText("#it{ATLAS} #bf{Internal}");
  p->AddText("#bf{RMS Bands}");
  p->SetFillColor(0);
  p->SetLineColor(0);
  p->SetBorderSize(0);
  p->SetTextAlign(32);

  TPaveText *p2 = new TPaveText(.15,.87,.43,.97, "NDC");
  if (type.compare("ME")==0) p2->AddText("#bf{MS Extrapolated Muons}");
  if (type.compare("MS")==0) p2->AddText("#bf{MS Only Muons}");
  if (type.compare("ID")==0) p2->AddText("#bf{ID Muons}");
  if (type.compare("CB")==0) p2->AddText("#bf{CB Muons}");
  p2->SetFillColor(0);
  p2->SetLineColor(0);
  p2->SetBorderSize(0);
  p2->SetTextAlign(32);
  
  bool drawP2 = true;
  if (name.compare("rho")==0) drawP2 = false;
 
  c->cd();
  TPad plusPad("plusPad", "plusPad", 0, 0.48, 1, 1.0);
  plusPad.SetBottomMargin(0);
  plusPad.SetTopMargin(0);
  plusPad.SetTickx();
  plusPad.SetTicky();
  plusPad.Draw();
  plusPad.cd();
  p_p_o->Draw("e2 9");
  p_p_n->Draw("e2 9 same");
  l->Draw();
  p->Draw();
  if (drawP2) p2->Draw();
  plusPad.RedrawAxis();

  c->cd();
  TPad minPad("minPad", "minPad", 0, 0.0, 1, 0.48);
  minPad.SetTopMargin(0);
  minPad.SetBottomMargin(0.2);
  minPad.SetTickx();
  minPad.SetTicky();
  minPad.Draw();
  minPad.cd();
  p_m_o->Draw("e2 9");
  p_m_n->Draw("e2 9 same");
  minPad.RedrawAxis();
  c->cd();

  newMapName.erase (std::remove (newMapName.begin(), newMapName.end(), ' '), newMapName.end());
  oldMapName.erase (std::remove (oldMapName.begin(), oldMapName.end(), ' '), oldMapName.end());
 
  c->Print(Form("plots/%s_%s_%s_ChargeSeparatedProfile_%s_vs_%s.png", file_prefix.c_str(), name.c_str(), type.c_str(), oldMapName.c_str(), newMapName.c_str()));

  delete l;
}

/**************************************************************************************************/

void MakeProfilePlots(string file_prefix, string oldFileName, string newFileName) {
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetPadBorderSize(2);

  const int nBins = 50;

  // create profiles and format
  // rho charge separated
  TProfile* p_rho_plus_ME_o = new TProfile("p_rho_plus_ME_o","",nBins,-2.5,2.5,"s");
  p_rho_plus_ME_o->SetMaximum(.4); 
  p_rho_plus_ME_o->SetMinimum(-.4); 
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
 
  // rho
  TProfile* p_rho_ME_o = (TProfile*)p_rho_plus_ME_o->Clone("p_rho_ME_o");
  p_rho_ME_o->SetTitle(";#eta_{ID};#rho_{ME}"); 
  TProfile* p_rho_MS_o = (TProfile*)p_rho_plus_ME_o->Clone("p_rho_MS_o");
  p_rho_MS_o->SetTitle(";#eta_{ID};#rho_{MS}"); 

  TProfile* p_rho_ME_n = (TProfile*)p_rho_plus_ME_o->Clone("p_rho_ME_n");
  TProfile* p_rho_MS_n = (TProfile*)p_rho_plus_ME_o->Clone("p_rho_MS_n");

  // <rho> histogram binned in eta
  TH1F* h_rho_plus_ME_o = new TH1F("h_rho_plus_ME_o","",nBins,-2.5,2.5);
  h_rho_plus_ME_o->SetTitle(";#eta_{ID};<#rho>_{ME} (#mu^{+})");
  TH1F* h_rho_min_ME_o = (TH1F*)h_rho_plus_ME_o->Clone("h_rho_min_ME_o");
  h_rho_min_ME_o->SetTitle(";#eta_{ID};<#rho>_{ME} (#mu^{-})");
  TH1F* h_rho_plus_MS_o = (TH1F*)h_rho_plus_ME_o->Clone("h_rho_plus_MS_o");
  h_rho_plus_MS_o->SetTitle(";#eta_{ID};<#rho>_{MS} (#mu^{+})");
  TH1F* h_rho_min_MS_o = (TH1F*)h_rho_plus_ME_o->Clone("h_rho_min_MS_o");
  h_rho_min_MS_o->SetTitle(";#eta_{ID};<#rho>_{MS} (#mu^{-})");
  
  TH1F* h_rho_plus_ME_n = (TH1F*)h_rho_plus_ME_o->Clone("h_rho_plus_ME_n");
  TH1F* h_rho_min_ME_n = (TH1F*)h_rho_plus_ME_o->Clone("h_rho_min_ME_n");
  TH1F* h_rho_plus_MS_n = (TH1F*)h_rho_plus_ME_o->Clone("h_rho_plus_MS_n");
  TH1F* h_rho_min_MS_n = (TH1F*)h_rho_plus_ME_o->Clone("h_rho_min_MS_n");
  
  TH1F* h_rho_ME_o = (TH1F*)h_rho_plus_ME_o->Clone("h_rho_ME_o");
  h_rho_ME_o->SetTitle(";#eta_{ID};<#rho>_{ME}"); 
  TH1F* h_rho_MS_o = (TH1F*)h_rho_plus_ME_o->Clone("h_rho_MS_o");
  h_rho_MS_o->SetTitle(";#eta_{ID};<#rho>_{MS}"); 

  TH1F* h_rho_ME_n = (TH1F*)h_rho_plus_ME_o->Clone("h_rho_ME_n");
  TH1F* h_rho_MS_n = (TH1F*)h_rho_plus_ME_o->Clone("h_rho_MS_n");
  
  // rho rms histogram binned in eta
  TH1F* h_rhorms_plus_ME_o = new TH1F("h_rhorms_plus_ME_o","",nBins,-2.5,2.5);
  h_rhorms_plus_ME_o->SetTitle(";#eta_{ID};#rho_{ME} RMS (#mu^{+})");
  TH1F* h_rhorms_min_ME_o = (TH1F*)h_rhorms_plus_ME_o->Clone("h_rhorms_min_ME_o");
  h_rhorms_min_ME_o->SetTitle(";#eta_{ID};#rho_{ME} RMS (#mu^{-})");
  TH1F* h_rhorms_plus_MS_o = (TH1F*)h_rhorms_plus_ME_o->Clone("h_rhorms_plus_MS_o");
  h_rhorms_plus_MS_o->SetTitle(";#eta_{ID};#rho_{MS} RMS (#mu^{+})");
  TH1F* h_rhorms_min_MS_o = (TH1F*)h_rhorms_plus_ME_o->Clone("h_rhorms_min_MS_o");
  h_rhorms_min_MS_o->SetTitle(";#eta_{ID};#rho_{MS} RMS (#mu^{-})");
  
  TH1F* h_rhorms_plus_ME_n = (TH1F*)h_rhorms_plus_ME_o->Clone("h_rhorms_plus_ME_n");
  TH1F* h_rhorms_min_ME_n = (TH1F*)h_rhorms_plus_ME_o->Clone("h_rhorms_min_ME_n");
  TH1F* h_rhorms_plus_MS_n = (TH1F*)h_rhorms_plus_ME_o->Clone("h_rhorms_plus_MS_n");
  TH1F* h_rhorms_min_MS_n = (TH1F*)h_rhorms_plus_ME_o->Clone("h_rhorms_min_MS_n");
  
  TH1F* h_rhorms_ME_o = (TH1F*)h_rhorms_plus_ME_o->Clone("h_rhorms_ME_o");
  h_rhorms_ME_o->SetTitle(";#eta_{ID};#rho_{ME} RMS"); 
  TH1F* h_rhorms_MS_o = (TH1F*)h_rhorms_plus_ME_o->Clone("h_rhorms_MS_o");
  h_rhorms_MS_o->SetTitle(";#eta_{ID};#rho_{MS} RMS"); 

  TH1F* h_rhorms_ME_n = (TH1F*)h_rhorms_plus_ME_o->Clone("h_rhorms_ME_n");
  TH1F* h_rhorms_MS_n = (TH1F*)h_rhorms_plus_ME_o->Clone("h_rhorms_MS_n");
  
  // put rho histograms into vectors
  vector<TH1F*> *vrho_o = new vector<TH1F*>();
  vector<TH1F*> *vrho_n = new vector<TH1F*>();
  
  vrho_o->push_back(h_rho_plus_ME_o);
  vrho_o->push_back(h_rho_min_ME_o);
  vrho_o->push_back(h_rho_ME_o);
  vrho_o->push_back(h_rho_plus_MS_o);
  vrho_o->push_back(h_rho_min_MS_o);
  vrho_o->push_back(h_rho_MS_o);
  vrho_o->push_back(h_rhorms_plus_ME_o);
  vrho_o->push_back(h_rhorms_min_ME_o);
  vrho_o->push_back(h_rhorms_ME_o);
  vrho_o->push_back(h_rhorms_plus_MS_o);
  vrho_o->push_back(h_rhorms_min_MS_o);
  vrho_o->push_back(h_rhorms_MS_o);

  vrho_n->push_back(h_rho_plus_ME_n);
  vrho_n->push_back(h_rho_min_ME_n);
  vrho_n->push_back(h_rho_ME_n);
  vrho_n->push_back(h_rho_plus_MS_n);
  vrho_n->push_back(h_rho_min_MS_n);
  vrho_n->push_back(h_rho_MS_n);
  vrho_n->push_back(h_rhorms_plus_ME_n);
  vrho_n->push_back(h_rhorms_min_ME_n);
  vrho_n->push_back(h_rhorms_ME_n);
  vrho_n->push_back(h_rhorms_plus_MS_n);
  vrho_n->push_back(h_rhorms_min_MS_n);
  vrho_n->push_back(h_rhorms_MS_n);
  
  // pt
  // plus
  TProfile* p_pt_plus_CB_o = new TProfile("p_pt_plus_CB_o","",nBins,-2.7,2.7,"s");
  p_pt_plus_CB_o->SetTitle(";#eta;P_{T} (#mu^{+}) (GeV)"); 
  p_pt_plus_CB_o->SetMaximum(80);
  p_pt_plus_CB_o->SetMinimum(0); 
  TProfile* p_pt_plus_ME_o = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_plus_ME_o");
  TProfile* p_pt_plus_MS_o = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_plus_MS_o");
  TProfile* p_pt_plus_ID_o = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_plus_ID_o");

  TProfile* p_pt_plus_CB_n = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_plus_CB_n");
  TProfile* p_pt_plus_ME_n = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_plus_ME_n");
  TProfile* p_pt_plus_MS_n = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_plus_MS_n");
  TProfile* p_pt_plus_ID_n = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_plus_ID_n");
  
  // minus
  TProfile* p_pt_min_CB_o = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_min_CB_o");
  p_pt_min_CB_o->GetYaxis()->SetTitle("P_{T} (#mu^{-}) (GeV)");
  TProfile* p_pt_min_ME_o = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_min_ME_o");
  p_pt_min_ME_o->GetYaxis()->SetTitle("P_{T} (#mu^{-}) (GeV)");
  TProfile* p_pt_min_MS_o = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_min_MS_o");
  p_pt_min_MS_o->GetYaxis()->SetTitle("P_{T} (#mu^{-}) (GeV)");
  TProfile* p_pt_min_ID_o = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_min_ID_o");
  p_pt_min_ID_o->GetYaxis()->SetTitle("P_{T} (#mu^{-}) (GeV)");

  TProfile* p_pt_min_CB_n = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_min_CB_n");
  TProfile* p_pt_min_ME_n = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_min_ME_n");
  TProfile* p_pt_min_MS_n = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_min_MS_n");
  TProfile* p_pt_min_ID_n = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_min_ID_n");
 
  // no charge
  TProfile* p_pt_CB_o = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_CB_o"); 
  p_pt_CB_o->GetYaxis()->SetTitle("P_{T} (GeV)");
  TProfile* p_pt_ME_o = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_ME_o");
  p_pt_ME_o->GetYaxis()->SetTitle("P_{T} (GeV)");
  TProfile* p_pt_MS_o = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_MS_o");
  p_pt_MS_o->GetYaxis()->SetTitle("P_{T} (GeV)");
  TProfile* p_pt_ID_o = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_ID_o");
  p_pt_ID_o->GetYaxis()->SetTitle("P_{T} (GeV)");

  TProfile* p_pt_CB_n = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_CB_n");
  TProfile* p_pt_ME_n = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_ME_n");
  TProfile* p_pt_MS_n = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_MS_n");
  TProfile* p_pt_ID_n = (TProfile*)p_pt_plus_CB_o->Clone("p_pt_ID_n");

  // chi2/dof
  // plus
  TProfile* p_rchi2_plus_CB_o = new TProfile("p_rchi2_plus_CB_o","",nBins,-2.7,2.7,"s");
  p_rchi2_plus_CB_o->SetTitle(";#eta;#chi^{2}/D.o.F. (#mu^{+})"); 
  TProfile* p_rchi2_plus_ME_o = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_plus_ME_o");
  TProfile* p_rchi2_plus_MS_o = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_plus_MS_o");
  TProfile* p_rchi2_plus_ID_o = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_plus_ID_o");

  TProfile* p_rchi2_plus_CB_n = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_plus_CB_n");
  TProfile* p_rchi2_plus_ME_n = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_plus_ME_n");
  TProfile* p_rchi2_plus_MS_n = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_plus_MS_n");
  TProfile* p_rchi2_plus_ID_n = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_plus_ID_n");
  
  // minus
  TProfile* p_rchi2_min_CB_o = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_min_CB_o");
  p_rchi2_min_CB_o->SetTitle(";#eta;#chi^{2}/D.o.F. (#mu^{-})"); 
  TProfile* p_rchi2_min_ME_o = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_min_ME_o");
  p_rchi2_min_ME_o->SetTitle(";#eta;#chi^{2}/D.o.F. (#mu^{-})"); 
  TProfile* p_rchi2_min_MS_o = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_min_MS_o");
  p_rchi2_min_MS_o->SetTitle(";#eta;#chi^{2}/D.o.F. (#mu^{-})"); 
  TProfile* p_rchi2_min_ID_o = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_min_ID_o");
  p_rchi2_min_ID_o->SetTitle(";#eta;#chi^{2}/D.o.F. (#mu^{-})"); 

  TProfile* p_rchi2_min_CB_n = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_min_CB_n");
  TProfile* p_rchi2_min_ME_n = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_min_ME_n");
  TProfile* p_rchi2_min_MS_n = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_min_MS_n");
  TProfile* p_rchi2_min_ID_n = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_min_ID_n");
 
  // chi2/dof no charge
  TProfile* p_rchi2_CB_o = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_CB_o"); 
  p_rchi2_CB_o->SetTitle(";#eta;#chi^{2}/D.o.F."); 
  TProfile* p_rchi2_ME_o = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_ME_o");
  p_rchi2_ME_o->SetTitle(";#eta;#chi^{2}/D.o.F."); 
  TProfile* p_rchi2_MS_o = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_MS_o");
  p_rchi2_MS_o->SetTitle(";#eta;#chi^{2}/D.o.F."); 
  TProfile* p_rchi2_ID_o = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_ID_o");
  p_rchi2_ID_o->SetTitle(";#eta;#chi^{2}/D.o.F."); 

  TProfile* p_rchi2_CB_n = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_CB_n");
  TProfile* p_rchi2_ME_n = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_ME_n");
  TProfile* p_rchi2_MS_n = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_MS_n");
  TProfile* p_rchi2_ID_n = (TProfile*)p_rchi2_plus_CB_o->Clone("p_rchi2_ID_n");
 
  // set indivudual plot ranges
  p_rchi2_plus_CB_o->SetMaximum(10);
  p_rchi2_plus_CB_o->SetMinimum(-3); 
  p_rchi2_min_CB_o->SetMaximum(10);
  p_rchi2_min_CB_o->SetMinimum(-3); 
  p_rchi2_CB_o->SetMaximum(10);
  p_rchi2_CB_o->SetMinimum(-3);
 
  p_rchi2_plus_ID_o->SetMaximum(2);
  p_rchi2_plus_ID_o->SetMinimum(0); 
  p_rchi2_min_ID_o->SetMaximum(2);
  p_rchi2_min_ID_o->SetMinimum(0); 
  p_rchi2_ID_o->SetMaximum(2);
  p_rchi2_ID_o->SetMinimum(0);
  
  p_rchi2_plus_ME_o->SetMaximum(10);
  p_rchi2_plus_ME_o->SetMinimum(-3); 
  p_rchi2_min_ME_o->SetMaximum(10);
  p_rchi2_min_ME_o->SetMinimum(-3); 
  p_rchi2_ME_o->SetMaximum(10);
  p_rchi2_ME_o->SetMinimum(-3); 
  
  p_rchi2_plus_MS_o->SetMaximum(2.5);
  p_rchi2_plus_MS_o->SetMinimum(0); 
  p_rchi2_min_MS_o->SetMaximum(2.5);
  p_rchi2_min_MS_o->SetMinimum(0); 
  p_rchi2_MS_o->SetMaximum(2.5);
  p_rchi2_MS_o->SetMinimum(0); 
  
  // q/p sigificance charge separated
  TProfile* p_qpsig_plus_ME_o = new TProfile("p_qpsig_plus_ME_o","",nBins,-2.5,2.5,"s");
  p_qpsig_plus_ME_o->SetMaximum(3.1); 
  p_qpsig_plus_ME_o->SetMinimum(-0.5); 
  p_qpsig_plus_ME_o->SetTitle(";#eta_{ID};q/p Sigificance (ME-ID) (#mu^{+})");
  TProfile* p_qpsig_min_ME_o = (TProfile*)p_qpsig_plus_ME_o->Clone("p_qpsig_min_ME_o");
  p_qpsig_min_ME_o->SetTitle(";#eta_{ID};q/p Sigificance (ME-ID) (#mu^{-})");
  TProfile* p_qpsig_plus_MS_o = (TProfile*)p_qpsig_plus_ME_o->Clone("p_qpsig_plus_MS_o");
  p_qpsig_plus_MS_o->SetTitle(";#eta_{ID};q/p Sigificance (MS-ID) (#mu^{+})");
  TProfile* p_qpsig_min_MS_o = (TProfile*)p_qpsig_plus_ME_o->Clone("p_qpsig_min_MS_o");
  p_qpsig_min_MS_o->SetTitle(";#eta_{ID};q/p Sigificance (MS-ID) (#mu^{-})");

  TProfile* p_qpsig_plus_ME_n = (TProfile*)p_qpsig_plus_ME_o->Clone("p_qpsig_plus_ME_n");
  TProfile* p_qpsig_min_ME_n = (TProfile*)p_qpsig_plus_ME_o->Clone("p_qpsig_min_ME_n");
  TProfile* p_qpsig_plus_MS_n = (TProfile*)p_qpsig_plus_ME_o->Clone("p_qpsig_plus_MS_n");
  TProfile* p_qpsig_min_MS_n = (TProfile*)p_qpsig_plus_ME_o->Clone("p_qpsig_min_MS_n");
 
  // q/p
  TProfile* p_qpsig_ME_o = (TProfile*)p_qpsig_plus_ME_o->Clone("p_qpsig_ME_o");
  p_qpsig_ME_o->SetTitle(";#eta_{ID};q/p Sigificance (ME-ID)"); 
  TProfile* p_qpsig_MS_o = (TProfile*)p_qpsig_plus_ME_o->Clone("p_qpsig_MS_o");
  p_qpsig_MS_o->SetTitle(";#eta_{ID};q/p Sigificance (MS-ID)"); 

  TProfile* p_qpsig_ME_n = (TProfile*)p_qpsig_plus_ME_o->Clone("p_qpsig_ME_n");
  TProfile* p_qpsig_MS_n = (TProfile*)p_qpsig_plus_ME_o->Clone("p_qpsig_MS_n");
 
  // set individual plot ranges
  p_qpsig_plus_MS_o->SetMaximum(10); 
  p_qpsig_plus_MS_o->SetMinimum(-10); 
  p_qpsig_min_MS_o->SetMaximum(10); 
  p_qpsig_min_MS_o->SetMinimum(-10); 
  p_qpsig_MS_o->SetMaximum(10); 
  p_qpsig_MS_o->SetMinimum(-10); 

  // q/p only  charge separated
  TProfile* p_qp_plus_ME_o = new TProfile("p_qp_plus_ME_o","",nBins,-2.5,2.5,"s");
  p_qp_plus_ME_o->SetMaximum(.006); 
  p_qp_plus_ME_o->SetMinimum(-.006); 
  p_qp_plus_ME_o->SetTitle(";#eta_{ID};q/p_{ME}-q/p_{ID} (GeV^{-1}) (#mu^{+})");
  TProfile* p_qp_min_ME_o = (TProfile*)p_qp_plus_ME_o->Clone("p_qp_min_ME_o");
  p_qp_min_ME_o->SetTitle(";#eta_{ID};q/p_{ME}-q/p_{ID} (GeV^{-1}) (#mu^{-})");
  TProfile* p_qp_plus_MS_o = (TProfile*)p_qp_plus_ME_o->Clone("p_qp_plus_MS_o");
  p_qp_plus_MS_o->SetTitle(";#eta_{ID};q/p_{MS}-q/p_{ID} (GeV^{-1}) (#mu^{+})");
  TProfile* p_qp_min_MS_o = (TProfile*)p_qp_plus_ME_o->Clone("p_qp_min_MS_o");
  p_qp_min_MS_o->SetTitle(";#eta_{ID};q/p_{MS}-q/p_{ID} (GeV^{-1}) (#mu^{-})");

  TProfile* p_qp_plus_ME_n = (TProfile*)p_qp_plus_ME_o->Clone("p_qp_plus_ME_n");
  TProfile* p_qp_min_ME_n = (TProfile*)p_qp_plus_ME_o->Clone("p_qp_min_ME_n");
  TProfile* p_qp_plus_MS_n = (TProfile*)p_qp_plus_ME_o->Clone("p_qp_plus_MS_n");
  TProfile* p_qp_min_MS_n = (TProfile*)p_qp_plus_ME_o->Clone("p_qp_min_MS_n");
 
  // q/p
  TProfile* p_qp_ME_o = (TProfile*)p_qp_plus_ME_o->Clone("p_qp_ME_o");
  p_qp_ME_o->SetTitle(";#eta_{ID};q/p_{ME}-q/p_{ID} (GeV^{-1})"); 
  TProfile* p_qp_MS_o = (TProfile*)p_qp_plus_ME_o->Clone("p_qp_MS_o");
  p_qp_MS_o->SetTitle(";#eta_{ID};q/p_{MS}-q/p_{ID} (GeV^{-1})"); 

  TProfile* p_qp_ME_n = (TProfile*)p_qp_plus_ME_o->Clone("p_qp_ME_n");
  TProfile* p_qp_MS_n = (TProfile*)p_qp_plus_ME_o->Clone("p_qp_MS_n");
 
  // set individual plot ranges
  p_qp_plus_MS_o->SetMaximum(.05); 
  p_qp_plus_MS_o->SetMinimum(-.05); 
  p_qp_min_MS_o->SetMaximum(.05); 
  p_qp_min_MS_o->SetMinimum(-.05); 
  p_qp_MS_o->SetMaximum(.05); 
  p_qp_MS_o->SetMinimum(-.05); 
  
  // q/p uncertainty  charge separated
  TProfile* p_qpunc_plus_ME_o = new TProfile("p_qpunc_plus_ME_o","",nBins,-2.5,2.5,"s");
  p_qpunc_plus_ME_o->SetMaximum(0.004); 
  p_qpunc_plus_ME_o->SetMinimum(0); 
  p_qpunc_plus_ME_o->SetTitle(";#eta_{ID};#sigma_{q/p}^{ME} (GeV^{-1}) (#mu^{+})");
  TProfile* p_qpunc_min_ME_o = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_min_ME_o");
  p_qpunc_min_ME_o->SetTitle(";#eta_{ID};#sigma_{q/p}^{ME} (GeV^{-1}) (#mu^{-})");
  TProfile* p_qpunc_plus_MS_o = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_plus_MS_o");
  p_qpunc_plus_MS_o->SetTitle(";#eta_{ID};#sigma_{q/p}^{MS} (GeV^{-1}) (#mu^{+})");
  TProfile* p_qpunc_min_MS_o = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_min_MS_o");
  p_qpunc_min_MS_o->SetTitle(";#eta_{ID};#sigma_{q/p}^{MS} (GeV^{-1}) (#mu^{-})");
  TProfile* p_qpunc_plus_ID_o = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_plus_ID_o");
  p_qpunc_plus_ID_o->SetTitle(";#eta_{ID};#sigma_{q/p}^{ID} (GeV^{-1}) (#mu^{+})");
  TProfile* p_qpunc_min_ID_o = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_min_ID_o");
  p_qpunc_min_ID_o->SetTitle(";#eta_{ID};#sigma_{q/p}^{ID} (GeV^{-1}) (#mu^{-})");

  TProfile* p_qpunc_plus_ME_n = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_plus_ME_n");
  TProfile* p_qpunc_min_ME_n = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_min_ME_n");
  TProfile* p_qpunc_plus_MS_n = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_plus_MS_n");
  TProfile* p_qpunc_min_MS_n = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_min_MS_n");
  TProfile* p_qpunc_plus_ID_n = (TProfile*)p_qpunc_plus_ID_o->Clone("p_qpunc_plus_ID_n");
  TProfile* p_qpunc_min_ID_n = (TProfile*)p_qpunc_plus_ID_o->Clone("p_qpunc_min_ID_n");
 
  // q/p
  TProfile* p_qpunc_ME_o = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_ME_o");
  p_qpunc_ME_o->SetTitle(";#eta_{ID};#sigma_{q/p}^{ME} (GeV^{-1})"); 
  TProfile* p_qpunc_MS_o = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_MS_o");
  p_qpunc_MS_o->SetTitle(";#eta_{ID};#sigma_{q/p}^{MS} (GeV^{-1})"); 
  TProfile* p_qpunc_ID_o = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_ID_o");
  p_qpunc_ID_o->SetTitle(";#eta_{ID};#sigma_{q/p}^{ID} (GeV^{-1})"); 

  TProfile* p_qpunc_ME_n = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_ME_n");
  TProfile* p_qpunc_MS_n = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_MS_n");
  TProfile* p_qpunc_ID_n = (TProfile*)p_qpunc_plus_ME_o->Clone("p_qpunc_ID_n");
 
  // set individual plot ranges
  p_qpunc_plus_ID_o->SetMaximum(0.0019); 
  p_qpunc_plus_ID_o->SetMinimum(0); 
  p_qpunc_min_ID_o->SetMaximum(0.0019); 
  p_qpunc_min_ID_o->SetMinimum(0); 
  p_qpunc_ID_o->SetMaximum(0.0019); 
  p_qpunc_ID_o->SetMinimum(0); 

  /*  
  // template
  // plus
  TProfile* p__plus_CB_o = new TProfile("p__plus_CB_o","",nBins,-2.5,2.5,"s");
  p__plus_CB_o->SetTitle(";#eta;"); 
  p__plus_CB_o->SetMaximum();
  p__plus_CB_o->SetMinimum(); 
  TProfile* p__plus_ME_o = (TProfile*)p__plus_CB_o->Clone("p__plus_ME_o");
  TProfile* p__plus_MS_o = (TProfile*)p__plus_CB_o->Clone("p__plus_MS_o");
  TProfile* p__plus_ID_o = (TProfile*)p__plus_CB_o->Clone("p__plus_ID_o");

  TProfile* p__plus_CB_n = (TProfile*)p__plus_CB_o->Clone("p__plus_CB_n");
  TProfile* p__plus_ME_n = (TProfile*)p__plus_CB_o->Clone("p__plus_ME_n");
  TProfile* p__plus_MS_n = (TProfile*)p__plus_CB_o->Clone("p__plus_MS_n");
  TProfile* p__plus_ID_n = (TProfile*)p__plus_CB_o->Clone("p__plus_ID_n");
  
  // minus
  TProfile* p__min_CB_o = (TProfile*)p__plus_CB_o->Clone("p__min_CB_o")
  TProfile* p__min_ME_o = (TProfile*)p__plus_CB_o->Clone("p__min_ME_o");
  TProfile* p__min_MS_o = (TProfile*)p__plus_CB_o->Clone("p__min_MS_o");
  TProfile* p__min_ID_o = (TProfile*)p__plus_CB_o->Clone("p__min_ID_o");

  TProfile* p__min_CB_n = (TProfile*)p__plus_CB_o->Clone("p__min_CB_n");
  TProfile* p__min_ME_n = (TProfile*)p__plus_CB_o->Clone("p__min_ME_n");
  TProfile* p__min_MS_n = (TProfile*)p__plus_CB_o->Clone("p__min_MS_n");
  TProfile* p__min_ID_n = (TProfile*)p__plus_CB_o->Clone("p__min_ID_n");
 
  // template no charge
  TProfile* p__CB_o = (TProfile*)p__plus_CB_o->Clone("p__CB_o"); 
  TProfile* p__ME_o = (TProfile*)p__plus_CB_o->Clone("p__ME_o");
  TProfile* p__MS_o = (TProfile*)p__plus_CB_o->Clone("p__MS_o");
  TProfile* p__ID_o = (TProfile*)p__plus_CB_o->Clone("p__ID_o");

  TProfile* p__CB_n = (TProfile*)p__plus_CB_o->Clone("p__CB_n");
  TProfile* p__ME_n = (TProfile*)p__plus_CB_o->Clone("p__ME_n");
  TProfile* p__MS_n = (TProfile*)p__plus_CB_o->Clone("p__MS_n");
  TProfile* p__ID_n = (TProfile*)p__plus_CB_o->Clone("p__ID_n");
  */

  // add histograms to vectors for formatting
  std::vector<TProfile*> *vp_plus_o = new std::vector<TProfile*>();
  std::vector<TProfile*> *vp_plus_n = new std::vector<TProfile*>();
  std::vector<TProfile*> *vp_min_o = new std::vector<TProfile*>();
  std::vector<TProfile*> *vp_min_n = new std::vector<TProfile*>();
  std::vector<TProfile*> *vp_o = new std::vector<TProfile*>();
  std::vector<TProfile*> *vp_n = new std::vector<TProfile*>();
  
  // fill vectors 
  vp_plus_o->push_back(p_rho_plus_ME_o);
  vp_plus_o->push_back(p_rho_plus_MS_o);
  vp_plus_o->push_back(p_qpsig_plus_ME_o);
  vp_plus_o->push_back(p_qpsig_plus_MS_o);
  vp_plus_o->push_back(p_qp_plus_ME_o);
  vp_plus_o->push_back(p_qp_plus_MS_o);
  vp_plus_o->push_back(p_qpunc_plus_ME_o);
  vp_plus_o->push_back(p_qpunc_plus_MS_o);
  vp_plus_o->push_back(p_qpunc_plus_ID_o);
  vp_plus_o->push_back(p_pt_plus_CB_o);
  vp_plus_o->push_back(p_pt_plus_ME_o);
  vp_plus_o->push_back(p_pt_plus_MS_o);
  vp_plus_o->push_back(p_pt_plus_ID_o);
  vp_plus_o->push_back(p_rchi2_plus_CB_o);
  vp_plus_o->push_back(p_rchi2_plus_ME_o);
  vp_plus_o->push_back(p_rchi2_plus_MS_o);
  vp_plus_o->push_back(p_rchi2_plus_ID_o);
   
  vp_min_o->push_back(p_rho_min_ME_o);
  vp_min_o->push_back(p_rho_min_MS_o);
  vp_min_o->push_back(p_qpsig_min_ME_o);
  vp_min_o->push_back(p_qpsig_min_MS_o);
  vp_min_o->push_back(p_qp_min_ME_o);
  vp_min_o->push_back(p_qp_min_MS_o);
  vp_min_o->push_back(p_qpunc_min_ME_o);
  vp_min_o->push_back(p_qpunc_min_MS_o);
  vp_min_o->push_back(p_qpunc_min_ID_o);
  vp_min_o->push_back(p_pt_min_CB_o);
  vp_min_o->push_back(p_pt_min_ME_o);
  vp_min_o->push_back(p_pt_min_MS_o);
  vp_min_o->push_back(p_pt_min_ID_o);
  vp_min_o->push_back(p_rchi2_min_CB_o);
  vp_min_o->push_back(p_rchi2_min_ME_o);
  vp_min_o->push_back(p_rchi2_min_MS_o);
  vp_min_o->push_back(p_rchi2_min_ID_o);
  
  vp_plus_n->push_back(p_rho_plus_ME_n);
  vp_plus_n->push_back(p_rho_plus_MS_n);
  vp_plus_n->push_back(p_qpsig_plus_ME_n);
  vp_plus_n->push_back(p_qpsig_plus_MS_n);
  vp_plus_n->push_back(p_qp_plus_ME_n);
  vp_plus_n->push_back(p_qp_plus_MS_n);
  vp_plus_n->push_back(p_qpunc_plus_ME_n);
  vp_plus_n->push_back(p_qpunc_plus_MS_n);
  vp_plus_n->push_back(p_qpunc_plus_ID_n);
  vp_plus_n->push_back(p_pt_plus_CB_n);
  vp_plus_n->push_back(p_pt_plus_ME_n);
  vp_plus_n->push_back(p_pt_plus_MS_n);
  vp_plus_n->push_back(p_pt_plus_ID_n);
  vp_plus_n->push_back(p_rchi2_plus_CB_n);
  vp_plus_n->push_back(p_rchi2_plus_ME_n);
  vp_plus_n->push_back(p_rchi2_plus_MS_n);
  vp_plus_n->push_back(p_rchi2_plus_ID_n);
   
  vp_min_n->push_back(p_rho_min_ME_n);
  vp_min_n->push_back(p_rho_min_MS_n);
  vp_min_n->push_back(p_qpsig_min_ME_n);
  vp_min_n->push_back(p_qpsig_min_MS_n);
  vp_min_n->push_back(p_qp_min_ME_n);
  vp_min_n->push_back(p_qp_min_MS_n);
  vp_min_n->push_back(p_qpunc_min_ME_n);
  vp_min_n->push_back(p_qpunc_min_MS_n);
  vp_min_n->push_back(p_qpunc_min_ID_n);
  vp_min_n->push_back(p_pt_min_CB_n);
  vp_min_n->push_back(p_pt_min_ME_n);
  vp_min_n->push_back(p_pt_min_MS_n);
  vp_min_n->push_back(p_pt_min_ID_n);
  vp_min_n->push_back(p_rchi2_min_CB_n);
  vp_min_n->push_back(p_rchi2_min_ME_n);
  vp_min_n->push_back(p_rchi2_min_MS_n);
  vp_min_n->push_back(p_rchi2_min_ID_n);
 
  // uncharged 
  vp_o->push_back(p_rho_ME_o);
  vp_o->push_back(p_rho_MS_o);
  vp_o->push_back(p_qpsig_ME_o);
  vp_o->push_back(p_qpsig_MS_o);
  vp_o->push_back(p_qp_ME_o);
  vp_o->push_back(p_qp_MS_o);
  vp_o->push_back(p_qpunc_ME_o);
  vp_o->push_back(p_qpunc_MS_o);
  vp_o->push_back(p_qpunc_ID_o);
  vp_o->push_back(p_pt_CB_o);
  vp_o->push_back(p_pt_ME_o);
  vp_o->push_back(p_pt_MS_o);
  vp_o->push_back(p_pt_ID_o);
  vp_o->push_back(p_rchi2_CB_o);
  vp_o->push_back(p_rchi2_ME_o);
  vp_o->push_back(p_rchi2_MS_o);
  vp_o->push_back(p_rchi2_ID_o);
 
  vp_n->push_back(p_rho_ME_n);
  vp_n->push_back(p_rho_MS_n);
  vp_n->push_back(p_qpsig_ME_n);
  vp_n->push_back(p_qpsig_MS_n);
  vp_n->push_back(p_qp_ME_n);
  vp_n->push_back(p_qp_MS_n);
  vp_n->push_back(p_qpunc_ME_n);
  vp_n->push_back(p_qpunc_MS_n);
  vp_n->push_back(p_qpunc_ID_n);
  vp_n->push_back(p_pt_CB_n);
  vp_n->push_back(p_pt_ME_n);
  vp_n->push_back(p_pt_MS_n);
  vp_n->push_back(p_pt_ID_n);
  vp_n->push_back(p_rchi2_CB_n);
  vp_n->push_back(p_rchi2_ME_n);
  vp_n->push_back(p_rchi2_MS_n);
  vp_n->push_back(p_rchi2_ID_n);
  
  // check that the vectors have the same size
  if ( vp_o->size() != vp_n->size()) 
    std::cout << "Warning: profile vector sizes unequal between two maps" << std::endl;
  if ( vp_plus_o->size() != vp_plus_n->size() || vp_plus_o->size() != vp_min_o->size()
        || vp_plus_o->size() != vp_min_n->size()) 
    std::cout << "Warning: plus/minus profile vector sizes unequal" << std::endl;
  
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
  TTreeReaderValue<vector<float>> qOverP_CB_o(reader_old, "qOverP_CB");
  TTreeReaderValue<vector<float>> qOverP_ME_o(reader_old, "qOverP_ME");
  TTreeReaderValue<vector<float>> qOverP_MS_o(reader_old, "qOverP_MS");
  TTreeReaderValue<vector<float>> qOverP_ID_o(reader_old, "qOverP_ID");
  TTreeReaderValue<vector<float>> dqOverP_CB_o(reader_old, "dqOverP_CB");
  TTreeReaderValue<vector<float>> dqOverP_ME_o(reader_old, "dqOverP_ME");
  TTreeReaderValue<vector<float>> dqOverP_MS_o(reader_old, "dqOverP_MS");
  TTreeReaderValue<vector<float>> dqOverP_ID_o(reader_old, "dqOverP_ID");
  TTreeReaderValue<vector<float>> chi2_CB_o(reader_old, "chi2_CB");
  TTreeReaderValue<vector<float>> chi2_ME_o(reader_old, "chi2_ME");
  TTreeReaderValue<vector<float>> chi2_MS_o(reader_old, "chi2_MS");
  TTreeReaderValue<vector<float>> chi2_ID_o(reader_old, "chi2_ID");
  TTreeReaderValue<vector<float>> dof_CB_o(reader_old, "dof_CB");
  TTreeReaderValue<vector<float>> dof_ME_o(reader_old, "dof_ME");
  TTreeReaderValue<vector<float>> dof_MS_o(reader_old, "dof_MS");
  TTreeReaderValue<vector<float>> dof_ID_o(reader_old, "dof_ID");
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
    // loop over two muons
    for (int i = 0; i < 2; i++) {
      // if CB muon
      if (pt_CB_o->at(i) != -1) {
        p_pt_CB_o->Fill(eta_CB_o->at(i), pt_CB_o->at(i));
        p_rchi2_CB_o->Fill(eta_CB_o->at(i), chi2_CB_o->at(i)/dof_CB_o->at(i));
        if (charge_CB_o->at(i) > 0) {
          p_pt_plus_CB_o->Fill(eta_CB_o->at(i),pt_CB_o->at(i));
          p_rchi2_plus_CB_o->Fill(eta_CB_o->at(i),chi2_CB_o->at(i)/dof_CB_o->at(i));
        }
        if (charge_CB_o->at(i) < 0) {
          p_pt_min_CB_o->Fill(eta_CB_o->at(i),pt_CB_o->at(i));
          p_rchi2_min_CB_o->Fill(eta_CB_o->at(i),chi2_CB_o->at(i)/dof_CB_o->at(i));
        }
       }
  
      // if ME muon
      if (pt_ME_o->at(i) != -1) {
        p_pt_ME_o->Fill(eta_ME_o->at(i), pt_ME_o->at(i));
        p_rchi2_ME_o->Fill(eta_ME_o->at(i), chi2_ME_o->at(i)/dof_ME_o->at(i));
        if (charge_ME_o->at(i) > 0) {
          p_pt_plus_ME_o->Fill(eta_ME_o->at(i),pt_ME_o->at(i));
          p_rchi2_plus_ME_o->Fill(eta_ME_o->at(i),chi2_ME_o->at(i)/dof_ME_o->at(i));
        }
        if (charge_ME_o->at(i) < 0) {
          p_pt_min_ME_o->Fill(eta_ME_o->at(i),pt_ME_o->at(i));
          p_rchi2_min_ME_o->Fill(eta_ME_o->at(i),chi2_ME_o->at(i)/dof_ME_o->at(i));
        }
      }

      // if MS muon
      if (pt_MS_o->at(i) != -1 && pt_MS_o->at(i)*cosh(eta_MS_o->at(i)) < 1e4) {
        p_pt_MS_o->Fill(eta_MS_o->at(i), pt_MS_o->at(i));
        p_rchi2_MS_o->Fill(eta_MS_o->at(i), chi2_MS_o->at(i)/dof_MS_o->at(i));
        if (charge_MS_o->at(i) > 0) {
          p_pt_plus_MS_o->Fill(eta_MS_o->at(i),pt_MS_o->at(i));
          p_rchi2_plus_MS_o->Fill(eta_MS_o->at(i),chi2_MS_o->at(i)/dof_MS_o->at(i));
        }
        if (charge_MS_o->at(i) < 0) {
          p_pt_min_MS_o->Fill(eta_MS_o->at(i),pt_MS_o->at(i));
          p_rchi2_min_MS_o->Fill(eta_MS_o->at(i),chi2_MS_o->at(i)/dof_MS_o->at(i));
        }
      }

      // if ID muon
      if (pt_ID_o->at(i) != -1) {
        p_pt_ID_o->Fill(eta_ID_o->at(i), pt_ID_o->at(i));
        p_rchi2_ID_o->Fill(eta_ID_o->at(i), chi2_ID_o->at(i)/dof_ID_o->at(i));
        p_qpunc_ID_o->Fill(eta_ID_o->at(i), dqOverP_ID_o->at(i));
        if (charge_ID_o->at(i) > 0) {
          p_pt_plus_ID_o->Fill(eta_ID_o->at(i),pt_ID_o->at(i));
          p_rchi2_plus_ID_o->Fill(eta_ID_o->at(i),chi2_ID_o->at(i)/dof_ID_o->at(i));
          p_qpunc_plus_ID_o->Fill(eta_ID_o->at(i), dqOverP_ID_o->at(i));
        }
        if (charge_ID_o->at(i) < 0) {
          p_pt_min_ID_o->Fill(eta_ID_o->at(i),pt_ID_o->at(i));
          p_rchi2_min_ID_o->Fill(eta_ID_o->at(i),chi2_ID_o->at(i)/dof_ID_o->at(i));
          p_qpunc_min_ID_o->Fill(eta_ID_o->at(i), dqOverP_ID_o->at(i));
        }
          
        
        // fill rho and q/p-sig ME
        if (pt_ME_o->at(i) != -1) {
          p_rho_ME_o->Fill(eta_ID_o->at(i), (pt_ME_o->at(i)-pt_ID_o->at(i))/pt_ID_o->at(i));
          p_qpsig_ME_o->Fill(eta_ID_o->at(i), fabs(qOverP_ME_o->at(i)-qOverP_ID_o->at(i))/sqrt(pow(dqOverP_ME_o->at(i),2)+pow(dqOverP_ID_o->at(i),2)));
          p_qp_ME_o->Fill(eta_ID_o->at(i), qOverP_ME_o->at(i)-qOverP_ID_o->at(i));
          p_qpunc_ME_o->Fill(eta_ID_o->at(i), dqOverP_ME_o->at(i));
          // charge separated
          if (charge_ID_o->at(i) == 1) {
            p_rho_plus_ME_o->Fill(eta_ID_o->at(i), (pt_ME_o->at(i)-pt_ID_o->at(i))/pt_ID_o->at(i)); 
            p_qpsig_plus_ME_o->Fill(eta_ID_o->at(i), fabs(qOverP_ME_o->at(i)-qOverP_ID_o->at(i))/sqrt(pow(dqOverP_ME_o->at(i),2)+pow(dqOverP_ID_o->at(i),2)));
            p_qp_plus_ME_o->Fill(eta_ID_o->at(i), qOverP_ME_o->at(i)-qOverP_ID_o->at(i));
            p_qpunc_plus_ME_o->Fill(eta_ID_o->at(i), dqOverP_ME_o->at(i));
          }
          if (charge_ID_o->at(i) == -1) {
            p_rho_min_ME_o->Fill(eta_ID_o->at(i), (pt_ME_o->at(i)-pt_ID_o->at(i))/pt_ID_o->at(i)); 
            p_qpsig_min_ME_o->Fill(eta_ID_o->at(i), fabs(qOverP_ME_o->at(i)-qOverP_ID_o->at(i))/sqrt(pow(dqOverP_ME_o->at(i),2)+pow(dqOverP_ID_o->at(i),2)));
            p_qp_min_ME_o->Fill(eta_ID_o->at(i), qOverP_ME_o->at(i)-qOverP_ID_o->at(i));
            p_qpunc_min_ME_o->Fill(eta_ID_o->at(i), dqOverP_ME_o->at(i));
          }  
        } 
        // fill rho and q/p-sig MS
        if (pt_MS_o->at(i) != -1 && pt_MS_o->at(i)*cosh(eta_MS_o->at(i)) < 1e4) {
          p_rho_MS_o->Fill(eta_ID_o->at(i), (pt_MS_o->at(i)-pt_ID_o->at(i))/pt_ID_o->at(i));
          p_qpsig_MS_o->Fill(eta_ID_o->at(i), fabs(qOverP_MS_o->at(i)-qOverP_ID_o->at(i))/sqrt(pow(dqOverP_MS_o->at(i),2)+pow(dqOverP_ID_o->at(i),2)));
          p_qp_MS_o->Fill(eta_ID_o->at(i), qOverP_MS_o->at(i)-qOverP_ID_o->at(i));
          p_qpunc_MS_o->Fill(eta_ID_o->at(i), dqOverP_MS_o->at(i));
          // charge separated
          if (charge_ID_o->at(i) == 1) {
            p_rho_plus_MS_o->Fill(eta_ID_o->at(i), (pt_MS_o->at(i)-pt_ID_o->at(i))/pt_ID_o->at(i)); 
            p_qpsig_plus_MS_o->Fill(eta_ID_o->at(i), fabs(qOverP_MS_o->at(i)-qOverP_ID_o->at(i))/sqrt(pow(dqOverP_MS_o->at(i),2)+pow(dqOverP_ID_o->at(i),2)));
            p_qp_plus_MS_o->Fill(eta_ID_o->at(i), qOverP_MS_o->at(i)-qOverP_ID_o->at(i));
            p_qpunc_plus_MS_o->Fill(eta_ID_o->at(i), dqOverP_MS_o->at(i));
          }
          if (charge_ID_o->at(i) == -1) {
            p_rho_min_MS_o->Fill(eta_ID_o->at(i), (pt_MS_o->at(i)-pt_ID_o->at(i))/pt_ID_o->at(i)); 
            p_qpsig_min_MS_o->Fill(eta_ID_o->at(i), fabs(qOverP_MS_o->at(i)-qOverP_ID_o->at(i))/sqrt(pow(dqOverP_MS_o->at(i),2)+pow(dqOverP_ID_o->at(i),2)));
            p_qp_min_MS_o->Fill(eta_ID_o->at(i), qOverP_MS_o->at(i)-qOverP_ID_o->at(i));
            p_qpunc_min_MS_o->Fill(eta_ID_o->at(i), dqOverP_MS_o->at(i));
          }  
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
  TTreeReaderValue<vector<float>> qOverP_CB_n(reader_new, "qOverP_CB");
  TTreeReaderValue<vector<float>> qOverP_ME_n(reader_new, "qOverP_ME");
  TTreeReaderValue<vector<float>> qOverP_MS_n(reader_new, "qOverP_MS");
  TTreeReaderValue<vector<float>> qOverP_ID_n(reader_new, "qOverP_ID");
  TTreeReaderValue<vector<float>> dqOverP_CB_n(reader_new, "dqOverP_CB");
  TTreeReaderValue<vector<float>> dqOverP_ME_n(reader_new, "dqOverP_ME");
  TTreeReaderValue<vector<float>> dqOverP_MS_n(reader_new, "dqOverP_MS");
  TTreeReaderValue<vector<float>> dqOverP_ID_n(reader_new, "dqOverP_ID");
  TTreeReaderValue<vector<float>> chi2_CB_n(reader_new, "chi2_CB");
  TTreeReaderValue<vector<float>> chi2_ME_n(reader_new, "chi2_ME");
  TTreeReaderValue<vector<float>> chi2_MS_n(reader_new, "chi2_MS");
  TTreeReaderValue<vector<float>> chi2_ID_n(reader_new, "chi2_ID");
  TTreeReaderValue<vector<float>> dof_CB_n(reader_new, "dof_CB");
  TTreeReaderValue<vector<float>> dof_ME_n(reader_new, "dof_ME");
  TTreeReaderValue<vector<float>> dof_MS_n(reader_new, "dof_MS");
  TTreeReaderValue<vector<float>> dof_ID_n(reader_new, "dof_ID");
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
    // loop over two muons
    for (int i = 0; i < 2; i++) {
      // if CB muon
      if (pt_CB_n->at(i) != -1) {
        p_pt_CB_n->Fill(eta_CB_n->at(i), pt_CB_n->at(i));
        p_rchi2_CB_n->Fill(eta_CB_n->at(i), chi2_CB_n->at(i)/dof_CB_n->at(i));
        if (charge_CB_n->at(i) > 0) {
          p_pt_plus_CB_n->Fill(eta_CB_n->at(i),pt_CB_n->at(i));
          p_rchi2_plus_CB_n->Fill(eta_CB_n->at(i),chi2_CB_n->at(i)/dof_CB_n->at(i));
        }
        if (charge_CB_n->at(i) < 0) {
          p_pt_min_CB_n->Fill(eta_CB_n->at(i),pt_CB_n->at(i));
          p_rchi2_min_CB_n->Fill(eta_CB_n->at(i),chi2_CB_n->at(i)/dof_CB_n->at(i));
        }
       }
  
      // if ME muon
      if (pt_ME_n->at(i) != -1) {
        p_pt_ME_n->Fill(eta_ME_n->at(i), pt_ME_n->at(i));
        p_rchi2_ME_n->Fill(eta_ME_n->at(i), chi2_ME_n->at(i)/dof_ME_n->at(i));
        if (charge_ME_n->at(i) > 0) {
          p_pt_plus_ME_n->Fill(eta_ME_n->at(i),pt_ME_n->at(i));
          p_rchi2_plus_ME_n->Fill(eta_ME_n->at(i),chi2_ME_n->at(i)/dof_ME_n->at(i));
        }
        if (charge_ME_n->at(i) < 0) {
          p_pt_min_ME_n->Fill(eta_ME_n->at(i),pt_ME_n->at(i));
          p_rchi2_min_ME_n->Fill(eta_ME_n->at(i),chi2_ME_n->at(i)/dof_ME_n->at(i));
        }
      }

      // if MS muon
      if (pt_MS_n->at(i) != -1 && pt_MS_n->at(i)*cosh(eta_MS_n->at(i)) < 1e4) {
        p_pt_MS_n->Fill(eta_MS_n->at(i), pt_MS_n->at(i));
        p_rchi2_MS_n->Fill(eta_MS_n->at(i), chi2_MS_n->at(i)/dof_MS_n->at(i));
        if (charge_MS_n->at(i) > 0) {
          p_pt_plus_MS_n->Fill(eta_MS_n->at(i),pt_MS_n->at(i));
          p_rchi2_plus_MS_n->Fill(eta_MS_n->at(i),chi2_MS_n->at(i)/dof_MS_n->at(i));
        }
        if (charge_MS_n->at(i) < 0) {
          p_pt_min_MS_n->Fill(eta_MS_n->at(i),pt_MS_n->at(i));
          p_rchi2_min_MS_n->Fill(eta_MS_n->at(i),chi2_MS_n->at(i)/dof_MS_n->at(i));
        }
      }

      // if ID muon
      if (pt_ID_n->at(i) != -1) {
        p_pt_ID_n->Fill(eta_ID_n->at(i), pt_ID_n->at(i));
        p_rchi2_ID_n->Fill(eta_ID_n->at(i), chi2_ID_n->at(i)/dof_ID_n->at(i));
        p_qpunc_ID_n->Fill(eta_ID_n->at(i), dqOverP_ID_n->at(i));
        if (charge_ID_n->at(i) > 0) {
          p_pt_plus_ID_n->Fill(eta_ID_n->at(i),pt_ID_n->at(i));
          p_rchi2_plus_ID_n->Fill(eta_ID_n->at(i),chi2_ID_n->at(i)/dof_ID_n->at(i));
          p_qpunc_plus_ID_n->Fill(eta_ID_n->at(i), dqOverP_ID_n->at(i));
        }
        if (charge_ID_n->at(i) < 0) {
          p_pt_min_ID_n->Fill(eta_ID_n->at(i),pt_ID_n->at(i));
          p_rchi2_min_ID_n->Fill(eta_ID_n->at(i),chi2_ID_n->at(i)/dof_ID_n->at(i));
          p_qpunc_min_ID_n->Fill(eta_ID_n->at(i), dqOverP_ID_n->at(i));
        }
        
        // fill rho and q/p-sig ME
        if (pt_ME_n->at(i) != -1) {
          p_rho_ME_n->Fill(eta_ID_n->at(i), (pt_ME_n->at(i)-pt_ID_n->at(i))/pt_ID_n->at(i));
          p_qpsig_ME_n->Fill(eta_ID_n->at(i), fabs(qOverP_ME_n->at(i)-qOverP_ID_n->at(i))/sqrt(pow(dqOverP_ME_n->at(i),2)+pow(dqOverP_ID_n->at(i),2)));
          p_qp_ME_n->Fill(eta_ID_n->at(i), qOverP_ME_n->at(i)-qOverP_ID_n->at(i));
          p_qpunc_ME_n->Fill(eta_ID_n->at(i), dqOverP_ME_n->at(i));
          // charge separated
          if (charge_ID_n->at(i) == 1) {
            p_rho_plus_ME_n->Fill(eta_ID_n->at(i), (pt_ME_n->at(i)-pt_ID_n->at(i))/pt_ID_n->at(i)); 
            p_qpsig_plus_ME_n->Fill(eta_ID_n->at(i), fabs(qOverP_ME_n->at(i)-qOverP_ID_n->at(i))/sqrt(pow(dqOverP_ME_n->at(i),2)+pow(dqOverP_ID_n->at(i),2)));
            p_qp_plus_ME_n->Fill(eta_ID_n->at(i), qOverP_ME_n->at(i)-qOverP_ID_n->at(i));
            p_qpunc_plus_ME_n->Fill(eta_ID_n->at(i), dqOverP_ME_n->at(i));
          }
          if (charge_ID_n->at(i) == -1) {
            p_rho_min_ME_n->Fill(eta_ID_n->at(i), (pt_ME_n->at(i)-pt_ID_n->at(i))/pt_ID_n->at(i)); 
            p_qpsig_min_ME_n->Fill(eta_ID_n->at(i), fabs(qOverP_ME_n->at(i)-qOverP_ID_n->at(i))/sqrt(pow(dqOverP_ME_n->at(i),2)+pow(dqOverP_ID_n->at(i),2)));
            p_qp_min_ME_n->Fill(eta_ID_n->at(i), qOverP_ME_n->at(i)-qOverP_ID_n->at(i));
            p_qpunc_min_ME_n->Fill(eta_ID_n->at(i), dqOverP_ME_n->at(i));
          }  
        } 
        // fill rho and q/p-sig MS
        if (pt_MS_n->at(i) != -1 && pt_MS_n->at(i)*cosh(eta_MS_n->at(i)) < 1e4) {
          p_rho_MS_n->Fill(eta_ID_n->at(i), (pt_MS_n->at(i)-pt_ID_n->at(i))/pt_ID_n->at(i));
          p_qpsig_MS_n->Fill(eta_ID_n->at(i), fabs(qOverP_MS_n->at(i)-qOverP_ID_n->at(i))/sqrt(pow(dqOverP_MS_n->at(i),2)+pow(dqOverP_ID_n->at(i),2)));
          p_qp_MS_n->Fill(eta_ID_n->at(i), qOverP_MS_n->at(i)-qOverP_ID_n->at(i));
          p_qpunc_MS_n->Fill(eta_ID_n->at(i), dqOverP_MS_n->at(i));
          // charge separated
          if (charge_ID_n->at(i) == 1) {
            p_rho_plus_MS_n->Fill(eta_ID_n->at(i), (pt_MS_n->at(i)-pt_ID_n->at(i))/pt_ID_n->at(i)); 
            p_qpsig_plus_MS_n->Fill(eta_ID_n->at(i), fabs(qOverP_MS_n->at(i)-qOverP_ID_n->at(i))/sqrt(pow(dqOverP_MS_n->at(i),2)+pow(dqOverP_ID_n->at(i),2)));
            p_qp_plus_MS_n->Fill(eta_ID_n->at(i), qOverP_MS_n->at(i)-qOverP_ID_n->at(i));
            p_qpunc_plus_MS_n->Fill(eta_ID_n->at(i), dqOverP_MS_n->at(i));
          }
          if (charge_ID_n->at(i) == -1) {
            p_rho_min_MS_n->Fill(eta_ID_n->at(i), (pt_MS_n->at(i)-pt_ID_n->at(i))/pt_ID_n->at(i)); 
            p_qpsig_min_MS_n->Fill(eta_ID_n->at(i), fabs(qOverP_MS_n->at(i)-qOverP_ID_n->at(i))/sqrt(pow(dqOverP_MS_n->at(i),2)+pow(dqOverP_ID_n->at(i),2)));
            p_qp_min_MS_n->Fill(eta_ID_n->at(i), qOverP_MS_n->at(i)-qOverP_ID_n->at(i));
            p_qpunc_min_MS_n->Fill(eta_ID_n->at(i), dqOverP_MS_n->at(i));
          }  
        } 
      }
    }
  }
  delete f_recoNew;

  // canvas charge-separated profiles
  TCanvas *CSProfileCan = new TCanvas("profileCan","",1000,1000);
  CSProfileCan->cd();
  for (int i = 0; i < vp_plus_o->size(); i++) {
    MakeCSProfile(CSProfileCan, vp_plus_o->at(i), vp_min_o->at(i), vp_plus_n->at(i), vp_min_n->at(i),
                  oldMapName, newMapName, file_prefix);
    CSProfileCan->Clear();
  }
  CSProfileCan->Close();

  // canvas for profiles
  TCanvas *profileCan = new TCanvas("profileCan","",1000,1000);
  profileCan->cd();
  for (int i = 0; i < vp_o->size(); i++) { 
    MakeProfile(profileCan, vp_o->at(i), vp_n->at(i), oldMapName, newMapName, file_prefix);
    profileCan->Clear();
  }
  profileCan->Close();

  delete CSProfileCan;
  delete profileCan;

  // format histograms
  for (int i = 0; i < vrho_o->size(); i++) {
    vrho_o->at(i)->SetLineColor(16); 
    vrho_o->at(i)->SetLineWidth(2);
    vrho_o->at(i)->SetMarkerColor(kBlack);
    vrho_o->at(i)->SetMarkerStyle(20);

    vrho_n->at(i)->SetLineColor(38);
    vrho_n->at(i)->SetLineWidth(2);
    vrho_n->at(i)->SetMarkerColor(kAzure);
    vrho_n->at(i)->SetMarkerStyle(20);
  }

  // loop through bins of rho tprofile to fill
  for (int bin = 1; bin <= nBins; bin++) {
    h_rho_plus_ME_o->SetBinContent(bin, p_rho_plus_ME_o->GetBinContent(bin));
    h_rho_min_ME_o->SetBinContent(bin, p_rho_min_ME_o->GetBinContent(bin));
    h_rho_ME_o->SetBinContent(bin, p_rho_ME_o->GetBinContent(bin));
    h_rho_plus_MS_o->SetBinContent(bin, p_rho_plus_MS_o->GetBinContent(bin));
    h_rho_min_MS_o->SetBinContent(bin, p_rho_min_MS_o->GetBinContent(bin));
    h_rho_MS_o->SetBinContent(bin, p_rho_MS_o->GetBinContent(bin));
    
    h_rho_plus_ME_n->SetBinContent(bin, p_rho_plus_ME_n->GetBinContent(bin));
    h_rho_min_ME_n->SetBinContent(bin, p_rho_min_ME_n->GetBinContent(bin));
    h_rho_ME_n->SetBinContent(bin, p_rho_ME_n->GetBinContent(bin));
    h_rho_plus_MS_n->SetBinContent(bin, p_rho_plus_MS_n->GetBinContent(bin));
    h_rho_min_MS_n->SetBinContent(bin, p_rho_min_MS_n->GetBinContent(bin));
    h_rho_MS_n->SetBinContent(bin, p_rho_MS_n->GetBinContent(bin));
      
    h_rhorms_plus_ME_o->SetBinContent(bin, p_rho_plus_ME_o->GetBinError(bin));
    h_rhorms_min_ME_o->SetBinContent(bin, p_rho_min_ME_o->GetBinError(bin));
    h_rhorms_ME_o->SetBinContent(bin, p_rho_ME_o->GetBinError(bin));
    h_rhorms_plus_MS_o->SetBinContent(bin, p_rho_plus_MS_o->GetBinError(bin));
    h_rhorms_min_MS_o->SetBinContent(bin, p_rho_min_MS_o->GetBinError(bin));
    h_rhorms_MS_o->SetBinContent(bin, p_rho_MS_o->GetBinError(bin));
    
    h_rhorms_plus_ME_n->SetBinContent(bin, p_rho_plus_ME_n->GetBinError(bin));
    h_rhorms_min_ME_n->SetBinContent(bin, p_rho_min_ME_n->GetBinError(bin));
    h_rhorms_ME_n->SetBinContent(bin, p_rho_ME_n->GetBinError(bin));
    h_rhorms_plus_MS_n->SetBinContent(bin, p_rho_plus_MS_n->GetBinError(bin));
    h_rhorms_min_MS_n->SetBinContent(bin, p_rho_min_MS_n->GetBinError(bin));
    h_rhorms_MS_n->SetBinContent(bin, p_rho_MS_n->GetBinError(bin));
  }

  // top right legend
  TLegend* l_tr = new TLegend(.66,.76,.89,.88);
  l_tr->AddEntry(h_rho_ME_o, Form("%s Map",oldMapName.c_str()), "lp");
  l_tr->AddEntry(h_rho_ME_n, Form("%s Map",newMapName.c_str()), "lp");

  // top center labels
  TPaveText* t_tc = new TPaveText(.39,.8,.67,.85,"NDC");
  t_tc->AddText("#it{ATLAS} #bf{Internal}");
  t_tc->SetFillColor(0);
  t_tc->SetLineColor(0);
  t_tc->SetBorderSize(0);
  t_tc->SetTextAlign(30);
  
  // set histogram max values
  double max = 1, min;
  for (int i = 0; i < vrho_o->size(); i++) {
    if (i > 5) max = .3;
    else max = .05;   
    min = std::min(vrho_o->at(i)->GetMinimum(), vrho_n->at(i)->GetMinimum());
    min -= 0.05*(max-min);
    max += 0.2*(max-min);
    
    vrho_o->at(i)->SetMaximum(max);
    vrho_n->at(i)->SetMaximum(max);
    vrho_o->at(i)->SetMinimum(min);
    vrho_n->at(i)->SetMinimum(min);
  }
 
  // plotting
  TCanvas* ratioCan = new TCanvas("ratioCan","",1000,1000);
  ratioCan->cd();
  ratioCan->Clear();
  /*
  for (int i = 0; i < vrho_o->size(); i++) {
    MakePercDiffPlot(ratioCan, vrho_o->at(i), vrho_n->at(i), l_tr, t_tc, 
                  oldMapName, newMapName, file_prefix);
  }
  */
  ratioCan->Close();

  for (int i = 0; i < vp_o->size(); i++) {
    delete vp_o->at(i);
    delete vp_n->at(i);
  }
  delete vp_o;
  delete vp_n;

  for (int i = 0; i < vp_plus_o->size(); i++) {
    delete vp_plus_o->at(i);
    delete vp_plus_n->at(i);
    delete vp_min_o->at(i);
    delete vp_min_n->at(i);
  }
  delete vp_plus_o;
  delete vp_plus_n;
  delete vp_min_o;
  delete vp_min_n;
 
  for (int i = 0; i < vrho_n->size(); i++) {
    delete vrho_o->at(i);
    delete vrho_n->at(i); 
  }
  delete vrho_o;
  delete vrho_n;
  
  delete ratioCan;
  delete l_tr;
  delete t_tc;
}
/**************************************************************************************************/

void MakeRatioPlots(string file_prefix, string oldFileName, string newFileName) {
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
  TH1F* h_m_MSO_o = (TH1F*)h_m_CB_o->Clone("h_m_MSO_o");
  TH1F* h_m_MSOE_o = (TH1F*)h_m_CB_o->Clone("h_m_MSOE_o");
  TH1F* h_m_ID_o = (TH1F*)h_m_CB_o->Clone("h_m_ID_o");
  
  TH1F* h_m_CB_n = (TH1F*)h_m_CB_o->Clone("h_m_CB_n");
  TH1F* h_m_ME_n = (TH1F*)h_m_CB_o->Clone("h_m_ME_n");
  TH1F* h_m_MSO_n = (TH1F*)h_m_CB_o->Clone("h_m_MSO_n");
  TH1F* h_m_MSOE_n = (TH1F*)h_m_CB_o->Clone("h_m_MSOE_n");
  TH1F* h_m_ID_n = (TH1F*)h_m_CB_o->Clone("h_m_ID_n");

  // leading pt histograms for 3 track types and 2 files
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
 
  // leading pt histograms for 3 track types and 2 files
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
  
  // eta histograms for 3 track types and 2 files leading
  TH1F* h_eta1_CB_o = new TH1F("h_eta1_CB_o","",nBins,-2.7,2.7);
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
 
  // eta histograms for 3 track types and 2 files subleading
  TH1F* h_eta2_CB_o = new TH1F("h_eta2_CB_o","",nBins,-2.7,2.7);
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

  TH1F* h_author1_n = (TH1F*)h_author1_o->Clone("h_author1_o");
  TH1F* h_author2_n = (TH1F*)h_author2_o->Clone("h_author2_o");

  // quality
  TH1F* h_allQuality_o = new TH1F("h_allQuality_o","",4,0,4);
  h_allQuality_o->SetTitle(";Muon Quality;Events");
  TH1F* h_quality_o = new TH1F("h_quality_o","",4,0,4);
  h_quality_o->SetTitle(";Highest P_{T} Muons Quality;Events");
  
  TH1F* h_allQuality_n = (TH1F*)h_allQuality_o->Clone("h_allQuality_o");
  TH1F* h_quality_n = (TH1F*)h_quality_o->Clone("h_quality_o");
  
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
  
  // add histograms to vectors for formatting
  std::vector<TH1F*> *vh_o = new std::vector<TH1F*>();
  std::vector<TH1F*> *vh_n = new std::vector<TH1F*>();
  std::vector<double> *maxFactor = new std::vector<double>();

  // push back vector of histograms and plot range scalings            
  vh_o->push_back(h_m_CB_o);        vh_n->push_back(h_m_CB_n);        maxFactor->push_back(1.05);
  vh_o->push_back(h_m_ME_o);        vh_n->push_back(h_m_ME_n);        maxFactor->push_back(1.05);
  vh_o->push_back(h_m_MSO_o);       vh_n->push_back(h_m_MSO_n);       maxFactor->push_back(1.05);
  vh_o->push_back(h_m_MSOE_o);      vh_n->push_back(h_m_MSOE_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_m_ID_o);        vh_n->push_back(h_m_ID_n);        maxFactor->push_back(1.05);
  vh_o->push_back(h_pt1_CB_o);      vh_n->push_back(h_pt1_CB_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_pt1_ME_o);      vh_n->push_back(h_pt1_ME_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_pt1_MSO_o);     vh_n->push_back(h_pt1_MSO_n);     maxFactor->push_back(1.05);
  vh_o->push_back(h_pt1_MSOE_o);    vh_n->push_back(h_pt1_MSOE_n);    maxFactor->push_back(1.05);
  vh_o->push_back(h_pt1_ID_o);      vh_n->push_back(h_pt1_ID_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_pt2_CB_o);      vh_n->push_back(h_pt2_CB_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_pt2_ME_o);      vh_n->push_back(h_pt2_ME_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_pt2_MSO_o);     vh_n->push_back(h_pt2_MSO_n);     maxFactor->push_back(1.05);
  vh_o->push_back(h_pt2_MSOE_o);    vh_n->push_back(h_pt2_MSOE_n);    maxFactor->push_back(1.05);
  vh_o->push_back(h_pt2_ID_o);      vh_n->push_back(h_pt2_ID_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_eta1_CB_o);     vh_n->push_back(h_eta1_CB_n);     maxFactor->push_back(1.3);
  vh_o->push_back(h_eta1_ME_o);     vh_n->push_back(h_eta1_ME_n);     maxFactor->push_back(1.3);
  vh_o->push_back(h_eta1_MSO_o);    vh_n->push_back(h_eta1_MSO_n);    maxFactor->push_back(1.3);
  vh_o->push_back(h_eta1_MSOE_o);   vh_n->push_back(h_eta1_MSOE_n);   maxFactor->push_back(1.3);
  vh_o->push_back(h_eta1_ID_o);     vh_n->push_back(h_eta1_ID_n);     maxFactor->push_back(1.3);
  vh_o->push_back(h_eta2_CB_o);     vh_n->push_back(h_eta2_CB_n);     maxFactor->push_back(1.3);
  vh_o->push_back(h_eta2_ME_o);     vh_n->push_back(h_eta2_ME_n);     maxFactor->push_back(1.3);
  vh_o->push_back(h_eta2_MSO_o);    vh_n->push_back(h_eta2_MSO_n);    maxFactor->push_back(1.3);
  vh_o->push_back(h_eta2_MSOE_o);   vh_n->push_back(h_eta2_MSOE_n);   maxFactor->push_back(1.3);
  vh_o->push_back(h_eta2_ID_o);     vh_n->push_back(h_eta2_ID_n);     maxFactor->push_back(1.3);
  vh_o->push_back(h_author1_o);     vh_n->push_back(h_author1_n);     maxFactor->push_back(1.05);
  vh_o->push_back(h_author2_o);     vh_n->push_back(h_author2_n);     maxFactor->push_back(1.05);
  vh_o->push_back(h_quality_o);     vh_n->push_back(h_quality_n);     maxFactor->push_back(1.05);
  vh_o->push_back(h_allQuality_o);  vh_n->push_back(h_allQuality_n);  maxFactor->push_back(1.05);
  vh_o->push_back(h_rho1_ME_o);     vh_n->push_back(h_rho1_ME_n);     maxFactor->push_back(1.05);
  vh_o->push_back(h_rho1_MSO_o);    vh_n->push_back(h_rho1_MSO_n);    maxFactor->push_back(1.05);
  vh_o->push_back(h_rho2_ME_o);     vh_n->push_back(h_rho2_ME_n);     maxFactor->push_back(1.05);
  vh_o->push_back(h_rho2_MSO_o);    vh_n->push_back(h_rho2_MSO_n);    maxFactor->push_back(1.05);
  vh_o->push_back(h_dR1_ME_o);      vh_n->push_back(h_dR1_ME_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_dR1_MSOE_o);    vh_n->push_back(h_dR1_MSOE_n);    maxFactor->push_back(1.05);
  vh_o->push_back(h_dR2_ME_o);      vh_n->push_back(h_dR2_ME_n);      maxFactor->push_back(1.05);
  vh_o->push_back(h_dR2_MSOE_o);    vh_n->push_back(h_dR2_MSOE_n);    maxFactor->push_back(1.05);


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
  //TFile* f_recoOld = new TFile(Form("ntuples_muonSelection/%s", oldFileName.c_str()));
  //if (!f_recoOld) cout << "Warning: could not open file " << oldFileName << endl;
  
  TChain chainOld("RecoMuons");
  std::ifstream recoOldFileNames("AODfiles_test.txt");
  string fileNameOld;
  while (recoOldFileNames >> fileNameOld) 
    chainOld.Add(fileNameOld.c_str());
  
  string oldMapName = "Official AOD";
  //string oldMapName = "Run I";

  long int nEvents_o = 0;
  long int nLeadingPlus_o = 0;
  long int nEvents_noBadMS_o = 0;
  long int nLeadingPlus_noBadMS_o = 0;

  // event level  
  TTreeReader reader_old(&chainOld);
  //TTreeReader reader_old("RecoMuons", f_recoOld);
  TTreeReaderValue<bool> passGRL_o(reader_old, "passGRL");
  TTreeReaderValue<unsigned long long> eventNumber_o(reader_old, "eventNumber");
  TTreeReaderValue<unsigned int> nPositiveMuons_o(reader_old, "nPositiveMuons");
  TTreeReaderValue<unsigned int> nNegativeMuons_o(reader_old, "nNegativeMuons");
  
  // positive muon variables
  TTreeReaderValue<vector<bool>> p_passIDcuts_o(reader_old, "p_passIDcuts");
  TTreeReaderValue<vector<bool>> p_passAll_o(reader_old, "p_passAll");
  TTreeReaderValue<vector<float>> p_eLoss_o(reader_old, "p_eLoss");
  TTreeReaderValue<vector<float>> p_ptcone40_o(reader_old, "p_ptcone40");
  TTreeReaderValue<vector<int>> p_muonType_o(reader_old, "p_muonType");
  TTreeReaderValue<vector<int>> p_quality_o(reader_old, "p_quality");
  TTreeReaderValue<vector<int>> p_primaryAuthor_o(reader_old, "p_primaryAuthor");
  TTreeReaderValue<vector<int>> p_authors_o(reader_old, "p_authors");
  TTreeReaderValue<vector<int>> p_nPrecisionLayers_o(reader_old, "p_nPrecisionLayers");
  TTreeReaderValue<vector<int>> p_nPrecisionHoleLayers_o(reader_old, "p_nPrecisionHoleLayers");
  TTreeReaderValue<vector<int>> p_innerSmallHits_o(reader_old, "p_innerSmallHits");
  TTreeReaderValue<vector<int>> p_innerLargeHits_o(reader_old, "p_innerLargeHits");
  TTreeReaderValue<vector<int>> p_middleSmallHits_o(reader_old, "p_middleSmallHits");
  TTreeReaderValue<vector<int>> p_middleLargeHits_o(reader_old, "p_middleLargeHits");
  TTreeReaderValue<vector<int>> p_outerSmallHits_o(reader_old, "p_outerSmallHits");
  TTreeReaderValue<vector<int>> p_outerLargeHits_o(reader_old, "p_outerLargeHits");
  TTreeReaderValue<vector<int>> p_extendedSmallHits_o(reader_old, "p_extendedSmallHits");
  TTreeReaderValue<vector<int>> p_extendedLargeHits_o(reader_old, "p_extendedLargeHits"); 
  TTreeReaderValue<vector<int>> p_innerSmallHoles_o(reader_old, "p_innerSmallHoles");
  TTreeReaderValue<vector<int>> p_innerLargeHoles_o(reader_old, "p_innerLargeHoles");
  TTreeReaderValue<vector<int>> p_middleSmallHoles_o(reader_old, "p_middleSmallHoles");
  TTreeReaderValue<vector<int>> p_middleLargeHoles_o(reader_old, "p_middleLargeHoles");
  TTreeReaderValue<vector<int>> p_outerSmallHoles_o(reader_old, "p_outerSmallHoles");
  TTreeReaderValue<vector<int>> p_outerLargeHoles_o(reader_old, "p_outerLargeHoles");
  TTreeReaderValue<vector<int>> p_extendedSmallHoles_o(reader_old, "p_extendedSmallHoles");
  TTreeReaderValue<vector<int>> p_extendedLargeHoles_o(reader_old, "p_extendedLargeHoles");

  // negative muon variables
  TTreeReaderValue<vector<bool>> n_passIDcuts_o(reader_old, "n_passIDcuts");
  TTreeReaderValue<vector<bool>> n_passAll_o(reader_old, "n_passAll");
  TTreeReaderValue<vector<float>> n_eLoss_o(reader_old, "n_eLoss");
  TTreeReaderValue<vector<float>> n_ptcone40_o(reader_old, "n_ptcone40");
  TTreeReaderValue<vector<int>> n_muonType_o(reader_old, "n_muonType");
  TTreeReaderValue<vector<int>> n_quality_o(reader_old, "n_quality");
  TTreeReaderValue<vector<int>> n_primaryAuthor_o(reader_old, "n_primaryAuthor");
  TTreeReaderValue<vector<int>> n_authors_o(reader_old, "n_authors");
  TTreeReaderValue<vector<int>> n_nPrecisionLayers_o(reader_old, "n_nPrecisionLayers");
  TTreeReaderValue<vector<int>> n_nPrecisionHoleLayers_o(reader_old, "n_nPrecisionHoleLayers");
  TTreeReaderValue<vector<int>> n_innerSmallHits_o(reader_old, "n_innerSmallHits");
  TTreeReaderValue<vector<int>> n_innerLargeHits_o(reader_old, "n_innerLargeHits");
  TTreeReaderValue<vector<int>> n_middleSmallHits_o(reader_old, "n_middleSmallHits");
  TTreeReaderValue<vector<int>> n_middleLargeHits_o(reader_old, "n_middleLargeHits");
  TTreeReaderValue<vector<int>> n_outerSmallHits_o(reader_old, "n_outerSmallHits");
  TTreeReaderValue<vector<int>> n_outerLargeHits_o(reader_old, "n_outerLargeHits");
  TTreeReaderValue<vector<int>> n_extendedSmallHits_o(reader_old, "n_extendedSmallHits");
  TTreeReaderValue<vector<int>> n_extendedLargeHits_o(reader_old, "n_extendedLargeHits"); 
  TTreeReaderValue<vector<int>> n_innerSmallHoles_o(reader_old, "n_innerSmallHoles");
  TTreeReaderValue<vector<int>> n_innerLargeHoles_o(reader_old, "n_innerLargeHoles");
  TTreeReaderValue<vector<int>> n_middleSmallHoles_o(reader_old, "n_middleSmallHoles");
  TTreeReaderValue<vector<int>> n_middleLargeHoles_o(reader_old, "n_middleLargeHoles");
  TTreeReaderValue<vector<int>> n_outerSmallHoles_o(reader_old, "n_outerSmallHoles");
  TTreeReaderValue<vector<int>> n_outerLargeHoles_o(reader_old, "n_outerLargeHoles");
  TTreeReaderValue<vector<int>> n_extendedSmallHoles_o(reader_old, "n_extendedSmallHoles");
  TTreeReaderValue<vector<int>> n_extendedLargeHoles_o(reader_old, "n_extendedLargeHoles");

  // positive muon track variables
  TTreeReaderValue<vector<bool>> p_isCB_o(reader_old, "p_isCB");
  TTreeReaderValue<vector<bool>> p_isME_o(reader_old, "p_isME");
  TTreeReaderValue<vector<bool>> p_isMSO_o(reader_old, "p_isMSO");
  TTreeReaderValue<vector<bool>> p_isMSOE_o(reader_old, "p_isMSOE");
  TTreeReaderValue<vector<bool>> p_isID_o(reader_old, "p_isID");
  TTreeReaderValue<vector<float>> p_eta_CB_o(reader_old, "p_eta_CB");
  TTreeReaderValue<vector<float>> p_eta_ME_o(reader_old, "p_eta_ME");
  TTreeReaderValue<vector<float>> p_eta_MSO_o(reader_old, "p_eta_MSO");
  TTreeReaderValue<vector<float>> p_eta_MSOE_o(reader_old, "p_eta_MSOE");
  TTreeReaderValue<vector<float>> p_eta_ID_o(reader_old, "p_eta_ID");
  TTreeReaderValue<vector<float>> p_phi_CB_o(reader_old, "p_phi_CB");
  TTreeReaderValue<vector<float>> p_phi_ME_o(reader_old, "p_phi_ME");
  TTreeReaderValue<vector<float>> p_phi_MSO_o(reader_old, "p_phi_MSO");
  TTreeReaderValue<vector<float>> p_phi_MSOE_o(reader_old, "p_phi_MSOE");
  TTreeReaderValue<vector<float>> p_phi_ID_o(reader_old, "p_phi_ID");
  TTreeReaderValue<vector<float>> p_d0sig_CB_o(reader_old, "p_d0sig_CB");
  TTreeReaderValue<vector<float>> p_d0sig_ME_o(reader_old, "p_d0sig_ME");
  TTreeReaderValue<vector<float>> p_d0sig_MSO_o(reader_old, "p_d0sig_MSO");
  TTreeReaderValue<vector<float>> p_d0sig_MSOE_o(reader_old, "p_d0sig_MSOE");
  TTreeReaderValue<vector<float>> p_d0sig_ID_o(reader_old, "p_d0sig_ID");
  TTreeReaderValue<vector<float>> p_z0_CB_o(reader_old, "p_z0_CB");
  TTreeReaderValue<vector<float>> p_z0_ME_o(reader_old, "p_z0_ME");
  TTreeReaderValue<vector<float>> p_z0_MSO_o(reader_old, "p_z0_MSO");
  TTreeReaderValue<vector<float>> p_z0_MSOE_o(reader_old, "p_z0_MSOE");
  TTreeReaderValue<vector<float>> p_z0_ID_o(reader_old, "p_z0_ID");
  TTreeReaderValue<vector<float>> p_qOverP_CB_o(reader_old, "p_qOverP_CB");
  TTreeReaderValue<vector<float>> p_qOverP_ME_o(reader_old, "p_qOverP_ME");
  TTreeReaderValue<vector<float>> p_qOverP_MSO_o(reader_old, "p_qOverP_MSO");
  TTreeReaderValue<vector<float>> p_qOverP_MSOE_o(reader_old, "p_qOverP_MSOE");
  TTreeReaderValue<vector<float>> p_qOverP_ID_o(reader_old, "p_qOverP_ID");
  TTreeReaderValue<vector<float>> p_dqOverP_CB_o(reader_old, "p_dqOverP_CB");
  TTreeReaderValue<vector<float>> p_dqOverP_ME_o(reader_old, "p_dqOverP_ME");
  TTreeReaderValue<vector<float>> p_dqOverP_MSO_o(reader_old, "p_dqOverP_MSO");
  TTreeReaderValue<vector<float>> p_dqOverP_MSOE_o(reader_old, "p_dqOverP_MSOE");
  TTreeReaderValue<vector<float>> p_dqOverP_ID_o(reader_old, "p_dqOverP_ID");
  TTreeReaderValue<vector<float>> p_rchi2_CB_o(reader_old, "p_rchi2_CB");
  TTreeReaderValue<vector<float>> p_rchi2_ME_o(reader_old, "p_rchi2_ME");
  TTreeReaderValue<vector<float>> p_rchi2_MSO_o(reader_old, "p_rchi2_MSO");
  TTreeReaderValue<vector<float>> p_rchi2_MSOE_o(reader_old, "p_rchi2_MSOE");
  TTreeReaderValue<vector<float>> p_rchi2_ID_o(reader_old, "p_rchi2_ID");
  
  // negative muon track variables
  TTreeReaderValue<vector<bool>> n_isCB_o(reader_old, "n_isCB");
  TTreeReaderValue<vector<bool>> n_isME_o(reader_old, "n_isME");
  TTreeReaderValue<vector<bool>> n_isMSO_o(reader_old, "n_isMSO");
  TTreeReaderValue<vector<bool>> n_isMSOE_o(reader_old, "n_isMSOE");
  TTreeReaderValue<vector<bool>> n_isID_o(reader_old, "n_isID");
  TTreeReaderValue<vector<float>> n_eta_CB_o(reader_old, "n_eta_CB");
  TTreeReaderValue<vector<float>> n_eta_ME_o(reader_old, "n_eta_ME");
  TTreeReaderValue<vector<float>> n_eta_MSO_o(reader_old, "n_eta_MSO");
  TTreeReaderValue<vector<float>> n_eta_MSOE_o(reader_old, "n_eta_MSOE");
  TTreeReaderValue<vector<float>> n_eta_ID_o(reader_old, "n_eta_ID");
  TTreeReaderValue<vector<float>> n_phi_CB_o(reader_old, "n_phi_CB");
  TTreeReaderValue<vector<float>> n_phi_ME_o(reader_old, "n_phi_ME");
  TTreeReaderValue<vector<float>> n_phi_MSO_o(reader_old, "n_phi_MSO");
  TTreeReaderValue<vector<float>> n_phi_MSOE_o(reader_old, "n_phi_MSOE");
  TTreeReaderValue<vector<float>> n_phi_ID_o(reader_old, "n_phi_ID");
  TTreeReaderValue<vector<float>> n_d0sig_CB_o(reader_old, "n_d0sig_CB");
  TTreeReaderValue<vector<float>> n_d0sig_ME_o(reader_old, "n_d0sig_ME");
  TTreeReaderValue<vector<float>> n_d0sig_MSO_o(reader_old, "n_d0sig_MSO");
  TTreeReaderValue<vector<float>> n_d0sig_MSOE_o(reader_old, "n_d0sig_MSOE");
  TTreeReaderValue<vector<float>> n_d0sig_ID_o(reader_old, "n_d0sig_ID");
  TTreeReaderValue<vector<float>> n_z0_CB_o(reader_old, "n_z0_CB");
  TTreeReaderValue<vector<float>> n_z0_ME_o(reader_old, "n_z0_ME");
  TTreeReaderValue<vector<float>> n_z0_MSO_o(reader_old, "n_z0_MSO");
  TTreeReaderValue<vector<float>> n_z0_MSOE_o(reader_old, "n_z0_MSOE");
  TTreeReaderValue<vector<float>> n_z0_ID_o(reader_old, "n_z0_ID");
  TTreeReaderValue<vector<float>> n_qOverP_CB_o(reader_old, "n_qOverP_CB");
  TTreeReaderValue<vector<float>> n_qOverP_ME_o(reader_old, "n_qOverP_ME");
  TTreeReaderValue<vector<float>> n_qOverP_MSO_o(reader_old, "n_qOverP_MSO");
  TTreeReaderValue<vector<float>> n_qOverP_MSOE_o(reader_old, "n_qOverP_MSOE");
  TTreeReaderValue<vector<float>> n_qOverP_ID_o(reader_old, "n_qOverP_ID");
  TTreeReaderValue<vector<float>> n_dqOverP_CB_o(reader_old, "n_dqOverP_CB");
  TTreeReaderValue<vector<float>> n_dqOverP_ME_o(reader_old, "n_dqOverP_ME");
  TTreeReaderValue<vector<float>> n_dqOverP_MSO_o(reader_old, "n_dqOverP_MSO");
  TTreeReaderValue<vector<float>> n_dqOverP_MSOE_o(reader_old, "n_dqOverP_MSOE");
  TTreeReaderValue<vector<float>> n_dqOverP_ID_o(reader_old, "n_dqOverP_ID");
  TTreeReaderValue<vector<float>> n_rchi2_CB_o(reader_old, "n_rchi2_CB");
  TTreeReaderValue<vector<float>> n_rchi2_ME_o(reader_old, "n_rchi2_ME");
  TTreeReaderValue<vector<float>> n_rchi2_MSO_o(reader_old, "n_rchi2_MSO");
  TTreeReaderValue<vector<float>> n_rchi2_MSOE_o(reader_old, "n_rchi2_MSOE");
  TTreeReaderValue<vector<float>> n_rchi2_ID_o(reader_old, "n_rchi2_ID");
  
  // loop through TTreeReader
  while (reader_old.Next()) {
    nEvents_o++;
    for (int i = 0; i < *nPositiveMuons_o; i++) {
      if (i == 0) h_quality_o->Fill(p_quality_o->at(i));
      h_allQuality_o->Fill(p_quality_o->at(i));
    }
    for (int i = 0; i < *nNegativeMuons_o; i++) {
      if (i == 0) h_quality_o->Fill(n_quality_o->at(i));
      h_allQuality_o->Fill(n_quality_o->at(i));
    }

    // skip event if event not on GRL
    if (!*passGRL_o) continue;
 
    // skip event if no pairs are found in event
    if (*nPositiveMuons_o == 0 || *nNegativeMuons_o == 0) continue;

    // skip event if muons are not tight or medium quality 
    if (p_quality_o->at(0) > 1 || n_quality_o->at(0) > 1) continue;

    // skip event if muons fail ID cuts
    if (!p_passIDcuts_o->at(0) || !n_passIDcuts_o->at(0))  continue;

    // find charge of leading muon
    double pt_p, pt_n;
    int q_lead = -1;
    if (p_isCB_o->at(0)) pt_p = cosh(p_eta_CB_o->at(0)) / p_qOverP_CB_o->at(0);
    else pt_p = cosh(p_eta_ME_o->at(0)) / p_qOverP_ME_o->at(0);
    if (n_isCB_o->at(0)) pt_n = -cosh(n_eta_CB_o->at(0)) / n_qOverP_CB_o->at(0);
    else pt_n = -cosh(n_eta_ME_o->at(0)) / n_qOverP_ME_o->at(0);
    
    if (pt_p > pt_n) {
      nLeadingPlus_o++;
      q_lead = 1;
    }
  
    // skip events where selected muons fail pt and isolation requirements
    if (q_lead == 1) {
      if (pt_p < 25) continue;
      if (pt_n < 20) continue;
    }
    else {
      if (pt_n < 25) continue;
      if (pt_p < 20) continue;
    }
    if (pt_p / p_ptcone40_o->at(0) < .3) continue;
    if (pt_n / n_ptcone40_o->at(0) < .3) continue;
    
    if (1./p_qOverP_MSO_o->at(0) > 0 and 1./p_qOverP_MSO_o->at(0) < 10000){
      nEvents_noBadMS_o++;
      if (q_lead == 1) 
        nLeadingPlus_noBadMS_o++;
    }
 
    // fill primary author
    if (q_lead == 1) {
      h_author1_o->Fill(p_primaryAuthor_o->at(0)); 
      h_author2_o->Fill(n_primaryAuthor_o->at(0)); 
    }
    else {
      h_author1_o->Fill(n_primaryAuthor_o->at(0)); 
      h_author2_o->Fill(p_primaryAuthor_o->at(0)); 
    }
  
    // fill invariant mass
    TLorentzVector v1, v2;
    if (p_isCB_o->at(0) and n_isCB_o->at(0)) {
      v1.SetPtEtaPhiM(cosh(p_eta_CB_o->at(0)) / p_qOverP_CB_o->at(0), p_eta_CB_o->at(0), p_phi_CB_o->at(0), .106);
      v2.SetPtEtaPhiM(-cosh(n_eta_CB_o->at(0)) / n_qOverP_CB_o->at(0), n_eta_CB_o->at(0), n_phi_CB_o->at(0), .106); 
      h_m_CB_o->Fill((v1+v2).M());
    }
    if (p_isME_o->at(0) and n_isME_o->at(0)) {
      v1.SetPtEtaPhiM(cosh(p_eta_ME_o->at(0)) / p_qOverP_ME_o->at(0), p_eta_ME_o->at(0), p_phi_ME_o->at(0), .106);
      v2.SetPtEtaPhiM(-cosh(n_eta_ME_o->at(0)) / n_qOverP_ME_o->at(0), n_eta_ME_o->at(0), n_phi_ME_o->at(0), .106); 
      h_m_ME_o->Fill((v1+v2).M());
    }
    if (p_isMSO_o->at(0) and n_isMSO_o->at(0)) {
      v1.SetPtEtaPhiM(cosh(p_eta_MSO_o->at(0)) / p_qOverP_MSO_o->at(0), p_eta_MSO_o->at(0), p_phi_MSO_o->at(0), .106);
      v2.SetPtEtaPhiM(-cosh(n_eta_MSO_o->at(0)) / n_qOverP_MSO_o->at(0), n_eta_MSO_o->at(0), n_phi_MSO_o->at(0), .106); 
      h_m_MSO_o->Fill((v1+v2).M());
    }
    if (p_isMSOE_o->at(0) and n_isMSOE_o->at(0)) {
      v1.SetPtEtaPhiM(cosh(p_eta_MSOE_o->at(0)) / p_qOverP_MSOE_o->at(0), p_eta_MSOE_o->at(0), p_phi_MSOE_o->at(0), .106);
      v2.SetPtEtaPhiM(-cosh(n_eta_MSOE_o->at(0)) / n_qOverP_MSOE_o->at(0), n_eta_MSOE_o->at(0), n_phi_MSOE_o->at(0), .106); 
      h_m_MSOE_o->Fill((v1+v2).M());
    }
    if (p_isID_o->at(0) and n_isID_o->at(0)) {
      v1.SetPtEtaPhiM(cosh(p_eta_ID_o->at(0)) / p_qOverP_ID_o->at(0), p_eta_ID_o->at(0), p_phi_ID_o->at(0), .106);
      v2.SetPtEtaPhiM(-cosh(n_eta_ID_o->at(0)) / n_qOverP_ID_o->at(0), n_eta_ID_o->at(0), n_phi_ID_o->at(0), .106); 
      h_m_ID_o->Fill((v1+v2).M());
    }
 
    // fill leading/subleading variable histograms
    if (q_lead == 1) {
      if (p_isCB_o->at(0)) {
        h_pt1_CB_o->Fill(cosh(p_eta_CB_o->at(0)) / p_qOverP_CB_o->at(0)); 
        h_eta1_CB_o->Fill(p_eta_CB_o->at(0));
      }
      if (n_isCB_o->at(0)) {
        h_pt2_CB_o->Fill(-cosh(n_eta_CB_o->at(0)) / n_qOverP_CB_o->at(0)); 
        h_eta2_CB_o->Fill(n_eta_CB_o->at(0));
      }
    
      if (p_isME_o->at(0)) {
        h_pt1_ME_o->Fill(cosh(p_eta_ME_o->at(0)) / p_qOverP_ME_o->at(0)); 
        h_eta1_ME_o->Fill(p_eta_ME_o->at(0));
      }
      if (n_isME_o->at(0)) {
        h_pt2_ME_o->Fill(-cosh(n_eta_ME_o->at(0)) / n_qOverP_ME_o->at(0)); 
        h_eta2_ME_o->Fill(n_eta_ME_o->at(0));
      }
    
      if (p_isMSO_o->at(0)) {
        h_pt1_MSO_o->Fill(cosh(p_eta_MSO_o->at(0)) / p_qOverP_MSO_o->at(0)); 
        h_eta1_MSO_o->Fill(p_eta_MSO_o->at(0));
      }
      if (n_isMSO_o->at(0)) {
        h_pt2_MSO_o->Fill(-cosh(n_eta_MSO_o->at(0)) / n_qOverP_MSO_o->at(0)); 
        h_eta2_MSO_o->Fill(n_eta_MSO_o->at(0));
      }
    
      if (p_isMSOE_o->at(0)) {
        h_pt1_MSOE_o->Fill(cosh(p_eta_MSOE_o->at(0)) / p_qOverP_MSOE_o->at(0)); 
        h_eta1_MSOE_o->Fill(p_eta_MSOE_o->at(0));
      }
      if (n_isMSOE_o->at(0)) {
        h_pt2_MSOE_o->Fill(-cosh(n_eta_MSOE_o->at(0)) / n_qOverP_MSOE_o->at(0)); 
        h_eta2_MSOE_o->Fill(n_eta_MSOE_o->at(0));
      }
    
      if (p_isID_o->at(0)) {
        h_pt1_ID_o->Fill(cosh(p_eta_ID_o->at(0)) / p_qOverP_ID_o->at(0)); 
        h_eta1_ID_o->Fill(p_eta_ID_o->at(0));
        if (p_isME_o->at(0)) {
          double ptID = cosh(p_eta_ID_o->at(0)) / p_qOverP_ID_o->at(0);
          double ptME = cosh(p_eta_ME_o->at(0)) / p_qOverP_ME_o->at(0);
          h_rho1_ME_o->Fill((ptME-ptID)/ptID);
          h_dR1_ME_o->Fill(sqrt( pow(p_eta_ID_o->at(0)-p_eta_ME_o->at(0),2) + pow(p_phi_ID_o->at(0)-p_phi_ME_o->at(0),2)));
        }
        if (p_isMSO_o->at(0)) {
          double ptID = cosh(p_eta_ID_o->at(0)) / p_qOverP_ID_o->at(0);
          double ptMSO = cosh(p_eta_MSO_o->at(0)) / p_qOverP_MSO_o->at(0);
          h_rho1_MSO_o->Fill((ptMSO-ptID)/ptID);
        }
        if (p_isMSOE_o->at(0))
          h_dR1_MSOE_o->Fill(sqrt( pow(p_eta_ID_o->at(0)-p_eta_MSOE_o->at(0),2) + pow(p_phi_ID_o->at(0)-p_phi_MSOE_o->at(0),2)));
      }
      if (n_isID_o->at(0)) {
        h_pt2_ID_o->Fill(-cosh(n_eta_ID_o->at(0)) / n_qOverP_ID_o->at(0)); 
        h_eta2_ID_o->Fill(n_eta_ID_o->at(0));
        if (n_isME_o->at(0)) {
          double ptID = cosh(n_eta_ID_o->at(0)) / n_qOverP_ID_o->at(0);
          double ptME = cosh(n_eta_ME_o->at(0)) / n_qOverP_ME_o->at(0);
          h_rho2_ME_o->Fill((ptME-ptID)/ptID);
          h_dR2_ME_o->Fill(sqrt( pow(n_eta_ID_o->at(0)-n_eta_ME_o->at(0),2) + pow(n_phi_ID_o->at(0)-n_phi_ME_o->at(0),2)));
        }
        if (n_isMSO_o->at(0)) {
          double ptID = cosh(n_eta_ID_o->at(0)) / n_qOverP_ID_o->at(0);
          double ptMSO = cosh(n_eta_MSO_o->at(0)) / n_qOverP_MSO_o->at(0);
          h_rho2_MSO_o->Fill((ptMSO-ptID)/ptID);
        }
        if (n_isMSOE_o->at(0))
          h_dR2_MSOE_o->Fill(sqrt( pow(n_eta_ID_o->at(0)-n_eta_MSOE_o->at(0),2) + pow(n_phi_ID_o->at(0)-n_phi_MSOE_o->at(0),2)));
      }
    }
    // if negative muon is primary
    else {
      if (p_isCB_o->at(0)) {
        h_pt2_CB_o->Fill(cosh(p_eta_CB_o->at(0)) / p_qOverP_CB_o->at(0)); 
        h_eta2_CB_o->Fill(p_eta_CB_o->at(0));
      }
      if (n_isCB_o->at(0)) {
        h_pt1_CB_o->Fill(-cosh(n_eta_CB_o->at(0)) / n_qOverP_CB_o->at(0)); 
        h_eta1_CB_o->Fill(n_eta_CB_o->at(0));
      }
    
      if (p_isME_o->at(0)) {
        h_pt2_ME_o->Fill(cosh(p_eta_ME_o->at(0)) / p_qOverP_ME_o->at(0)); 
        h_eta2_ME_o->Fill(p_eta_ME_o->at(0));
      }
      if (n_isME_o->at(0)) {
        h_pt1_ME_o->Fill(-cosh(n_eta_ME_o->at(0)) / n_qOverP_ME_o->at(0)); 
        h_eta1_ME_o->Fill(n_eta_ME_o->at(0));
      }
    
      if (p_isMSO_o->at(0)) {
        h_pt2_MSO_o->Fill(cosh(p_eta_MSO_o->at(0)) / p_qOverP_MSO_o->at(0)); 
        h_eta2_MSO_o->Fill(p_eta_MSO_o->at(0));
      }
      if (n_isMSO_o->at(0)) {
        h_pt1_MSO_o->Fill(-cosh(n_eta_MSO_o->at(0)) / n_qOverP_MSO_o->at(0)); 
        h_eta1_MSO_o->Fill(n_eta_MSO_o->at(0));
      }
    
      if (p_isMSOE_o->at(0)) {
        h_pt2_MSOE_o->Fill(cosh(p_eta_MSOE_o->at(0)) / p_qOverP_MSOE_o->at(0)); 
        h_eta2_MSOE_o->Fill(p_eta_MSOE_o->at(0));
      }
      if (n_isMSOE_o->at(0)) {
        h_pt1_MSOE_o->Fill(-cosh(n_eta_MSOE_o->at(0)) / n_qOverP_MSOE_o->at(0)); 
        h_eta1_MSOE_o->Fill(n_eta_MSOE_o->at(0));
      }
    
      if (p_isID_o->at(0)) {
        h_pt2_ID_o->Fill(cosh(p_eta_ID_o->at(0)) / p_qOverP_ID_o->at(0)); 
        h_eta2_ID_o->Fill(p_eta_ID_o->at(0));
        if (p_isME_o->at(0)) {
          double ptID = cosh(p_eta_ID_o->at(0)) / p_qOverP_ID_o->at(0);
          double ptME = cosh(p_eta_ME_o->at(0)) / p_qOverP_ME_o->at(0);
          h_rho2_ME_o->Fill((ptME-ptID)/ptID);
          h_dR2_ME_o->Fill(sqrt( pow(p_eta_ID_o->at(0)-p_eta_ME_o->at(0),2) + pow(p_phi_ID_o->at(0)-p_phi_ME_o->at(0),2)));
        }
        if (p_isMSO_o->at(0)) {
          double ptID = cosh(p_eta_ID_o->at(0)) / p_qOverP_ID_o->at(0);
          double ptMSO = cosh(p_eta_MSO_o->at(0)) / p_qOverP_MSO_o->at(0);
          h_rho2_MSO_o->Fill((ptMSO-ptID)/ptID);
        }
        if (p_isMSOE_o->at(0))
          h_dR2_MSOE_o->Fill(sqrt( pow(p_eta_ID_o->at(0)-p_eta_MSOE_o->at(0),2) + pow(p_phi_ID_o->at(0)-p_phi_MSOE_o->at(0),2)));
      }
      if (n_isID_o->at(0)) {
        h_pt1_ID_o->Fill(-cosh(n_eta_ID_o->at(0)) / n_qOverP_ID_o->at(0)); 
        h_eta1_ID_o->Fill(n_eta_ID_o->at(0));
        if (n_isME_o->at(0)) {
          double ptID = cosh(n_eta_ID_o->at(0)) / n_qOverP_ID_o->at(0);
          double ptME = cosh(n_eta_ME_o->at(0)) / n_qOverP_ME_o->at(0);
          h_rho1_ME_o->Fill((ptME-ptID)/ptID);
          h_dR1_ME_o->Fill(sqrt( pow(n_eta_ID_o->at(0)-n_eta_ME_o->at(0),2) + pow(n_phi_ID_o->at(0)-n_phi_ME_o->at(0),2)));
        }
        if (n_isMSO_o->at(0)) {
          double ptID = cosh(n_eta_ID_o->at(0)) / n_qOverP_ID_o->at(0);
          double ptMSO = cosh(n_eta_MSO_o->at(0)) / n_qOverP_MSO_o->at(0);
          h_rho1_MSO_o->Fill((ptMSO-ptID)/ptID);
        }
        if (n_isMSOE_o->at(0))
          h_dR1_MSOE_o->Fill(sqrt( pow(n_eta_ID_o->at(0)-n_eta_MSOE_o->at(0),2) + pow(n_phi_ID_o->at(0)-n_phi_MSOE_o->at(0),2)));
      }
    }
  }
  //delete f_recoOld;


  // load new file and set up reader
  //TFile* f_recoNew = new TFile(Form("ntuples_muonSelection/%s", newFileName.c_str()));
  //if (!f_recoNew) cout << "Warning: could not open file " << newFileName << endl;
  
  TChain chainNew("RecoMuons");
  std::ifstream recoNewFileNames("RunIfiles_test.txt");
  string fileNameNew;
  while (recoNewFileNames >> fileNameNew) 
    chainNew.Add(fileNameNew.c_str());
  
  //string newMapName = "2016";
  string newMapName = "Run I";

  long int nEvents_n = 0;
  long int nLeadingPlus_n = 0;
  long int nEvents_noBadMS_n = 0;
  long int nLeadingPlus_noBadMS_n = 0;

  // event level  
  //TTreeReader reader_new("RecoMuons", f_recoNew);
  TTreeReader reader_new(&chainNew);
  TTreeReaderValue<bool> passGRL_n(reader_new, "passGRL");
  TTreeReaderValue<unsigned long long> eventNumber_n(reader_new, "eventNumber");
  TTreeReaderValue<unsigned int> nPositiveMuons_n(reader_new, "nPositiveMuons");
  TTreeReaderValue<unsigned int> nNegativeMuons_n(reader_new, "nNegativeMuons");
  
  // positive muon variables
  TTreeReaderValue<vector<bool>> p_passIDcuts_n(reader_new, "p_passIDcuts");
  TTreeReaderValue<vector<bool>> p_passAll_n(reader_new, "p_passAll");
  TTreeReaderValue<vector<float>> p_eLoss_n(reader_new, "p_eLoss");
  TTreeReaderValue<vector<float>> p_ptcone40_n(reader_new, "p_ptcone40");
  TTreeReaderValue<vector<int>> p_muonType_n(reader_new, "p_muonType");
  TTreeReaderValue<vector<int>> p_quality_n(reader_new, "p_quality");
  TTreeReaderValue<vector<int>> p_primaryAuthor_n(reader_new, "p_primaryAuthor");
  TTreeReaderValue<vector<int>> p_authors_n(reader_new, "p_authors");
  TTreeReaderValue<vector<int>> p_nPrecisionLayers_n(reader_new, "p_nPrecisionLayers");
  TTreeReaderValue<vector<int>> p_nPrecisionHoleLayers_n(reader_new, "p_nPrecisionHoleLayers");
  TTreeReaderValue<vector<int>> p_innerSmallHits_n(reader_new, "p_innerSmallHits");
  TTreeReaderValue<vector<int>> p_innerLargeHits_n(reader_new, "p_innerLargeHits");
  TTreeReaderValue<vector<int>> p_middleSmallHits_n(reader_new, "p_middleSmallHits");
  TTreeReaderValue<vector<int>> p_middleLargeHits_n(reader_new, "p_middleLargeHits");
  TTreeReaderValue<vector<int>> p_outerSmallHits_n(reader_new, "p_outerSmallHits");
  TTreeReaderValue<vector<int>> p_outerLargeHits_n(reader_new, "p_outerLargeHits");
  TTreeReaderValue<vector<int>> p_extendedSmallHits_n(reader_new, "p_extendedSmallHits");
  TTreeReaderValue<vector<int>> p_extendedLargeHits_n(reader_new, "p_extendedLargeHits"); 
  TTreeReaderValue<vector<int>> p_innerSmallHoles_n(reader_new, "p_innerSmallHoles");
  TTreeReaderValue<vector<int>> p_innerLargeHoles_n(reader_new, "p_innerLargeHoles");
  TTreeReaderValue<vector<int>> p_middleSmallHoles_n(reader_new, "p_middleSmallHoles");
  TTreeReaderValue<vector<int>> p_middleLargeHoles_n(reader_new, "p_middleLargeHoles");
  TTreeReaderValue<vector<int>> p_outerSmallHoles_n(reader_new, "p_outerSmallHoles");
  TTreeReaderValue<vector<int>> p_outerLargeHoles_n(reader_new, "p_outerLargeHoles");
  TTreeReaderValue<vector<int>> p_extendedSmallHoles_n(reader_new, "p_extendedSmallHoles");
  TTreeReaderValue<vector<int>> p_extendedLargeHoles_n(reader_new, "p_extendedLargeHoles");

  // negative muon variables
  TTreeReaderValue<vector<bool>> n_passIDcuts_n(reader_new, "n_passIDcuts");
  TTreeReaderValue<vector<bool>> n_passAll_n(reader_new, "n_passAll");
  TTreeReaderValue<vector<float>> n_eLoss_n(reader_new, "n_eLoss");
  TTreeReaderValue<vector<float>> n_ptcone40_n(reader_new, "n_ptcone40");
  TTreeReaderValue<vector<int>> n_muonType_n(reader_new, "n_muonType");
  TTreeReaderValue<vector<int>> n_quality_n(reader_new, "n_quality");
  TTreeReaderValue<vector<int>> n_primaryAuthor_n(reader_new, "n_primaryAuthor");
  TTreeReaderValue<vector<int>> n_authors_n(reader_new, "n_authors");
  TTreeReaderValue<vector<int>> n_nPrecisionLayers_n(reader_new, "n_nPrecisionLayers");
  TTreeReaderValue<vector<int>> n_nPrecisionHoleLayers_n(reader_new, "n_nPrecisionHoleLayers");
  TTreeReaderValue<vector<int>> n_innerSmallHits_n(reader_new, "n_innerSmallHits");
  TTreeReaderValue<vector<int>> n_innerLargeHits_n(reader_new, "n_innerLargeHits");
  TTreeReaderValue<vector<int>> n_middleSmallHits_n(reader_new, "n_middleSmallHits");
  TTreeReaderValue<vector<int>> n_middleLargeHits_n(reader_new, "n_middleLargeHits");
  TTreeReaderValue<vector<int>> n_outerSmallHits_n(reader_new, "n_outerSmallHits");
  TTreeReaderValue<vector<int>> n_outerLargeHits_n(reader_new, "n_outerLargeHits");
  TTreeReaderValue<vector<int>> n_extendedSmallHits_n(reader_new, "n_extendedSmallHits");
  TTreeReaderValue<vector<int>> n_extendedLargeHits_n(reader_new, "n_extendedLargeHits"); 
  TTreeReaderValue<vector<int>> n_innerSmallHoles_n(reader_new, "n_innerSmallHoles");
  TTreeReaderValue<vector<int>> n_innerLargeHoles_n(reader_new, "n_innerLargeHoles");
  TTreeReaderValue<vector<int>> n_middleSmallHoles_n(reader_new, "n_middleSmallHoles");
  TTreeReaderValue<vector<int>> n_middleLargeHoles_n(reader_new, "n_middleLargeHoles");
  TTreeReaderValue<vector<int>> n_outerSmallHoles_n(reader_new, "n_outerSmallHoles");
  TTreeReaderValue<vector<int>> n_outerLargeHoles_n(reader_new, "n_outerLargeHoles");
  TTreeReaderValue<vector<int>> n_extendedSmallHoles_n(reader_new, "n_extendedSmallHoles");
  TTreeReaderValue<vector<int>> n_extendedLargeHoles_n(reader_new, "n_extendedLargeHoles");

  // positive muon track variables
  TTreeReaderValue<vector<bool>> p_isCB_n(reader_new, "p_isCB");
  TTreeReaderValue<vector<bool>> p_isME_n(reader_new, "p_isME");
  TTreeReaderValue<vector<bool>> p_isMSO_n(reader_new, "p_isMSO");
  TTreeReaderValue<vector<bool>> p_isMSOE_n(reader_new, "p_isMSOE");
  TTreeReaderValue<vector<bool>> p_isID_n(reader_new, "p_isID");
  TTreeReaderValue<vector<float>> p_eta_CB_n(reader_new, "p_eta_CB");
  TTreeReaderValue<vector<float>> p_eta_ME_n(reader_new, "p_eta_ME");
  TTreeReaderValue<vector<float>> p_eta_MSO_n(reader_new, "p_eta_MSO");
  TTreeReaderValue<vector<float>> p_eta_MSOE_n(reader_new, "p_eta_MSOE");
  TTreeReaderValue<vector<float>> p_eta_ID_n(reader_new, "p_eta_ID");
  TTreeReaderValue<vector<float>> p_phi_CB_n(reader_new, "p_phi_CB");
  TTreeReaderValue<vector<float>> p_phi_ME_n(reader_new, "p_phi_ME");
  TTreeReaderValue<vector<float>> p_phi_MSO_n(reader_new, "p_phi_MSO");
  TTreeReaderValue<vector<float>> p_phi_MSOE_n(reader_new, "p_phi_MSOE");
  TTreeReaderValue<vector<float>> p_phi_ID_n(reader_new, "p_phi_ID");
  TTreeReaderValue<vector<float>> p_d0sig_CB_n(reader_new, "p_d0sig_CB");
  TTreeReaderValue<vector<float>> p_d0sig_ME_n(reader_new, "p_d0sig_ME");
  TTreeReaderValue<vector<float>> p_d0sig_MSO_n(reader_new, "p_d0sig_MSO");
  TTreeReaderValue<vector<float>> p_d0sig_MSOE_n(reader_new, "p_d0sig_MSOE");
  TTreeReaderValue<vector<float>> p_d0sig_ID_n(reader_new, "p_d0sig_ID");
  TTreeReaderValue<vector<float>> p_z0_CB_n(reader_new, "p_z0_CB");
  TTreeReaderValue<vector<float>> p_z0_ME_n(reader_new, "p_z0_ME");
  TTreeReaderValue<vector<float>> p_z0_MSO_n(reader_new, "p_z0_MSO");
  TTreeReaderValue<vector<float>> p_z0_MSOE_n(reader_new, "p_z0_MSOE");
  TTreeReaderValue<vector<float>> p_z0_ID_n(reader_new, "p_z0_ID");
  TTreeReaderValue<vector<float>> p_qOverP_CB_n(reader_new, "p_qOverP_CB");
  TTreeReaderValue<vector<float>> p_qOverP_ME_n(reader_new, "p_qOverP_ME");
  TTreeReaderValue<vector<float>> p_qOverP_MSO_n(reader_new, "p_qOverP_MSO");
  TTreeReaderValue<vector<float>> p_qOverP_MSOE_n(reader_new, "p_qOverP_MSOE");
  TTreeReaderValue<vector<float>> p_qOverP_ID_n(reader_new, "p_qOverP_ID");
  TTreeReaderValue<vector<float>> p_dqOverP_CB_n(reader_new, "p_dqOverP_CB");
  TTreeReaderValue<vector<float>> p_dqOverP_ME_n(reader_new, "p_dqOverP_ME");
  TTreeReaderValue<vector<float>> p_dqOverP_MSO_n(reader_new, "p_dqOverP_MSO");
  TTreeReaderValue<vector<float>> p_dqOverP_MSOE_n(reader_new, "p_dqOverP_MSOE");
  TTreeReaderValue<vector<float>> p_dqOverP_ID_n(reader_new, "p_dqOverP_ID");
  TTreeReaderValue<vector<float>> p_rchi2_CB_n(reader_new, "p_rchi2_CB");
  TTreeReaderValue<vector<float>> p_rchi2_ME_n(reader_new, "p_rchi2_ME");
  TTreeReaderValue<vector<float>> p_rchi2_MSO_n(reader_new, "p_rchi2_MSO");
  TTreeReaderValue<vector<float>> p_rchi2_MSOE_n(reader_new, "p_rchi2_MSOE");
  TTreeReaderValue<vector<float>> p_rchi2_ID_n(reader_new, "p_rchi2_ID");
  
  // negative muon track variables
  TTreeReaderValue<vector<bool>> n_isCB_n(reader_new, "n_isCB");
  TTreeReaderValue<vector<bool>> n_isME_n(reader_new, "n_isME");
  TTreeReaderValue<vector<bool>> n_isMSO_n(reader_new, "n_isMSO");
  TTreeReaderValue<vector<bool>> n_isMSOE_n(reader_new, "n_isMSOE");
  TTreeReaderValue<vector<bool>> n_isID_n(reader_new, "n_isID");
  TTreeReaderValue<vector<float>> n_eta_CB_n(reader_new, "n_eta_CB");
  TTreeReaderValue<vector<float>> n_eta_ME_n(reader_new, "n_eta_ME");
  TTreeReaderValue<vector<float>> n_eta_MSO_n(reader_new, "n_eta_MSO");
  TTreeReaderValue<vector<float>> n_eta_MSOE_n(reader_new, "n_eta_MSOE");
  TTreeReaderValue<vector<float>> n_eta_ID_n(reader_new, "n_eta_ID");
  TTreeReaderValue<vector<float>> n_phi_CB_n(reader_new, "n_phi_CB");
  TTreeReaderValue<vector<float>> n_phi_ME_n(reader_new, "n_phi_ME");
  TTreeReaderValue<vector<float>> n_phi_MSO_n(reader_new, "n_phi_MSO");
  TTreeReaderValue<vector<float>> n_phi_MSOE_n(reader_new, "n_phi_MSOE");
  TTreeReaderValue<vector<float>> n_phi_ID_n(reader_new, "n_phi_ID");
  TTreeReaderValue<vector<float>> n_d0sig_CB_n(reader_new, "n_d0sig_CB");
  TTreeReaderValue<vector<float>> n_d0sig_ME_n(reader_new, "n_d0sig_ME");
  TTreeReaderValue<vector<float>> n_d0sig_MSO_n(reader_new, "n_d0sig_MSO");
  TTreeReaderValue<vector<float>> n_d0sig_MSOE_n(reader_new, "n_d0sig_MSOE");
  TTreeReaderValue<vector<float>> n_d0sig_ID_n(reader_new, "n_d0sig_ID");
  TTreeReaderValue<vector<float>> n_z0_CB_n(reader_new, "n_z0_CB");
  TTreeReaderValue<vector<float>> n_z0_ME_n(reader_new, "n_z0_ME");
  TTreeReaderValue<vector<float>> n_z0_MSO_n(reader_new, "n_z0_MSO");
  TTreeReaderValue<vector<float>> n_z0_MSOE_n(reader_new, "n_z0_MSOE");
  TTreeReaderValue<vector<float>> n_z0_ID_n(reader_new, "n_z0_ID");
  TTreeReaderValue<vector<float>> n_qOverP_CB_n(reader_new, "n_qOverP_CB");
  TTreeReaderValue<vector<float>> n_qOverP_ME_n(reader_new, "n_qOverP_ME");
  TTreeReaderValue<vector<float>> n_qOverP_MSO_n(reader_new, "n_qOverP_MSO");
  TTreeReaderValue<vector<float>> n_qOverP_MSOE_n(reader_new, "n_qOverP_MSOE");
  TTreeReaderValue<vector<float>> n_qOverP_ID_n(reader_new, "n_qOverP_ID");
  TTreeReaderValue<vector<float>> n_dqOverP_CB_n(reader_new, "n_dqOverP_CB");
  TTreeReaderValue<vector<float>> n_dqOverP_ME_n(reader_new, "n_dqOverP_ME");
  TTreeReaderValue<vector<float>> n_dqOverP_MSO_n(reader_new, "n_dqOverP_MSO");
  TTreeReaderValue<vector<float>> n_dqOverP_MSOE_n(reader_new, "n_dqOverP_MSOE");
  TTreeReaderValue<vector<float>> n_dqOverP_ID_n(reader_new, "n_dqOverP_ID");
  TTreeReaderValue<vector<float>> n_rchi2_CB_n(reader_new, "n_rchi2_CB");
  TTreeReaderValue<vector<float>> n_rchi2_ME_n(reader_new, "n_rchi2_ME");
  TTreeReaderValue<vector<float>> n_rchi2_MSO_n(reader_new, "n_rchi2_MSO");
  TTreeReaderValue<vector<float>> n_rchi2_MSOE_n(reader_new, "n_rchi2_MSOE");
  TTreeReaderValue<vector<float>> n_rchi2_ID_n(reader_new, "n_rchi2_ID");
  
  // loop through TTreeReader
  while (reader_new.Next()) {
    nEvents_n++;
    for (int i = 0; i < *nPositiveMuons_n; i++) {
      if (i == 0) h_quality_n->Fill(p_quality_n->at(i));
      h_allQuality_n->Fill(p_quality_n->at(i));
    }
    for (int i = 0; i < *nNegativeMuons_n; i++) {
      if (i == 0) h_quality_n->Fill(n_quality_n->at(i));
      h_allQuality_n->Fill(n_quality_n->at(i));
    }

    // skip event if event not on GRL
    if (!*passGRL_n) continue;
 
    // skip event if no pairs are found in event
    if (*nPositiveMuons_n == 0 || *nNegativeMuons_n == 0) continue;

    // skip event if muons are not tight or medium quality 
    if (p_quality_n->at(0) > 1 || n_quality_n->at(0) > 1) continue;

    // skip event if muons fail ID cuts
    if (!p_passIDcuts_n->at(0) || !n_passIDcuts_n->at(0))  continue;

    // find charge of leading muon
    double pt_p, pt_n;
    int q_lead = -1;
    if (p_isCB_n->at(0)) pt_p = cosh(p_eta_CB_n->at(0)) / p_qOverP_CB_n->at(0);
    else pt_p = cosh(p_eta_ME_n->at(0)) / p_qOverP_ME_n->at(0);
    if (n_isCB_n->at(0)) pt_n = -cosh(n_eta_CB_n->at(0)) / n_qOverP_CB_n->at(0);
    else pt_n = -cosh(n_eta_ME_n->at(0)) / n_qOverP_ME_n->at(0);
    
    if (pt_p > pt_n) {
      nLeadingPlus_n++;
      q_lead = 1;
    }

    // skip events where selected muons fail pt and isolation requirements
    if (q_lead == 1) {
      if (pt_p < 25) continue;
      if (pt_n < 20) continue;
    }
    else {
      if (pt_n < 25) continue;
      if (pt_p < 20) continue;
    }
    if (pt_p / p_ptcone40_n->at(0) < .3) continue;
    if (pt_n / n_ptcone40_n->at(0) < .3) continue;
    
    if (1./p_qOverP_MSO_n->at(0) > 0 and 1./p_qOverP_MSO_n->at(0) < 10000){
      nEvents_noBadMS_n++;
      if (q_lead == 1) 
        nLeadingPlus_noBadMS_n++;
    }
    
    // fill primary author
    if (q_lead == 1) {
      h_author1_n->Fill(p_primaryAuthor_n->at(0)); 
      h_author2_n->Fill(n_primaryAuthor_n->at(0)); 
    }
    else {
      h_author1_n->Fill(n_primaryAuthor_n->at(0)); 
      h_author2_n->Fill(p_primaryAuthor_n->at(0)); 
    }
  
    // fill invariant mass
    TLorentzVector v1, v2;
    if (p_isCB_n->at(0) and n_isCB_n->at(0)) {
      v1.SetPtEtaPhiM(cosh(p_eta_CB_n->at(0)) / p_qOverP_CB_n->at(0), p_eta_CB_n->at(0), p_phi_CB_n->at(0), .106);
      v2.SetPtEtaPhiM(-cosh(n_eta_CB_n->at(0)) / n_qOverP_CB_n->at(0), n_eta_CB_n->at(0), n_phi_CB_n->at(0), .106); 
      h_m_CB_n->Fill((v1+v2).M());
    }
    if (p_isME_n->at(0) and n_isME_n->at(0)) {
      v1.SetPtEtaPhiM(cosh(p_eta_ME_n->at(0)) / p_qOverP_ME_n->at(0), p_eta_ME_n->at(0), p_phi_ME_n->at(0), .106);
      v2.SetPtEtaPhiM(-cosh(n_eta_ME_n->at(0)) / n_qOverP_ME_n->at(0), n_eta_ME_n->at(0), n_phi_ME_n->at(0), .106); 
      h_m_ME_n->Fill((v1+v2).M());
    }
    if (p_isMSO_n->at(0) and n_isMSO_n->at(0)) {
      v1.SetPtEtaPhiM(cosh(p_eta_MSO_n->at(0)) / p_qOverP_MSO_n->at(0), p_eta_MSO_n->at(0), p_phi_MSO_n->at(0), .106);
      v2.SetPtEtaPhiM(-cosh(n_eta_MSO_n->at(0)) / n_qOverP_MSO_n->at(0), n_eta_MSO_n->at(0), n_phi_MSO_n->at(0), .106); 
      h_m_MSO_n->Fill((v1+v2).M());
    }
    if (p_isMSOE_n->at(0) and n_isMSOE_n->at(0)) {
      v1.SetPtEtaPhiM(cosh(p_eta_MSOE_n->at(0)) / p_qOverP_MSOE_n->at(0), p_eta_MSOE_n->at(0), p_phi_MSOE_n->at(0), .106);
      v2.SetPtEtaPhiM(-cosh(n_eta_MSOE_n->at(0)) / n_qOverP_MSOE_n->at(0), n_eta_MSOE_n->at(0), n_phi_MSOE_n->at(0), .106); 
      h_m_MSOE_n->Fill((v1+v2).M());
    }
    if (p_isID_n->at(0) and n_isID_n->at(0)) {
      v1.SetPtEtaPhiM(cosh(p_eta_ID_n->at(0)) / p_qOverP_ID_n->at(0), p_eta_ID_n->at(0), p_phi_ID_n->at(0), .106);
      v2.SetPtEtaPhiM(-cosh(n_eta_ID_n->at(0)) / n_qOverP_ID_n->at(0), n_eta_ID_n->at(0), n_phi_ID_n->at(0), .106); 
      h_m_ID_n->Fill((v1+v2).M());
    }
 
    // fill leading/subleading variable histograms
    if (q_lead == 1) {
      if (p_isCB_n->at(0)) {
        h_pt1_CB_n->Fill(cosh(p_eta_CB_n->at(0)) / p_qOverP_CB_n->at(0)); 
        h_eta1_CB_n->Fill(p_eta_CB_n->at(0));
      }
      if (n_isCB_n->at(0)) {
        h_pt2_CB_n->Fill(-cosh(n_eta_CB_n->at(0)) / n_qOverP_CB_n->at(0)); 
        h_eta2_CB_n->Fill(n_eta_CB_n->at(0));
      }
    
      if (p_isME_n->at(0)) {
        h_pt1_ME_n->Fill(cosh(p_eta_ME_n->at(0)) / p_qOverP_ME_n->at(0)); 
        h_eta1_ME_n->Fill(p_eta_ME_n->at(0));
      }
      if (n_isME_n->at(0)) {
        h_pt2_ME_n->Fill(-cosh(n_eta_ME_n->at(0)) / n_qOverP_ME_n->at(0)); 
        h_eta2_ME_n->Fill(n_eta_ME_n->at(0));
      }
    
      if (p_isMSO_n->at(0)) {
        h_pt1_MSO_n->Fill(cosh(p_eta_MSO_n->at(0)) / p_qOverP_MSO_n->at(0)); 
        h_eta1_MSO_n->Fill(p_eta_MSO_n->at(0));
      }
      if (n_isMSO_n->at(0)) {
        h_pt2_MSO_n->Fill(-cosh(n_eta_MSO_n->at(0)) / n_qOverP_MSO_n->at(0)); 
        h_eta2_MSO_n->Fill(n_eta_MSO_n->at(0));
      }
    
      if (p_isMSOE_n->at(0)) {
        h_pt1_MSOE_n->Fill(cosh(p_eta_MSOE_n->at(0)) / p_qOverP_MSOE_n->at(0)); 
        h_eta1_MSOE_n->Fill(p_eta_MSOE_n->at(0));
      }
      if (n_isMSOE_n->at(0)) {
        h_pt2_MSOE_n->Fill(-cosh(n_eta_MSOE_n->at(0)) / n_qOverP_MSOE_n->at(0)); 
        h_eta2_MSOE_n->Fill(n_eta_MSOE_n->at(0));
      }
    
      if (p_isID_n->at(0)) {
        h_pt1_ID_n->Fill(cosh(p_eta_ID_n->at(0)) / p_qOverP_ID_n->at(0)); 
        h_eta1_ID_n->Fill(p_eta_ID_n->at(0));
        if (p_isME_n->at(0)) {
          double ptID = cosh(p_eta_ID_n->at(0)) / p_qOverP_ID_n->at(0);
          double ptME = cosh(p_eta_ME_n->at(0)) / p_qOverP_ME_n->at(0);
          h_rho1_ME_n->Fill((ptME-ptID)/ptID);
          h_dR1_ME_n->Fill(sqrt( pow(p_eta_ID_n->at(0)-p_eta_ME_n->at(0),2) + pow(p_phi_ID_n->at(0)-p_phi_ME_n->at(0),2)));
        }
        if (p_isMSO_n->at(0)) {
          double ptID = cosh(p_eta_ID_n->at(0)) / p_qOverP_ID_n->at(0);
          double ptMSO = cosh(p_eta_MSO_n->at(0)) / p_qOverP_MSO_n->at(0);
          h_rho1_MSO_n->Fill((ptMSO-ptID)/ptID);
        }
        if (p_isMSOE_n->at(0))
          h_dR1_MSOE_n->Fill(sqrt( pow(p_eta_ID_n->at(0)-p_eta_MSOE_n->at(0),2) + pow(p_phi_ID_n->at(0)-p_phi_MSOE_n->at(0),2)));
      }
      if (n_isID_n->at(0)) {
        h_pt2_ID_n->Fill(-cosh(n_eta_ID_n->at(0)) / n_qOverP_ID_n->at(0)); 
        h_eta2_ID_n->Fill(n_eta_ID_n->at(0));
        if (n_isME_n->at(0)) {
          double ptID = cosh(n_eta_ID_n->at(0)) / n_qOverP_ID_n->at(0);
          double ptME = cosh(n_eta_ME_n->at(0)) / n_qOverP_ME_n->at(0);
          h_rho2_ME_n->Fill((ptME-ptID)/ptID);
          h_dR2_ME_n->Fill(sqrt( pow(n_eta_ID_n->at(0)-n_eta_ME_n->at(0),2) + pow(n_phi_ID_n->at(0)-n_phi_ME_n->at(0),2)));
        }
        if (n_isMSO_n->at(0)) {
          double ptID = cosh(n_eta_ID_n->at(0)) / n_qOverP_ID_n->at(0);
          double ptMSO = cosh(n_eta_MSO_n->at(0)) / n_qOverP_MSO_n->at(0);
          h_rho2_MSO_n->Fill((ptMSO-ptID)/ptID);
        }
        if (n_isMSOE_n->at(0))
          h_dR2_MSOE_n->Fill(sqrt( pow(n_eta_ID_n->at(0)-n_eta_MSOE_n->at(0),2) + pow(n_phi_ID_n->at(0)-n_phi_MSOE_n->at(0),2)));
      }
    }
    // if negative muon is primary
    else {
      if (p_isCB_n->at(0)) {
        h_pt2_CB_n->Fill(cosh(p_eta_CB_n->at(0)) / p_qOverP_CB_n->at(0)); 
        h_eta2_CB_n->Fill(p_eta_CB_n->at(0));
      }
      if (n_isCB_n->at(0)) {
        h_pt1_CB_n->Fill(-cosh(n_eta_CB_n->at(0)) / n_qOverP_CB_n->at(0)); 
        h_eta1_CB_n->Fill(n_eta_CB_n->at(0));
      }
    
      if (p_isME_n->at(0)) {
        h_pt2_ME_n->Fill(cosh(p_eta_ME_n->at(0)) / p_qOverP_ME_n->at(0)); 
        h_eta2_ME_n->Fill(p_eta_ME_n->at(0));
      }
      if (n_isME_n->at(0)) {
        h_pt1_ME_n->Fill(-cosh(n_eta_ME_n->at(0)) / n_qOverP_ME_n->at(0)); 
        h_eta1_ME_n->Fill(n_eta_ME_n->at(0));
      }
    
      if (p_isMSO_n->at(0)) {
        h_pt2_MSO_n->Fill(cosh(p_eta_MSO_n->at(0)) / p_qOverP_MSO_n->at(0)); 
        h_eta2_MSO_n->Fill(p_eta_MSO_n->at(0));
      }
      if (n_isMSO_n->at(0)) {
        h_pt1_MSO_n->Fill(-cosh(n_eta_MSO_n->at(0)) / n_qOverP_MSO_n->at(0)); 
        h_eta1_MSO_n->Fill(n_eta_MSO_n->at(0));
      }
    
      if (p_isMSOE_n->at(0)) {
        h_pt2_MSOE_n->Fill(cosh(p_eta_MSOE_n->at(0)) / p_qOverP_MSOE_n->at(0)); 
        h_eta2_MSOE_n->Fill(p_eta_MSOE_n->at(0));
      }
      if (n_isMSOE_n->at(0)) {
        h_pt1_MSOE_n->Fill(-cosh(n_eta_MSOE_n->at(0)) / n_qOverP_MSOE_n->at(0)); 
        h_eta1_MSOE_n->Fill(n_eta_MSOE_n->at(0));
      }
    
      if (p_isID_n->at(0)) {
        h_pt2_ID_n->Fill(cosh(p_eta_ID_n->at(0)) / p_qOverP_ID_n->at(0)); 
        h_eta2_ID_n->Fill(p_eta_ID_n->at(0));
        if (p_isME_n->at(0)) {
          double ptID = cosh(p_eta_ID_n->at(0)) / p_qOverP_ID_n->at(0);
          double ptME = cosh(p_eta_ME_n->at(0)) / p_qOverP_ME_n->at(0);
          h_rho2_ME_n->Fill((ptME-ptID)/ptID);
          h_dR2_ME_n->Fill(sqrt( pow(p_eta_ID_n->at(0)-p_eta_ME_n->at(0),2) + pow(p_phi_ID_n->at(0)-p_phi_ME_n->at(0),2)));
        }
        if (p_isMSO_n->at(0)) {
          double ptID = cosh(p_eta_ID_n->at(0)) / p_qOverP_ID_n->at(0);
          double ptMSO = cosh(p_eta_MSO_n->at(0)) / p_qOverP_MSO_n->at(0);
          h_rho2_MSO_n->Fill((ptMSO-ptID)/ptID);
        }
        if (p_isMSOE_n->at(0))
          h_dR2_MSOE_n->Fill(sqrt( pow(p_eta_ID_n->at(0)-p_eta_MSOE_n->at(0),2) + pow(p_phi_ID_n->at(0)-p_phi_MSOE_n->at(0),2)));
      }
      if (n_isID_n->at(0)) {
        h_pt1_ID_n->Fill(-cosh(n_eta_ID_n->at(0)) / n_qOverP_ID_n->at(0)); 
        h_eta1_ID_n->Fill(n_eta_ID_n->at(0));
        if (n_isME_n->at(0)) {
          double ptID = cosh(n_eta_ID_n->at(0)) / n_qOverP_ID_n->at(0);
          double ptME = cosh(n_eta_ME_n->at(0)) / n_qOverP_ME_n->at(0);
          h_rho1_ME_n->Fill((ptME-ptID)/ptID);
          h_dR1_ME_n->Fill(sqrt( pow(n_eta_ID_n->at(0)-n_eta_ME_n->at(0),2) + pow(n_phi_ID_n->at(0)-n_phi_ME_n->at(0),2)));
        }
        if (n_isMSO_n->at(0)) {
          double ptID = cosh(n_eta_ID_n->at(0)) / n_qOverP_ID_n->at(0);
          double ptMSO = cosh(n_eta_MSO_n->at(0)) / n_qOverP_MSO_n->at(0);
          h_rho1_MSO_n->Fill((ptMSO-ptID)/ptID);
        }
        if (n_isMSOE_n->at(0))
          h_dR1_MSOE_n->Fill(sqrt( pow(n_eta_ID_n->at(0)-n_eta_MSOE_n->at(0),2) + pow(n_phi_ID_n->at(0)-n_phi_MSOE_n->at(0),2)));
      }
    }
  }
  //delete f_recoNew;
  

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
  vl->push_back(l_tr); vl->push_back(l_tr);
  // dR leading
  vl->push_back(l_tr); vl->push_back(l_tr);
  // dR subleading
  vl->push_back(l_tr); vl->push_back(l_tr);
  // rho leading
  vl->push_back(l_tr); vl->push_back(l_tr);
  // rho subleading
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
  vt->push_back(nullptr); vt->push_back(nullptr);
  // dR leading
  vt->push_back(nullptr); vt->push_back(nullptr);
  // dR subleading
  vt->push_back(nullptr); vt->push_back(nullptr);
  // rho leading
  vt->push_back(nullptr); vt->push_back(nullptr);
  // rho subleading
  vt->push_back(nullptr); vt->push_back(nullptr);

  // set histogram max values
  double max = 0, n_min = min(nEvents_o, nEvents_n);
  for (int i = 0; i < vh_o->size(); i++) {
    if (nEvents_o > 0 and nEvents_n > 0) {
      vh_o->at(i)->Scale(n_min/nEvents_o);
      vh_n->at(i)->Scale(n_min/nEvents_n);
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
    MakeRatioPlot(ratioCan, vh_o->at(i), vh_n->at(i), vl->at(i), vt->at(i), 
                  oldMapName, newMapName, file_prefix);
  }
  ratioCan->Close();

  cout << oldMapName << endl
       << "-------------------------------------------------" << endl
       << "Fraction of positive leading muons: " << (double)nLeadingPlus_o/(double)nEvents_o*100. << " +/- " << .5/sqrt((double)nEvents_o)*100. << " %" << endl
       << "Total number of events: " << nEvents_o << endl
       << "Fraction of positive leading muons (no bad MS muons): " << (double)nLeadingPlus_noBadMS_o/(double)nEvents_noBadMS_o*100. << " +/- " << .5/sqrt((double)nEvents_noBadMS_o)*100. << " %" << endl
       << "Total number of events (no bad MS muons): " << nEvents_noBadMS_o << endl << endl
       << newMapName << endl
       << "-------------------------------------------------" << endl
       << "Fraction of positive leading muons: " << (double)nLeadingPlus_n/(double)nEvents_n*100. << " +/- " << .5/sqrt((double)nEvents_n)*100. << " %" << endl
       << "Total number of events: " << nEvents_n << endl
       << "Fraction of positive leading muons (no bad MS muons): " << (double)nLeadingPlus_noBadMS_n/(double)nEvents_noBadMS_n*100. << " +/- " << .5/sqrt((double)nEvents_noBadMS_n)*100. << " %" << endl
       << "Total number of events (no bad MS muons): " << nEvents_noBadMS_n << endl;
 
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

void MakeChargePlots(string file_prefix, string fileName) {
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetPadBorderSize(2);
  gStyle->SetTitleFontSize(0.075);
  //gStyle->SetTitleX(0.1f);
  gStyle->SetTitleW(-0.1f);
  Int_t colors[] = {kAzure, kAzure-2};
  gStyle->SetPalette(2,colors);
  
  TCanvas *can3x2 = new TCanvas("can3x2","",1500,1000);

  TH2F* hh_qIDMSlowp = new TH2F("hh_qIDMSlowp","P < 10 TeV",2,-1.5,1.5,2,-1.5,1.5);
  TH2F* hh_qIDMShighp = new TH2F("hh_qIDMShighp","P > 10 TeV",2,-1.5,1.5,2,-1.5,1.5);
  TH2F* hh_qIDMElowp = new TH2F("hh_qIDMElowp","P < 10 TeV",2,-1.5,1.5,2,-1.5,1.5);
  TH2F* hh_qIDMEhighp = new TH2F("hh_qIDMEhighp","P > 10 TeV",2,-1.5,1.5,2,-1.5,1.5);
  TH2F* hh_qIDCBlowp = new TH2F("hh_qIDCBlowp","P < 10 TeV",2,-1.5,1.5,2,-1.5,1.5);
  TH2F* hh_qIDCBhighp = new TH2F("hh_qIDCBhighp","P > 10 TeV",2,-1.5,1.5,2,-1.5,1.5);

  const char *qID[2] = {"#bf{q_{ID} = -1}", "#bf{q_{ID} = +1}"};
  const char *qX[6][2] = {{"#bf{q_{MS} = -1}", "#bf{q_{MS} = +1}"},
                          {"#bf{q_{ME} = -1}", "#bf{q_{ME} = +1}"},
                          {"#bf{q_{CB} = -1}", "#bf{q_{CB} = +1}"},
                          {"#bf{q_{MS} = -1}", "#bf{q_{MS} = +1}"},
                          {"#bf{q_{ME} = -1}", "#bf{q_{ME} = +1}"},
                          {"#bf{q_{CB} = -1}", "#bf{q_{CB} = +1}"}}; 
 
  vector<TH2F*>* vhh = new vector<TH2F*>();
  vhh->push_back(hh_qIDMShighp);
  vhh->push_back(hh_qIDMEhighp);
  vhh->push_back(hh_qIDCBhighp);  
  vhh->push_back(hh_qIDMSlowp);
  vhh->push_back(hh_qIDMElowp);
  vhh->push_back(hh_qIDCBlowp);  

  for (auto *hh : *vhh) {
    hh->GetXaxis()->SetLabelOffset(99);
    hh->GetXaxis()->SetNdivisions(0);
    hh->GetYaxis()->SetLabelOffset(99);
    hh->GetYaxis()->SetNdivisions(0);
  }
  
  TFile* f_selection = new TFile(Form("ntuples_muonSelection/%s", fileName.c_str()));
  if (!f_selection) cout << "Warning: could not open file " << fileName << endl;
 
  TTreeReader reader("SelectedZDecayMuons", f_selection);
  TTreeReaderValue<vector<float>> pt_CB(reader, "Pt_CB");
  TTreeReaderValue<vector<float>> pt_ME(reader, "Pt_ME");
  TTreeReaderValue<vector<float>> pt_MS(reader, "Pt_MS");
  TTreeReaderValue<vector<float>> pt_ID(reader, "Pt_ID");
  TTreeReaderValue<vector<float>> eta_CB(reader, "Eta_CB");
  TTreeReaderValue<vector<float>> eta_ME(reader, "Eta_ME");
  TTreeReaderValue<vector<float>> eta_MS(reader, "Eta_MS");
  TTreeReaderValue<vector<float>> eta_ID(reader, "Eta_ID");
  TTreeReaderValue<vector<float>> charge_CB(reader, "Charge_CB");
  TTreeReaderValue<vector<float>> charge_ME(reader, "Charge_ME");
  TTreeReaderValue<vector<float>> charge_MS(reader, "Charge_MS");
  TTreeReaderValue<vector<float>> charge_ID(reader, "Charge_ID");
  TTreeReaderValue<vector<int>> muonType(reader, "MuonType");
  TTreeReaderValue<vector<int>> quality(reader, "Quality");
  TTreeReaderValue<vector<int>> author(reader, "Author");
  
  // loop through TTreeReader
  while (reader.Next()) {
    // loop over two muons
    for (int i = 0; i < 2; i++) {
      if (pt_MS->at(i) == -1) continue;
      // bad MS muons
      if (pt_MS->at(i)*eta_MS->at(i) > 10000) {
        hh_qIDMShighp->Fill(charge_MS->at(i), charge_ID->at(i));
        hh_qIDMEhighp->Fill(charge_ME->at(i), charge_ID->at(i));
        hh_qIDCBhighp->Fill(charge_CB->at(i), charge_ID->at(i));
      }
      // good MS muons
      else {
        hh_qIDMSlowp->Fill(charge_MS->at(i), charge_ID->at(i));
        hh_qIDMElowp->Fill(charge_ME->at(i), charge_ID->at(i));
        hh_qIDCBlowp->Fill(charge_CB->at(i), charge_ID->at(i));
      } 
    }
  }
  delete f_selection;

  float x, y;
  TLatex t, tp;
  t.SetTextSize(0.06);
  tp.SetTextSize(0.06);
  tp.SetTextAlign(22);
  float tot;
  can3x2->Divide(3,2);

  for (int j = 0; j < 6; j++) {
    can3x2->cd(j+1);
    gPad->SetRightMargin(0);
    vhh->at(j)->Draw("COL");
    tot = 0;
    tot += vhh->at(j)->GetBinContent(1,1);
    tot += vhh->at(j)->GetBinContent(1,2);
    tot += vhh->at(j)->GetBinContent(2,1);
    tot += vhh->at(j)->GetBinContent(2,2);
    for (int bx = 1; bx <= 2; bx++) 
      for (int by = 1; by <= 2; by++) {
        x = vhh->at(j)->GetXaxis()->GetBinCenter(bx);
        y = vhh->at(j)->GetYaxis()->GetBinCenter(by);
        tp.DrawLatex(x,y,Form("#bf{%.1f%%}",vhh->at(j)->GetBinContent(bx,by)/tot*100));
      }
    // draw labels along x
    y = gPad->GetUymin() - 1.15*vhh->at(j)->GetYaxis()->GetBinWidth(1);
    t.SetTextAlign(22);
    t.SetTextAngle(0);
    for (int i=0;i<2;i++) {
       x = vhh->at(j)->GetXaxis()->GetBinCenter(i+1);
       t.DrawLatex(x,y,qX[j][i]);
    }
    // draw labels along y
    x = gPad->GetUxmin() - 1.15*vhh->at(j)->GetXaxis()->GetBinWidth(1);
    t.SetTextAlign(22);
    t.SetTextAngle(90);
    for (int i=0;i<2;i++) {
       y = vhh->at(j)->GetYaxis()->GetBinCenter(i+1);
       t.DrawLatex(x,y,qID[i]);
    }
  } 
  can3x2->Print(Form("plots/%s_%s_charge.png", file_prefix.c_str(), fileName.substr(18,4).c_str())); 
  
  can3x2->Close();
  delete can3x2;
  for (int i = 0; i < 6; i++) delete vhh->at(i);
  delete vhh;
}

/**************************************************************************************************/

void GeneratePlots() {
  // declare and print file prefix
  //string file_prefix = "TEST_";
  string file_prefix = "group.perf-muons.AODvRunI";
  string oldFileName = "test.root";
  string newFileName = "bbullard.00340072.2016.root";

  cout << "Old map reconstruction: " << oldFileName << endl;
  cout << "New map reconstruction: " << newFileName << endl;
  cout << "Preparing output with prefix " << file_prefix.c_str() << endl; 
  
  MakeRatioPlots(file_prefix, oldFileName, newFileName);
  //MakeProfilePlots(file_prefix, oldFileName, newFileName);
  //MakeChargePlots(file_prefix, oldFileName);
  //MakeChargePlots(file_prefix, newFileName);
}
