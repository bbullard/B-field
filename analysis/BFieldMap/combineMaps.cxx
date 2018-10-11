// combineMaps.cxx
//
// combine a Toroid and Solenoid maps, with a shift + tilt.
//
#include "BFieldMap.h"
#include "BFieldSolenoid.h"
#include <fstream>
#include "TFile.h"
using namespace std;

void usage()
{
    cout << "usage: combineMaps <toroidmap> [<solenoidmap>] <combinedmap>" << endl;
    cout << "    <toroidmap>   can be bmagatlas_09_fullAsym20400.data" << endl;
    cout << "    <solenoidmap> can be map7730bes2.grid" << endl;
    cout << "    <combinedmap> can be BFieldMap_FullAsym_20400.root" << endl;
    cout << "If <solenoidmap> is missing, simply convert <toroidmap> to <combinedmap>" << endl;
}

int main( int argc, char** argv )
{
    const char* toroidmap(0);
    const char* solenoidmap(0);
    const char* rootname(0);

    // check parameters
    if ( argc == 4 ) {
        toroidmap = argv[1];
        solenoidmap = argv[2];
        rootname = argv[3];
    } else if ( argc == 3 ) {
        toroidmap = argv[1];
        rootname = argv[2];
    } else {
        usage();
        return 1;
    }

    // read the toroid map from ASCII
    cout << "Reading the map from " << toroidmap << endl;
    ifstream input( toroidmap );
    BFieldMap map;
    map.readMap( input );
    input.close();

    if ( solenoidmap ) {
        // read the solenoid map from ASCII
        cout << "Reading the map from " << solenoidmap << endl;
        ifstream input2( solenoidmap );
        BFieldSolenoid solenoid;
        solenoid.readMap( input2 );
        input2.close();

        // move and tilt the solenoid
        solenoid.moveMap( 0.0, 1.6, 0.0, 0.55e-3, -0.53e-3 );
        const BFieldMesh<double> *tilted = solenoid.tiltedMap();

        // create a zone that represents the solenoid
        const int newid(7000); // zone ID of the new solenoid field
        const double bscale(1e-7); // unit 1 gauss = 1e-7 kT
        BFieldZone newzone( newid, tilted->zmin(), tilted->zmax(), tilted->rmin(), tilted->rmax(),
                            tilted->phimin(), tilted->phimax(), bscale );
        for ( int i = 0; i < 3; i++ ) { // z, r, phi
            for ( unsigned j = 0; j < tilted->nmesh(i); j++ ) {
                newzone.appendMesh( i, tilted->mesh(i,j) ); // copy mesh
            }
        }
        for ( unsigned i = 0; i < tilted->nfield(); i++ ) {
            BFieldVector<double> field( tilted->field(i) );
            short iz = (field.z()>=0) ? (short)(field.z()/bscale+0.5) : (short)(field.z()/bscale-0.5);
            short ir = (field.r()>=0) ? (short)(field.r()/bscale+0.5) : (short)(field.r()/bscale-0.5);
            short iphi = (field.phi()>=0) ? (short)(field.phi()/bscale+0.5) : (short)(field.phi()/bscale-0.5);
            newzone.appendField( BFieldVector<short>( iz, ir, iphi ) );
        }
        // append the zone to the toroid map
        map.appendZone( newzone );
    }

    // write the combined map
    cout << "Writing the map to " << rootname << endl;
    TFile* rootfile = new TFile( rootname, "RECREATE" );
    map.writeMap( rootfile );
    rootfile->Close();

    return 0;
}
