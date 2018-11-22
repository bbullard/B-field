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

using namespace std;

namespace PlottingHelpers
{
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
    ratio->SetMinimum(0.94);
    ratio->SetMaximum(1.06);
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
}
