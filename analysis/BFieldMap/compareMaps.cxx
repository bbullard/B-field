// $Id$

// C++ standard libraries
#include <iostream>
using namespace std;
// ROOT libraries
#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TString.h"
#include "TStyle.h"
// BField code
#include "BFieldMap.h"

const double PI(3.14159265);

BFieldMap* bmap;

void
xyscan( TVirtualPad* pad, double maxr, double z = 0, int n = 1000 )
{
    TH2D* h_B1 = new TH2D("h_B1",Form("|B_{1}| @ z = %.2f m;x (mm);y (mm)", z/1000),n,-maxr,maxr,n,-maxr,maxr);
    TH2D* h_B2 = new TH2D("h_B2","|B_{2}|;x (mm);y (mm)",n,-maxr,maxr,n,-maxr,maxr);
    TH2D* h_dB = new TH2D("h_dB","|B_{1} - B_{2}|/|B_{1}| (%);x (mm);y (mm)",n,-maxr,maxr,n,-maxr,maxr);
    h_B1->SetStats(0); h_B2->SetStats(0); h_dB->SetStats(0);
    for ( int i = 0; i < n; i++ ) for ( int j = 0; j < n; j++ ) {
        double x = (2.*(i+0.5)/n-1.)*maxr;
        double y = (2.*(j+0.5)/n-1.)*maxr;
        double pos[3];
        pos[0] = x; pos[1] = y; pos[2] = z;
        TVector3 B[2];
        for ( int k = 0; k < 2; k++ ) {
            double BkT[3];
            bmap[k].getB( pos, BkT ); // in kT
            B[k] = BkT;
            B[k] *= 1000.; // in T
        }
        h_B1->Fill(x,y,B[0].Mag());
        h_B2->Fill(x,y,B[1].Mag());
        TVector3 deltaB = B[0] - B[1];
        h_dB->Fill(x,y,deltaB.Mag()/B[0].Mag()*100);
    }
    pad->Divide(1,3);
    pad->cd(1);
    h_B1->DrawCopy("colz");
    pad->cd(2);
    h_B2->DrawCopy("colz");
    pad->cd(3);
    h_dB->SetMaximum(10);
    h_dB->SetMinimum(0);
    h_dB->DrawCopy("colz");
    pad->Update();
    delete h_B1;
    delete h_B2;
    delete h_dB;
}

void
zrscan( TVirtualPad* pad, double maxz, double maxr, double phi = 0, int n = 1000 )
{
    TH2D* h_B1 = new TH2D("h_B1",Form("|B_{1}| @ #phi = %.2f#pi;z (mm);r (mm)",phi/PI),n,-maxz,maxz,n,-maxr,maxr);
    TH2D* h_B2 = new TH2D("h_B2","|B_{2}|;z (mm);r (mm)",n,-maxz,maxz,n,-maxr,maxr);
    TH2D* h_dB = new TH2D("h_dB","|B_{1} - B_{2}|/|B_{1}| (%);z (mm);r (mm)",n,-maxz,maxz,n,-maxr,maxr);
    h_B1->SetStats(0); h_B2->SetStats(0); h_dB->SetStats(0);
    for ( int i = 0; i < n; i++ ) for ( int j = 0; j < n; j++ ) {
        double z = (2.*(i+0.5)/n-1.)*maxz;
        double r = (2.*(j+0.5)/n-1.)*maxr;
        double pos[3];
        pos[0] = cos(phi)*r; pos[1] = sin(phi)*r; pos[2] = z;
        TVector3 B[2];
        for ( int k = 0; k < 2; k++ ) {
            double BkT[3];
            bmap[k].getB( pos, BkT ); // in kT
            B[k] = BkT;
            B[k] *= 1000.; // in T
        }
        h_B1->Fill(z,r,B[0].Mag());
        h_B2->Fill(z,r,B[1].Mag());
        TVector3 deltaB = B[0] - B[1];
        h_dB->Fill(z,r,deltaB.Mag()/B[0].Mag()*100);
    }
    pad->Divide(1,3);
    pad->cd(1);
    h_B1->DrawCopy("colz");
    pad->cd(2);
    h_B2->DrawCopy("colz");
    pad->cd(3);
    h_dB->SetMaximum(10);
    h_dB->SetMinimum(0);
    h_dB->DrawCopy("colz");
    pad->Update();
    delete h_B1;
    delete h_B2;
    delete h_dB;
}

