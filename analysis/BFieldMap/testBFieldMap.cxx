// testBFieldMap.cc
//
// test code for the C++ version
//
// Masahiro Morii, Harvard University
//
#include "BFieldMap.h"
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
using namespace std;

int main( int argc, char** argv )
{
    if ( argc != 2 ) {
        cout << "usage: testBFieldMap <mapfile>" << endl;
        return 1;
    }
    BFieldMap map;
    map.readMap( argv[1] );

    cout << "Type in x y z coordinates in mm." << endl;
    while ( cin.good() ) {
        double x, y, z;
        cout << "x y z = " << flush;
        cin >> x >> y >> z;
        if ( ! cin.good() ) break;
        double pos[3] = { x, y, z };
        double B[3];
        map.getB( pos, B );
        cout << "Bx By Bz = " << B[0]*1000 << " " << B[1]*1000 << " " << B[2]*1000 << " in tesla" << endl;
    }
    return 0;
}
