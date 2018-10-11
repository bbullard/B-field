// $Id$

// C++ standard libraries
#include <iostream>
using namespace std;
// ROOT libraries
#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TString.h"
#include "TStyle.h"
// BField code
#include "BFieldMap.h"

const double PI(3.14159265358979);

class BthetaMap : public BFieldMap { // map with Btheta() function
public:
    BthetaMap() : BFieldMap() {;}
    double Btheta( double *x, double *p ) {
        TVector3 pos;
        pos.SetPtEtaPhi(x[0], p[0], p[1]);
        double xyz[3], B[3];
        pos.GetXYZ(xyz);
        getB(xyz, B);
        TVector3 field(B);
        field.RotateZ(-p[1]);
        field.RotateY(PI-p[0]);
        return field.Z();
    };
};

double
IntBtheta( TF1* fbtheta, double eta, double phi )
{
    // range of integration
    const double Rin(4300), Rout(12200), Zin(7100), Zout(15000);
    double tantheta = tan(2.0*atan(exp(-eta)));
    double Rstart(Rin);
    if ( ( tantheta < Rin/Zin ) && ( tantheta > -Rin/Zin ) ) {
        Rstart = abs(tantheta*Zin);
    }
    double Rend(Rout);
    if ( ( tantheta < Rout/Zout ) && ( tantheta > -Rout/Zout ) ) {
        Rend = abs(tantheta*Zout);
    }
    fbtheta->SetParameter(0,eta);
    fbtheta->SetParameter(1,phi);
    return fbtheta->Integral( Rstart, Rend );
}

int
main( int argc, const char* argv[] )
{
    if ( argc != 3 ) {
        cerr << "usage: compIntBtheta <map1> <map2>" << endl;
        return 1;
    }
    // maps
    BthetaMap* bmap = new BthetaMap[2];
    if ( bmap[0].readMap(argv[1]) ) return 1;
    if ( bmap[1].readMap(argv[2]) ) return 1;
    // Bphi of each map
    TF1* fbtheta[2];
    fbtheta[0] = new TF1( "fbtheta0", bmap+0, &BthetaMap::Btheta, 0.0, 14000.0, 2, "BthetaMap", "Btheta" );
    fbtheta[1] = new TF1( "fbtheta1", bmap+1, &BthetaMap::Btheta, 0.0, 14000.0, 2, "BthetaMap", "Btheta" );
    for ( int i=0; i<2; i++ ) {
        fbtheta[i]->SetParameter(0,0.0); fbtheta[i]->SetParameter(1,0.0); // eta and phi
    }

    TFile* outputfile = new TFile("compThetaBphi_out.root","RECREATE","comThetaBphi output");
    //TH1::AddDirectory(false); // histograms will not belong to TDirectory
    gStyle->SetNumberContours(100); // high-resolution color map

    TString title;
    title += argv[1];
    title += " vs. ";
    title += argv[2];
    TCanvas* c = new TCanvas("c",title,2000,1500);
    c->Divide(2,2);

    TH2D* hIBtheta[2];
    const int nbineta(100);
    const int nbinphi(100);
    for ( int i=0; i<2; i++ ) {
        char name[100];
        sprintf( name, "hIBphi%d", i );
        char title[200];
        sprintf( title, "Integral(B_{%i,#theta}) %s", i+1, argv[i+1] );
        hIBtheta[i] = new TH2D( name, title, nbineta, -2.7, 2.7, nbinphi, -PI, PI );
        hIBtheta[i]->SetXTitle("#eta");
        hIBtheta[i]->SetYTitle("#phi");
        hIBtheta[i]->SetStats(false);
    }
    for ( int j=0; j<nbineta; j++ ) {
        double eta = -2.7 + 5.4/nbineta*(j+0.5);
        for ( int k=0; k<nbinphi; k++ ) {
            double phi = -PI + 2*PI/nbinphi*(k+0.5);
            for ( int i=0; i<2; i++ ) {
                hIBtheta[i]->Fill( eta, phi, IntBtheta( fbtheta[i], eta, phi ) );
            }
        }
    }
  
    hIBtheta[0]->SetMaximum(1.5);
    hIBtheta[1]->SetMaximum(1.5);
    hIBtheta[0]->SetMinimum(-1.5);
    hIBtheta[1]->SetMinimum(-1.5);
  
    // for 2016-2015
    double hist_SF = 1.;
    // for 2016-2012
    //hist_SF = 10.;

    c->cd(1);
    hIBtheta[0]->Draw("colz");
    c->cd(2);
    hIBtheta[1]->Draw("colz");
    c->cd(3);
    TH2D* hdiff = new TH2D( *hIBtheta[1] );
    hdiff->Add( hIBtheta[0], -1 );
    hdiff->SetMaximum(.01*hist_SF);
    hdiff->SetMinimum(-.01*hist_SF);
    hdiff->SetTitle("#delta(B_{21,#theta})");
    hdiff->Draw("colz");
    c->cd(4);
    TH2D* hdiv = new TH2D( *hdiff );
    hdiv->Divide( hIBtheta[0] );
    hdiv->SetTitle("#delta(B_{21,#theta})/B_{1,#theta}");
    hdiv->SetMaximum(1*hist_SF);
    hdiv->SetMinimum(-1*hist_SF);
    hdiv->Draw("colz");

    c->Print("compIntBtheta.png");
    outputfile->Write();
    outputfile->Close();
}