void
zphiscan( TVirtualPad* pad, double maxz, double r = 4000, int n = 1000 )
{
    TH2D* h_B1 = new TH2D("h_B1","|B_{1}|;z (mm);#phi (rad)",n,-maxz,maxz,n,0,2.*M_PI);
    TH2D* h_B2 = new TH2D("h_B2","|B_{2}|;z (mm);#phi (rad)",n,-maxz,maxz,n,0,2.*M_PI);
    TH2D* h_dB = new TH2D("h_dB","|B_{1} - B_{2}|/|B_{1}| (%);z (mm);#phi (rad)",n,-maxz,maxz,n,0,2.*M_PI);
    h_B1->SetStats(0); h_B2->SetStats(0); h_dB->SetStats(0);
    for ( int i = 0; i < n; i++ ) for ( int j = 0; j < n; j++ ) {
        double z = (2.*(i+0.5)/n-1.)*maxz;
        double phi = 2.*(j+0.5)/n*M_PI;
        double pos[3];
        pos[0] = cos(phi)*r; pos[1] = sin(phi)*r; pos[2] = z;
        TVector3 B[2];
        for ( int k = 0; k < 2; k++ ) {
            double BkT[3];
            bmap[k].getB( pos, BkT ); // in kT
            B[k] = BkT;
            B[k] *= 1000.; // in T
        }
        h_B1->Fill(z,phi,B[0].Mag());
        h_B2->Fill(z,phi,B[1].Mag());
        TVector3 deltaB = B[0] - B[1];
        h_dB->Fill(z,phi,deltaB.Mag()/B[0].Mag()*100);
    }
    pad->Divide(1,3);
    pad->cd(1);
    h_B1->DrawCopy("colz");
    pad->cd(2);
    h_B2->DrawCopy("colz");
    pad->cd(3);
    h_dB->SetMaximum(10);
    h_dB->SetMinimum(0);
    h_dB->DrawCopy("colz");
    pad->Update();
    delete h_B1;
    delete h_B2;
    delete h_dB;
}

int
main( int argc, const char* argv[] )
{
    if ( argc != 3 ) {
        cerr << "usage: compareMaps <map1> <map2>" << endl;
        return 1;
    }
    TString mapfile[2];
    for ( int i=0; i<2; i++ ) mapfile[i] = argv[i+1];
    bmap = new BFieldMap[2];
    for ( int i=0; i<2; i++ ) {
        if ( bmap[i].readMap(mapfile[i]) ) return 1;
    }

    TH1::AddDirectory(false); // histograms will not belong to TDirectory
    gStyle->SetNumberContours(100); // high-resolution color map

    TString title;
    title += mapfile[0];
    title += " vs. ";
    title += mapfile[1];
    int Ncol = 8;
    int Nrow = 2; 
    TCanvas* c = new TCanvas("c",title,500*Ncol,1500*Nrow);
    c->Divide(Ncol,Nrow);
    /* x-y scan @ fixed z */
    //xyscan(c->GetPad(1),14000,0,500);
    //xyscan(c->GetPad(2),14000,10000,500);
    /* z-r scan @ fixed phi */
    //zrscan(c->GetPad(3),15000,14000,0.375*M_PI,500); // sector 4-12
    //zrscan(c->GetPad(4),15000,14000,0.5*M_PI,500); // sector 5-13
    //zrscan(c->GetPad(3),3000,2000,0.375*M_PI,500); // sector 4-12
    //zrscan(c->GetPad(4),3000,2000,0.5*M_PI,500); // sector 5-13
    /* z-phi scan @ fixed r */
    //zphiscan("c6",15000,4000,200);
    double phi1 = -1, phi2 = -2;
    for (int i = 0; i < Ncol*Nrow; i++)
      zrscan(c->GetPad(i+1),15000,14000,phi1+(phi2-phi1)*(double)i/(double)Ncol,500);
    c->Print("compareMaps_phiFeetRegionScan.png");
}
