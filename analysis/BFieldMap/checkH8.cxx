#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

using namespace std;

#include "BFieldH8Map.h"

int main( int argc, char* argv[] )
{
    TFile* rootfile = new TFile("checkH8.root","RECREATE");
    // create the map
    ifstream input("mbps1-200-sourcex4.data");
    BFieldH8Map map;
    map.readMap( input );

    TH2D* hBx = new TH2D("hBx","Bx",100,-2300,2300,100,-260,260);
    TH2D* hBy = new TH2D("hBy","By",100,-2300,2300,100,-260,260);
    TH2D* hBz = new TH2D("hBz","Bz",100,-2300,2300,100,-260,260);
    double xyz[3];
    xyz[2] = 0;
    for ( int i=0; i<100; i++ ) {
        xyz[0] = hBx->GetXaxis()->GetBinCenter(i+1);
        for ( int j=0; j<100; j++ ) {
            xyz[1] = hBx->GetYaxis()->GetBinCenter(j+1);
            double B[3];
            map.getB( xyz, B );
            hBx->Fill( xyz[0], xyz[1], B[0] );
            hBy->Fill( xyz[0], xyz[1], B[1] );
            hBz->Fill( xyz[0], xyz[1], B[2] );
        }
    }

    rootfile->Write();
    rootfile->Close();
}
