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
 
  // leading pt histograms for 3 track types and 2 files
  TH1F* h_pt1_CB_o = new TH1F("h_pt1_CB_o","",nBins,0,120);
  h_pt1_CB_o->SetTitle(";Leading p_{T} (GeV);Events");
  TH1F* h_pt1_ME_o = (TH1F*)h_pt1_CB_o->Clone("h_pt1_ME_o");
  TH1F* h_pt1_MS_o = (TH1F*)h_pt1_CB_o->Clone("h_pt1_MS_o");
  TH1F* h_pt1_ID_o = (TH1F*)h_pt1_CB_o->Clone("h_pt1_ID_o");

  TH1F* h_pt1_CB_n = (TH1F*)h_pt1_CB_o->Clone("h_pt1_CB_n");
  TH1F* h_pt1_ME_n = (TH1F*)h_pt1_CB_o->Clone("h_pt1_ME_n");
  TH1F* h_pt1_MS_n = (TH1F*)h_pt1_CB_o->Clone("h_pt1_MS_n");
  TH1F* h_pt1_ID_n = (TH1F*)h_pt1_CB_o->Clone("h_pt1_ID_n");
 
  // leading pt histograms for 3 track types and 2 files
  TH1F* h_pt2_CB_o = new TH1F("h_pt2_CB_o","",nBins,0,120);
  h_pt2_CB_o->SetTitle(";Subleading p_{T} (GeV);Events");
  TH1F* h_pt2_ME_o = (TH1F*)h_pt2_CB_o->Clone("h_pt2_ME_o");
  TH1F* h_pt2_MS_o = (TH1F*)h_pt2_CB_o->Clone("h_pt2_MS_o");
  TH1F* h_pt2_ID_o = (TH1F*)h_pt2_CB_o->Clone("h_pt2_ID_o");

  TH1F* h_pt2_CB_n = (TH1F*)h_pt2_CB_o->Clone("h_pt2_CB_n");
  TH1F* h_pt2_ME_n = (TH1F*)h_pt2_CB_o->Clone("h_pt2_ME_n");
  TH1F* h_pt2_MS_n = (TH1F*)h_pt2_CB_o->Clone("h_pt2_MS_n");
  TH1F* h_pt2_ID_n = (TH1F*)h_pt2_CB_o->Clone("h_pt2_ID_n");
  
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
  
  // eta histograms for 3 track types and 2 files leading
  TH1F* h_eta1_CB_o = new TH1F("h_eta1_CB_o","",nBins,-2.7,2.7);
  h_eta1_CB_o->SetTitle(";Leading Muon #eta;Events");
  TH1F* h_eta1_ME_o = (TH1F*)h_eta1_CB_o->Clone("h_eta1_ME_o");
  TH1F* h_eta1_MS_o = (TH1F*)h_eta1_CB_o->Clone("h_eta1_MS_o");
  TH1F* h_eta1_ID_o = (TH1F*)h_eta1_CB_o->Clone("h_eta1_ID_o");

  TH1F* h_eta1_CB_n = (TH1F*)h_eta1_CB_o->Clone("h_eta1_CB_n");
  TH1F* h_eta1_ME_n = (TH1F*)h_eta1_CB_o->Clone("h_eta1_ME_n");
  TH1F* h_eta1_MS_n = (TH1F*)h_eta1_CB_o->Clone("h_eta1_MS_n");
  TH1F* h_eta1_ID_n = (TH1F*)h_eta1_CB_o->Clone("h_eta1_ID_n");
 
  // eta histograms for 3 track types and 2 files subleading
  TH1F* h_eta2_CB_o = new TH1F("h_eta2_CB_o","",nBins,-2.7,2.7);
  h_eta2_CB_o->SetTitle(";Subleading Muon #eta;Events");
  TH1F* h_eta2_ME_o = (TH1F*)h_eta2_CB_o->Clone("h_eta2_ME_o");
  TH1F* h_eta2_MS_o = (TH1F*)h_eta2_CB_o->Clone("h_eta2_MS_o");
  TH1F* h_eta2_ID_o = (TH1F*)h_eta2_CB_o->Clone("h_eta2_ID_o");

  TH1F* h_eta2_CB_n = (TH1F*)h_eta2_CB_o->Clone("h_eta2_CB_n");
  TH1F* h_eta2_ME_n = (TH1F*)h_eta2_CB_o->Clone("h_eta2_ME_n");
  TH1F* h_eta2_MS_n = (TH1F*)h_eta2_CB_o->Clone("h_eta2_MS_n");
  TH1F* h_eta2_ID_n = (TH1F*)h_eta2_CB_o->Clone("h_eta2_ID_n");
  
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
 
  // author
  TH1F* h_author1_o = new TH1F("h_author1_o","",6,0,6);
  h_author1_o->SetTitle(";Leading Muon Author;Events");
  TH1F* h_author2_o = new TH1F("h_author2_o","",6,0,6);
  h_author2_o->SetTitle(";Subleading Muon Author;Events");

  TH1F* h_author1_n = (TH1F*)h_author1_o->Clone("h_author1_o");
  TH1F* h_author2_n = (TH1F*)h_author2_o->Clone("h_author2_o");

  // quality
  TH1F* h_quality1_o = new TH1F("h_quality1_o","",2,0,2);
  h_quality1_o->SetTitle(";Leading Muon Quality;Events");
  TH1F* h_quality2_o = new TH1F("h_quality2_o","",2,0,2);
  h_quality2_o->SetTitle(";Subleading Muon Quality;Events");
  
  TH1F* h_quality1_n = (TH1F*)h_quality1_o->Clone("h_quality1_o");
  TH1F* h_quality2_n = (TH1F*)h_quality2_o->Clone("h_quality2_o");
  
  // type
  TH1F* h_type1_o = new TH1F("h_type1_o","",4,0,4);
  h_type1_o->SetTitle(";Leading Muon Type;Events");
  TH1F* h_type2_o = new TH1F("h_type2_o","",4,0,4);
  h_type2_o->SetTitle(";Subleading Muon Type;Events");
  
  TH1F* h_type1_n = (TH1F*)h_type1_o->Clone("h_type1_o");
  TH1F* h_type2_n = (TH1F*)h_type2_o->Clone("h_type2_o");
  
  // rho 
  TH1F* h_rho_ME_o = new TH1F("h_rho_ME_o","",nBins,-.4,.4);
  h_rho_ME_o->SetTitle(";#rho_{ME};Events");
  TH1F* h_rho_MS_o = (TH1F*)h_rho_ME_o->Clone("h_rho_MS_o");
  h_rho_MS_o->SetTitle(";#rho_{MS};Events");

  TH1F* h_rho_ME_n = (TH1F*)h_rho_ME_o->Clone("h_rho_ME_n");
  TH1F* h_rho_MS_n = (TH1F*)h_rho_ME_o->Clone("h_rho_MS_n");

  // rho leading 
  TH1F* h_rho1_ME_o = new TH1F("h_rho1_ME_o","",nBins,-.4,.4);
  h_rho1_ME_o->SetTitle(";Leading Muon #rho_{ME};Events");
  TH1F* h_rho1_MS_o = (TH1F*)h_rho1_ME_o->Clone("h_rho1_MS_o");
  h_rho1_MS_o->SetTitle(";Leading Muon #rho_{MS};Events");

  TH1F* h_rho1_ME_n = (TH1F*)h_rho1_ME_o->Clone("h_rho1_ME_n");
  TH1F* h_rho1_MS_n = (TH1F*)h_rho1_MS_o->Clone("h_rho1_MS_n");
  
  // rho subleading
  TH1F* h_rho2_ME_o = new TH1F("h_rho2_ME_o","",nBins,-.4,.4);
  h_rho2_ME_o->SetTitle(";Subleading Muon #rho_{ME};Events");
  TH1F* h_rho2_MS_o = (TH1F*)h_rho2_ME_o->Clone("h_rho2_MS_o");
  h_rho2_MS_o->SetTitle(";Subleading Muon #rho_{MS};Events");

  TH1F* h_rho2_ME_n = (TH1F*)h_rho2_ME_o->Clone("h_rho2_ME_n");
  TH1F* h_rho2_MS_n = (TH1F*)h_rho2_MS_o->Clone("h_rho2_MS_n");
  
  // dR (MS or ME) and ID
  TH1F* h_dR_ME_o = new TH1F("h_dR_ME_o","",nBins,0,.01);
  h_dR_ME_o->SetTitle(";#Delta R_{ME,ID};Events");
  TH1F* h_dR_MS_o = new TH1F("h_dR_MS_o","",nBins,0,0.05);
  h_dR_MS_o->SetTitle(";#Delta R_{MS,ID};Events");

  TH1F* h_dR_ME_n = (TH1F*)h_dR_ME_o->Clone("h_dR_ME_n");
  TH1F* h_dR_MS_n = (TH1F*)h_dR_MS_o->Clone("h_dR_MS_n");
  
  // dR (MS or ME) and ID leading
  TH1F* h_dR1_ME_o = new TH1F("h_dR1_ME_o","",nBins,0,.01);
  h_dR1_ME_o->SetTitle(";Leading Muon #Delta R_{ME,ID};Events");
  TH1F* h_dR1_MS_o = new TH1F("h_dR1_MS_o","",nBins,0,0.05);
  h_dR1_MS_o->SetTitle(";Leading Muon #Delta R_{MS,ID};Events");

  TH1F* h_dR1_ME_n = (TH1F*)h_dR1_ME_o->Clone("h_dR1_ME_n");
  TH1F* h_dR1_MS_n = (TH1F*)h_dR1_MS_o->Clone("h_dR1_MS_n");

  // dR (MS or ME) and ID subleading
  TH1F* h_dR2_ME_o = new TH1F("h_dR2_ME_o","",nBins,0,.01);
  h_dR2_ME_o->SetTitle(";Subleading Muon #Delta R_{ME,ID};Events");
  TH1F* h_dR2_MS_o = new TH1F("h_dR2_MS_o","",nBins,0,.05);
  h_dR2_MS_o->SetTitle(";Subleading Muon #Delta R_{MS,ID};Events");

  TH1F* h_dR2_ME_n = (TH1F*)h_dR2_ME_o->Clone("h_dR2_ME_n");
  TH1F* h_dR2_MS_n = (TH1F*)h_dR2_MS_o->Clone("h_dR2_MS_n");
  
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
  vh_o->push_back(h_pt1_CB_o);
  vh_o->push_back(h_pt1_ME_o);
  vh_o->push_back(h_pt1_MS_o);
  vh_o->push_back(h_pt1_ID_o);
  vh_o->push_back(h_pt2_CB_o);
  vh_o->push_back(h_pt2_ME_o);
  vh_o->push_back(h_pt2_MS_o);
  vh_o->push_back(h_pt2_ID_o);
  vh_o->push_back(h_eta_CB_o);
  vh_o->push_back(h_eta_ME_o);
  vh_o->push_back(h_eta_MS_o);
  vh_o->push_back(h_eta_ID_o);
  vh_o->push_back(h_eta1_CB_o);
  vh_o->push_back(h_eta1_ME_o);
  vh_o->push_back(h_eta1_MS_o);
  vh_o->push_back(h_eta1_ID_o);
  vh_o->push_back(h_eta2_CB_o);
  vh_o->push_back(h_eta2_ME_o);
  vh_o->push_back(h_eta2_MS_o);
  vh_o->push_back(h_eta2_ID_o);
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
  vh_o->push_back(h_author1_o);
  vh_o->push_back(h_author2_o);
  vh_o->push_back(h_type1_o);
  vh_o->push_back(h_type2_o);
  vh_o->push_back(h_quality1_o);
  vh_o->push_back(h_quality2_o);
  vh_o->push_back(h_dR_ME_o);
  vh_o->push_back(h_dR_MS_o);
  vh_o->push_back(h_dR1_ME_o);
  vh_o->push_back(h_dR1_MS_o);
  vh_o->push_back(h_dR2_ME_o);
  vh_o->push_back(h_dR2_MS_o);
  vh_o->push_back(h_rho_ME_o);
  vh_o->push_back(h_rho_MS_o);
  vh_o->push_back(h_rho1_ME_o);
  vh_o->push_back(h_rho1_MS_o);
  vh_o->push_back(h_rho2_ME_o);
  vh_o->push_back(h_rho2_MS_o);

  vh_n->push_back(h_m_CB_n);
  vh_n->push_back(h_m_ME_n);
  vh_n->push_back(h_m_MS_n);
  vh_n->push_back(h_m_ID_n);
  vh_n->push_back(h_pt_CB_n);
  vh_n->push_back(h_pt_ME_n);
  vh_n->push_back(h_pt_MS_n);
  vh_n->push_back(h_pt_ID_n);
  vh_n->push_back(h_pt1_CB_n);
  vh_n->push_back(h_pt1_ME_n);
  vh_n->push_back(h_pt1_MS_n);
  vh_n->push_back(h_pt1_ID_n);
  vh_n->push_back(h_pt2_CB_n);
  vh_n->push_back(h_pt2_ME_n);
  vh_n->push_back(h_pt2_MS_n);
  vh_n->push_back(h_pt2_ID_n);
  vh_n->push_back(h_eta_CB_n);
  vh_n->push_back(h_eta_ME_n);
  vh_n->push_back(h_eta_MS_n);
  vh_n->push_back(h_eta_ID_n);
  vh_n->push_back(h_eta1_CB_n);
  vh_n->push_back(h_eta1_ME_n);
  vh_n->push_back(h_eta1_MS_n);
  vh_n->push_back(h_eta1_ID_n);
  vh_n->push_back(h_eta2_CB_n);
  vh_n->push_back(h_eta2_ME_n);
  vh_n->push_back(h_eta2_MS_n);
  vh_n->push_back(h_eta2_ID_n);
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
  vh_n->push_back(h_author1_n);
  vh_n->push_back(h_author2_n);
  vh_n->push_back(h_type1_n);
  vh_n->push_back(h_type2_n);
  vh_n->push_back(h_quality1_n);
  vh_n->push_back(h_quality2_n);
  vh_n->push_back(h_dR_ME_n);
  vh_n->push_back(h_dR_MS_n);
  vh_n->push_back(h_dR1_ME_n);
  vh_n->push_back(h_dR1_MS_n);
  vh_n->push_back(h_dR2_ME_n);
  vh_n->push_back(h_dR2_MS_n);
  vh_n->push_back(h_rho_ME_n);
  vh_n->push_back(h_rho_MS_n);
  vh_n->push_back(h_rho1_ME_n);
  vh_n->push_back(h_rho1_MS_n);
  vh_n->push_back(h_rho2_ME_n);
  vh_n->push_back(h_rho2_MS_n);
  
  // check that the vectors have the same size
  if ( vh_o->size() != vh_n->size()) 
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
  string oldMapName = "Run I";

  long int nEvents_o = 0;
  long int nLeadingPlus_o = 0;
  long int nEvents_noBadMS_o = 0;
  long int nLeadingPlus_noBadMS_o = 0;
  
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
    //if (pt_CB_o->at(0) < 0 or pt_CB_o->at(0) > 25) continue;
    //if (pt_CB_o->at(0) < 25) continue;
    nEvents_o++;
    if (charge_CB_o->at(0) == 1) 
      nLeadingPlus_o++;
  
    if (pt_MS_o->at(0) > 0 and pt_MS_o->at(0) < 10000){
      nEvents_noBadMS_o++;
      if (charge_CB_o->at(0) == 1) 
        nLeadingPlus_noBadMS_o++;
    }
 
    h_author1_o->Fill(author_o->at(0)); 
    h_author2_o->Fill(author_o->at(1)); 
    h_quality1_o->Fill(quality_o->at(0)); 
    h_quality2_o->Fill(quality_o->at(1)); 
    h_type1_o->Fill(muonType_o->at(0)); 
    h_type2_o->Fill(muonType_o->at(1)); 
    
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
 
    // fill leading/subleading variable histograms
    if (pt_CB_o->at(0) != -1) { 
      h_pt1_CB_o->Fill(pt_CB_o->at(0));
      h_eta1_CB_o->Fill(eta_CB_o->at(0)); 
    }
    if (pt_CB_o->at(1) != -1) {
      h_pt2_CB_o->Fill(pt_CB_o->at(1)); 
      h_eta2_CB_o->Fill(eta_CB_o->at(1)); 
    }
    if (pt_ME_o->at(0) != -1) {
      h_pt1_ME_o->Fill(pt_ME_o->at(0)); 
      h_eta1_ME_o->Fill(eta_ME_o->at(0)); 
    }
    if (pt_ME_o->at(1) != -1) {
      h_pt2_ME_o->Fill(pt_ME_o->at(1)); 
      h_eta2_ME_o->Fill(eta_ME_o->at(1)); 
    }
    if (pt_MS_o->at(0) != -1) {
      h_pt1_MS_o->Fill(pt_MS_o->at(0)); 
      h_eta1_MS_o->Fill(eta_MS_o->at(0)); 
    }
    if (pt_MS_o->at(1) != -1) {
      h_pt2_MS_o->Fill(pt_MS_o->at(1)); 
      h_eta2_MS_o->Fill(eta_MS_o->at(1)); 
    }
    if (pt_ID_o->at(0) != -1) {
      h_pt1_ID_o->Fill(pt_ID_o->at(0)); 
      h_eta1_ID_o->Fill(eta_ID_o->at(0));
      
      if (pt_ME_o->at(0) != -1) {
        h_rho1_ME_o->Fill((pt_ME_o->at(0)-pt_ID_o->at(0))/pt_ID_o->at(0));
        h_dR1_ME_o->Fill(sqrt( pow(eta_ID_o->at(0)-eta_ME_o->at(0),2) + pow(phi_ID_o->at(0)-phi_ME_o->at(0),2)));
      } 
      if (pt_MS_o->at(0) != -1) {
        h_rho1_MS_o->Fill((pt_MS_o->at(0)-pt_ID_o->at(0))/pt_ID_o->at(0));
        h_dR1_MS_o->Fill(sqrt( pow(eta_ID_o->at(0)-eta_MS_o->at(0),2) + pow(phi_ID_o->at(0)-phi_MS_o->at(0),2)));
      } 
    }
    if (pt_ID_o->at(1) != -1) {
      h_pt2_ID_o->Fill(pt_ID_o->at(1)); 
      h_eta2_ID_o->Fill(eta_ID_o->at(1)); 
      
      if (pt_ME_o->at(1) != -1) {
        h_rho2_ME_o->Fill((pt_ME_o->at(1)-pt_ID_o->at(1))/pt_ID_o->at(1));
        h_dR2_ME_o->Fill(sqrt( pow(eta_ID_o->at(1)-eta_ME_o->at(1),2) + pow(phi_ID_o->at(1)-phi_ME_o->at(1),2)));
      } 
      if (pt_MS_o->at(1) != -1) {
        h_rho2_MS_o->Fill((pt_MS_o->at(1)-pt_ID_o->at(1))/pt_ID_o->at(1));
        h_dR2_MS_o->Fill(sqrt( pow(eta_ID_o->at(1)-eta_MS_o->at(1),2) + pow(phi_ID_o->at(1)-phi_MS_o->at(1),2)));
      } 
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
        
        // fill rho, dR ME
        if (pt_ME_o->at(i) != -1) {
          h_rho_ME_o->Fill((pt_ME_o->at(i)-pt_ID_o->at(i))/pt_ID_o->at(i));
          h_dR_ME_o->Fill(sqrt( pow(eta_ID_o->at(i)-eta_ME_o->at(i),2) + pow(phi_ID_o->at(i)-phi_ME_o->at(i),2)));
        } 
        // fill rho, dR MS
        if (pt_MS_o->at(i) != -1) {
          h_rho_MS_o->Fill((pt_MS_o->at(i)-pt_ID_o->at(i))/pt_ID_o->at(i)); 
          h_dR_MS_o->Fill(sqrt( pow(eta_ID_o->at(i)-eta_MS_o->at(i),2) + pow(phi_ID_o->at(i)-phi_MS_o->at(i),2)));
        }
      }
    }
  }
  delete f_recoOld;

  // load new file and set up reader
  TFile* f_recoNew = new TFile(Form("ntuples_muonSelection/%s", newFileName.c_str()));
  if (!f_recoNew) cout << "Warning: could not open file " << newFileName << endl;
  string newMapName = "2016";
 
  long int nEvents_n = 0;
  long int nLeadingPlus_n = 0;
  long int nEvents_noBadMS_n = 0;
  long int nLeadingPlus_noBadMS_n = 0;
  
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
    //if (pt_CB_n->at(0) < 0 or pt_CB_n->at(0) > 25) continue;
    //if (pt_CB_n->at(0) < 25) continue;
    nEvents_n++;
    if (charge_CB_n->at(0) == 1) 
      nLeadingPlus_n++;
    
    if (pt_MS_n->at(0) > 0 and pt_MS_n->at(0) < 10000){
      nEvents_noBadMS_n++;
      if (charge_CB_n->at(0) == 1) 
        nLeadingPlus_noBadMS_n++;
    }

    h_author1_n->Fill(author_n->at(0)); 
    h_author2_n->Fill(author_n->at(1)); 
    h_quality1_n->Fill(quality_n->at(0)); 
    h_quality2_n->Fill(quality_n->at(1)); 
    h_type1_n->Fill(muonType_n->at(0)); 
    h_type2_n->Fill(muonType_n->at(1)); 

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
  
    // fill leading/subleading variable histograms
    if (pt_CB_n->at(0) != -1) { 
      h_pt1_CB_n->Fill(pt_CB_n->at(0));
      h_eta1_CB_n->Fill(eta_CB_n->at(0)); 
    }
    if (pt_CB_n->at(1) != -1) {
      h_pt2_CB_n->Fill(pt_CB_n->at(1)); 
      h_eta2_CB_n->Fill(eta_CB_n->at(1)); 
    }
    if (pt_ME_n->at(0) != -1) {
      h_pt1_ME_n->Fill(pt_ME_n->at(0)); 
      h_eta1_ME_n->Fill(eta_ME_n->at(0)); 
    }
    if (pt_ME_n->at(1) != -1) {
      h_pt2_ME_n->Fill(pt_ME_n->at(1)); 
      h_eta2_ME_n->Fill(eta_ME_n->at(1)); 
    }
    if (pt_MS_n->at(0) != -1) {
      h_pt1_MS_n->Fill(pt_MS_n->at(0)); 
      h_eta1_MS_n->Fill(eta_MS_n->at(0)); 
    }
    if (pt_MS_n->at(1) != -1) {
      h_pt2_MS_n->Fill(pt_MS_n->at(1)); 
      h_eta2_MS_n->Fill(eta_MS_n->at(1)); 
    }
    if (pt_ID_n->at(0) != -1) {
      h_pt1_ID_n->Fill(pt_ID_n->at(0)); 
      h_eta1_ID_n->Fill(eta_ID_n->at(0));
      
      if (pt_ME_n->at(0) != -1) {
        h_rho1_ME_n->Fill((pt_ME_n->at(0)-pt_ID_n->at(0))/pt_ID_n->at(0));
        h_dR1_ME_n->Fill(sqrt( pow(eta_ID_n->at(0)-eta_ME_n->at(0),2) + pow(phi_ID_n->at(0)-phi_ME_n->at(0),2)));
      } 
      if (pt_MS_n->at(0) != -1) {
        h_rho1_MS_n->Fill((pt_MS_n->at(0)-pt_ID_n->at(0))/pt_ID_n->at(0));
        h_dR1_MS_n->Fill(sqrt( pow(eta_ID_n->at(0)-eta_MS_n->at(0),2) + pow(phi_ID_n->at(0)-phi_MS_n->at(0),2)));
      } 
    }
    if (pt_ID_n->at(1) != -1) {
      h_pt2_ID_n->Fill(pt_ID_n->at(1)); 
      h_eta2_ID_n->Fill(eta_ID_n->at(1)); 
      
      if (pt_ME_n->at(1) != -1) {
        h_rho2_ME_n->Fill((pt_ME_n->at(1)-pt_ID_n->at(1))/pt_ID_n->at(1));
        h_dR2_ME_n->Fill(sqrt( pow(eta_ID_n->at(1)-eta_ME_n->at(1),2) + pow(phi_ID_n->at(1)-phi_ME_n->at(1),2)));
      } 
      if (pt_MS_n->at(1) != -1) {
        h_rho2_MS_n->Fill((pt_MS_n->at(1)-pt_ID_n->at(1))/pt_ID_n->at(1));
        h_dR2_MS_n->Fill(sqrt( pow(eta_ID_n->at(1)-eta_MS_n->at(1),2) + pow(phi_ID_n->at(1)-phi_MS_n->at(1),2)));
      } 
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
        
        // fill rho, dR ME
        if (pt_ME_n->at(i) != -1) {
          h_rho_ME_n->Fill((pt_ME_n->at(i)-pt_ID_n->at(i))/pt_ID_n->at(i));
          h_dR_ME_n->Fill(sqrt( pow(eta_ID_n->at(i)-eta_ME_n->at(i),2) + pow(phi_ID_n->at(i)-phi_ME_n->at(i),2)));
        } 
        // fill rho, dR MS
        if (pt_MS_n->at(i) != -1) {
          h_rho_MS_n->Fill((pt_MS_n->at(i)-pt_ID_n->at(i))/pt_ID_n->at(i)); 
          h_dR_MS_n->Fill(sqrt( pow(eta_ID_n->at(i)-eta_MS_n->at(i),2) + pow(phi_ID_n->at(i)-phi_MS_n->at(i),2)));
        }
      }
    }
  }
  delete f_recoNew;

  // plotting for invariant mass
  // gaussian fits
  h_m_CB_o->Fit("gaus","","", 85, 95);
  h_m_ME_o->Fit("gaus","","", 85, 95);
  h_m_MS_o->Fit("gaus","","", 79, 89);
  h_m_ID_o->Fit("gaus","","", 85, 95);
  h_m_CB_n->Fit("gaus","","", 85, 95);
  h_m_ME_n->Fit("gaus","","", 85, 95);
  h_m_MS_n->Fit("gaus","","", 79, 89);
  h_m_ID_n->Fit("gaus","","", 85, 95);

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
  t_tl_CB->AddText("#it{ATLAS} #bf{Internal}");
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
  t_tc_CB->AddText("#it{ATLAS} #bf{Internal}");
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
  t_tr_CB->AddText("#it{ATLAS} #bf{Internal}");
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
  // pt leading
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr);  
  // pt subleading
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr);  
  // eta
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr);  
  // eta leading
  vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr); vl->push_back(l_tr);  
  // eta subleading
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
  // author
  vl->push_back(l_tr); vl->push_back(l_tr);
  // type
  vl->push_back(l_tr); vl->push_back(l_tr);
  // quality
  vl->push_back(l_tr); vl->push_back(l_tr);
  // dR
  vl->push_back(l_tr); vl->push_back(l_tr);
  // dR leading
  vl->push_back(l_tr); vl->push_back(l_tr);
  // dR subleading
  vl->push_back(l_tr); vl->push_back(l_tr);
  // rho
  vl->push_back(l_tr); vl->push_back(l_tr);
  // rho leading
  vl->push_back(l_tr); vl->push_back(l_tr);
  // rho subleading
  vl->push_back(l_tr); vl->push_back(l_tr);

  // vector of tpavetext labels
  std::vector<TPaveText*> *vt = new std::vector<TPaveText*>();
  // m
  vt->push_back(t_tl_CB); vt->push_back(t_tl_ME); vt->push_back(t_tl_MS); vt->push_back(t_tl_ID);
  // pt
  vt->push_back(t_tr_CB); vt->push_back(t_tr_ME); vt->push_back(t_tr_MS); vt->push_back(t_tr_ID);  
  // pt leading
  vt->push_back(t_tr_CB); vt->push_back(t_tr_ME); vt->push_back(t_tr_MS); vt->push_back(t_tr_ID);  
  // pt subleading
  vt->push_back(t_tr_CB); vt->push_back(t_tr_ME); vt->push_back(t_tr_MS); vt->push_back(t_tr_ID);  
  // eta
  vt->push_back(t_tc_CB); vt->push_back(t_tc_ME); vt->push_back(t_tc_MS); vt->push_back(t_tc_ID);  
  // eta leading
  vt->push_back(t_tc_CB); vt->push_back(t_tc_ME); vt->push_back(t_tc_MS); vt->push_back(t_tc_ID);  
  // eta subleading
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
  // author
  vt->push_back(t_tr_CB); vt->push_back(t_tr_CB);
  // type
  vt->push_back(t_tr_CB); vt->push_back(t_tr_CB);
  // quality
  vt->push_back(t_tr_CB); vt->push_back(t_tr_CB);
  // dR
  vt->push_back(nullptr); vt->push_back(nullptr);
  // dR leading
  vt->push_back(nullptr); vt->push_back(nullptr);
  // dR subleading
  vt->push_back(nullptr); vt->push_back(nullptr);
  // rho
  vt->push_back(nullptr); vt->push_back(nullptr);
  // rho leading
  vt->push_back(nullptr); vt->push_back(nullptr);
  // rho subleading
  vt->push_back(nullptr); vt->push_back(nullptr);

  // currently plotting 10 variables 
  const int Nvar = 12*4 + 2*3 + 2*2*3;
  double maxFactor[Nvar] = {};
  for (int i = 0; i < 4; i++) {
    maxFactor[0*4+i] = 1.05; // m
    maxFactor[1*4+i] = 1.05; // pt
    maxFactor[2*4+i] = 1.05; // pt leading
    maxFactor[3*4+i] = 1.05; // pt subleading
    maxFactor[4*4+i] = 1.3; // eta
    maxFactor[5*4+i] = 1.3; // eta leading
    maxFactor[6*4+i] = 1.3; // eta subleading
    maxFactor[7*4+i] = 1.3; // phi
    maxFactor[8*4+i] = 1.3; // z0
    maxFactor[9*4+i] = 1.05; // d0sig
    maxFactor[10*4+i] = 1.05; // e
    maxFactor[11*4+i] = 1.05; // pzmumu
  }
  maxFactor[48] = 1.05; // author
  maxFactor[49] = 1.05;
  maxFactor[50] = 1.05; // quality
  maxFactor[51] = 1.05;
  maxFactor[52] = 1.05; // type
  maxFactor[53] = 1.05;
  maxFactor[54] = 1.05; // dR
  maxFactor[55] = 1.05;
  maxFactor[56] = 1.05; // dR leading
  maxFactor[57] = 1.05;
  maxFactor[58] = 1.05; // dR subleading
  maxFactor[59] = 1.05;
  maxFactor[60] = 1.05; // rho
  maxFactor[61] = 1.05;
  maxFactor[62] = 1.05; // rho leading
  maxFactor[63] = 1.05;
  maxFactor[64] = 1.05; // rho subleading
  maxFactor[65] = 1.05;

  bool skip[Nvar] = {};
  
  // set histogram max values
  double max = 0, n_min = min(nEvents_o, nEvents_n);
  for (int i = 0; i < vh_o->size(); i++) {
    if (nEvents_o > 0 and nEvents_n > 0) {
      vh_o->at(i)->Scale(n_min/nEvents_o);
      vh_n->at(i)->Scale(n_min/nEvents_n);
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

void MakeCutflows(string file_prefix, string oldFileName, string newFileName) {
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetPadBorderSize(2);
  gStyle->SetTitleFontSize(0.075);
  
  TCanvas *can = new TCanvas("can","",1000,1000);

  TFile* f_old = new TFile(Form("ntuples_muonSelection/%s", oldFileName.c_str()));
  if (!f_old) cout << "Warning: could not open file " << oldFileName << endl;
  TH1F* h_cutflowOld = (TH1F*)f_old->Get("h_cutflow"); 

  TFile* f_new = new TFile(Form("ntuples_muonSelection/%s", newFileName.c_str()));
  if (!f_new) cout << "Warning: could not open file " << newFileName << endl;
  TH1F* h_cutflowNew = (TH1F*)f_new->Get("h_cutflow"); 
  
  h_cutflowOld->Scale(1/h_cutflowOld->GetBinContent(1));
  h_cutflowOld->SetLineWidth(2);
  h_cutflowOld->SetLineColor(kBlack);
  h_cutflowOld->SetFillColor(18);
  h_cutflowOld->SetFillStyle(1001);  

  h_cutflowNew->Scale(1/h_cutflowNew->GetBinContent(1));
  h_cutflowNew->SetLineWidth(2);
  h_cutflowNew->SetLineColor(kAzure-2);
  h_cutflowNew->SetFillColor(38);
  h_cutflowNew->SetFillStyle(3006);  
  
  h_cutflowOld->SetMaximum(1.2);
  h_cutflowOld->SetMinimum(0);
  h_cutflowOld->SetTitle(";Selection Stage;Fraction of Total");

  string oldMapName = "Run I";
  string newMapName = "2016";
  
  // top right legend
  TLegend* l_tr = new TLegend(.66,.76,.89,.88);
  l_tr->AddEntry(h_cutflowOld, Form("%s Map",oldMapName.c_str()), "lf");
  l_tr->AddEntry(h_cutflowNew, Form("%s Map",newMapName.c_str()), "lf");

  // top center labels
  TPaveText* t_tc = new TPaveText(.39,.82,.67,.86,"NDC");
  t_tc->AddText("#it{ATLAS} #bf{Internal}");
  t_tc->SetFillColor(0);
  t_tc->SetLineColor(0);
  t_tc->SetBorderSize(0);
  t_tc->SetTextAlign(31);
 
  can->Divide(3,2);
  h_cutflowOld->Draw("hist");
  h_cutflowNew->Draw("hist same");
  l_tr->Draw();
  //t_tc->Draw();
  can->Print(Form("plots/%s_cutflows.png", file_prefix.c_str()));
  can->Close();
  delete can;
  
  delete h_cutflowOld;
  delete h_cutflowNew;
  delete l_tr;
  delete t_tc;
  delete f_old;
  delete f_new;
}

/**************************************************************************************************/


void GeneratePlots() {
  // declare and print file prefix
  //string file_prefix = "TEST_";
  string file_prefix = "bbullard.00340072";
  string oldFileName = "bbullard.00340072.RunI.root";
  string newFileName = "bbullard.00340072.2016.root";

  cout << "Old map reconstruction: " << oldFileName << endl;
  cout << "New map reconstruction: " << newFileName << endl;
  cout << "Preparing output with prefix " << file_prefix.c_str() << endl; 
  
  MakeRatioPlots(file_prefix, oldFileName, newFileName);
  //MakeProfilePlots(file_prefix, oldFileName, newFileName);
  //MakeChargePlots(file_prefix, oldFileName);
  //MakeChargePlots(file_prefix, newFileName);
  //MakeCutflows(file_prefix, oldFileName, newFileName); 
}
