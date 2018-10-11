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

class BphiMap : public BFieldMap { // map with Bphi() function
public:
    BphiMap() : BFieldMap() {;}
    double Bphi( double *x, double *p ) {
        TVector3 pos;
        pos.SetPtEtaPhi(x[0], p[0], p[1]);
        double xyz[3], B[3];
        pos.GetXYZ(xyz);
        getB(xyz, B);
        TVector3 field(B);
        field.RotateZ(-p[1]);
        return field.Y();
    };
};

double
IntBphi( TF1* fbphi, double eta, double phi )
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
    fbphi->SetParameter(0,eta);
    fbphi->SetParameter(1,phi);
    return fbphi->Integral( Rstart, Rend );
}

int
main( int argc, const char* argv[] )
{
    if ( argc != 3 ) {
        cerr << "usage: compIntBphi <map1> <map2>" << endl;
        return 1;
    }
    // maps
    BphiMap* bmap = new BphiMap[2];
    if ( bmap[0].readMap(argv[1]) ) return 1;
    if ( bmap[1].readMap(argv[2]) ) return 1;
    // Bphi of each map
    TF1* fbphi[2];
    fbphi[0] = new TF1( "fbphi0", bmap+0, &BphiMap::Bphi, 0.0, 14000.0, 2, "BphiMap", "Bphi" );
    fbphi[1] = new TF1( "fbphi1", bmap+1, &BphiMap::Bphi, 0.0, 14000.0, 2, "BphiMap", "Bphi" );
    for ( int i=0; i<2; i++ ) {
        fbphi[i]->SetParameter(0,0.0); fbphi[i]->SetParameter(1,0.0); // eta and phi
    }

    TFile* outputfile = new TFile("compIntBphi_out.root","RECREATE","comIntBphi output");
    //TH1::AddDirectory(false); // histograms will not belong to TDirectory
    gStyle->SetNumberContours(100); // high-resolution color map

    TString title;
    title += argv[1];
    title += " vs. ";
    title += argv[2];
    TCanvas* c = new TCanvas("c",title,2000,1500);
    c->Divide(2,2);

    TH2D* hIBphi[2];
    const int nbineta(100);
    const int nbinphi(100);
    for ( int i=0; i<2; i++ ) {
        char name[100];
        sprintf( name, "hIBphi%d", i );
        char title[200];
        sprintf( title, "Integral(B_{%i,#phi}) %s", i+1, argv[i+1] );
        hIBphi[i] = new TH2D( name, title, nbineta, -2.7, 2.7, nbinphi, -PI, PI );
        hIBphi[i]->SetXTitle("#eta");
        hIBphi[i]->SetYTitle("#phi");
        hIBphi[i]->SetStats(false);
    }
    for ( int j=0; j<nbineta; j++ ) {
        double eta = -2.7 + 5.4/nbineta*(j+0.5);
        for ( int k=0; k<nbinphi; k++ ) {
            double phi = -PI + 2*PI/nbinphi*(k+0.5);
            for ( int i=0; i<2; i++ ) {
                hIBphi[i]->Fill( eta, phi, IntBphi( fbphi[i], eta, phi ) );
            }
        }
    }
  
    hIBphi[0]->SetMaximum(3.5);
    hIBphi[1]->SetMaximum(3.5);
    hIBphi[0]->SetMinimum(-.5);
    hIBphi[1]->SetMinimum(-.5);
  
    // for 2016-2015
    double hist_SF = 1.;
    // for 2016-2012
    //hist_SF = 10.;

    c->cd(1);
    hIBphi[0]->Draw("colz");
    c->cd(2);
    hIBphi[1]->Draw("colz");
    c->cd(3);
    TH2D* hdiff = new TH2D( *hIBphi[1] );
    hdiff->Add( hIBphi[0], -1 );
    hdiff->SetMaximum(.006*hist_SF);
    hdiff->SetMinimum(-.006*hist_SF);
    hdiff->SetTitle("#delta(B_{21,#phi})");
    hdiff->Draw("colz");
    c->cd(4);
    TH2D* hdiv = new TH2D( *hdiff );
    hdiv->Divide( hIBphi[0]);
    hdiv->SetTitle("#delta(B_{21,#phi})/B_{1,#phi}");
    hdiv->SetMaximum(0.003*hist_SF);
    hdiv->SetMinimum(-0.003*hist_SF);
    hdiv->Draw("colz");

    c->Print("compIntBphi.png");
    outputfile->Write();
    outputfile->Close();
}
