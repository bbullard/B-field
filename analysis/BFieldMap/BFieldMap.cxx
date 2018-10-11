//
// BFieldMap.cxx
//
#include "BFieldMap.h"
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include "TTree.h"
using namespace std;

//
// Read the solenoid map from file.
// If the filename ends with ".root", it's in a ROOT format.
// Otherwise, it's in a compressed ASCII format.
//
int
BFieldMap::readMap( const char* filename )
{
    if ( strstr( filename, ".root" ) != 0 ) {
        TFile* rootfile = new TFile( filename, "OLD" );
        if ( ! rootfile ) {
            cerr << "BFieldMap::readMap(): failed to open " << filename << endl;
            return 1;
        }
        readMap( rootfile );
        rootfile->Close();
        delete rootfile;
    } else {
        ifstream textfile( filename );
        if ( ! textfile.good() ) {
            cerr << "BFieldMap::readMap(): failed to open " << filename << endl;
            return 1;
        }
        readMap( textfile );
    } 
    return 0;
}

//
// read an ASCII field map from istream
// return 0 if successful
// convert units m -> mm, and T -> kT
//
int
BFieldMap::readMap( istream& input )
{
    const double degree(M_PI/180.0); // degree in radians
    const double meter(1000.0);      // meter in mm
    const double tesla(0.001);       // tesla in kT
    const string myname("BFieldMap::readMap()");
    // first line contains version, date, time
    string word;
    int version;
    int date;
    int time;
    input >> word >> version;
    if ( word != "FORMAT-VERSION" ) {
        cerr << myname << ": found '" << word << "' instead of 'FORMAT-VERION'" << endl;
        return 1;
    }
    if ( version < 5 || version > 6 ) {
        cerr << myname << ": version number is " << version << " instead of 5 or 6" << endl;
        return 1;
    }
    input >> word >> date;
    if ( word != "DATE" ) {
        cerr << myname << ": found '" << word << "' instead of 'DATE'" << endl;
        return 1;
    }
    input >> word >> time;
    if ( word != "TIME" ) {
        cerr << myname << ": found '" << word << "' instead of 'TIME'" << endl;
        return 1;
    }

    // read and skip header cards
    int nheader;
    input >> word >> nheader;
    if ( word != "HEADERS" ) {
        cerr << myname << ": found '" << word << "' instead of 'HEADERS'" << endl;
        return 1;
    }
    string restofline;
    getline( input, restofline );
    for ( int i = 0; i < nheader; i++ ) {
        string header;
        getline( input, header );
    }

    // read zone definitions
    int nzone;
    input >> word >> nzone;
    if ( word != "ZONES" ) {
        cerr << myname << ": found '" << word << "' instead of 'ZONES'" << endl;
        return 1;
    }
    vector<int> jz(nzone), nz(nzone);
    vector<int> jr(nzone), nr(nzone);
    vector<int> jphi(nzone), nphi(nzone);
    vector<int> jbs(nzone), nbs(nzone);
    vector<int> jcoil(nzone), ncoil(nzone);
    vector<int> jfield(nzone), nfield(nzone);
    vector<int> jaux(nzone), naux(nzone);
    for ( int i = 0; i < nzone; i++ )
    {
        int id;
        int nrep, map; // unused
        double z1, z2, r1, r2, phi1, phi2;
        int nzrphi0; // unused
        double tol; // unused
        int mzn, mxsym, mrefl, mback; // unused
        double qz, qr, qphi; // unused
        double bscale;
        input >> id >> nrep;
        if ( version == 6 ) input >> map;
        input >> z1 >> z2 >> nz[i]
              >> r1 >> r2 >> nr[i]
              >> phi1 >> phi2 >> nphi[i]
              >> nzrphi0 >> tol
              >> jbs[i] >> nbs[i]
              >> jcoil[i] >> ncoil[i]
              >> jz[i] >> jr[i] >> jphi[i]
              >> jfield[i] >> nfield[i]
              >> mzn;
        if ( version == 6 ) input >> mxsym;
        input >> mrefl >> mback
              >> jaux[i] >> naux[i]
              >> qz >> qr >> qphi >> bscale;
        if ( id >= 0 ) { // remove dummy zone
            z1 *= meter;
            z2 *= meter;
            r1 *= meter;
            r2 *= meter;
            phi1 *= degree;
            phi2 *= degree;
            bscale *= tesla;
            BFieldZone zone( id, z1, z2, r1, r2, phi1, phi2, bscale );
            m_zone.push_back(zone);
        }
    }

    // read Biot-Savart data
    int nbiot;
    input >> word >> nbiot;
    if ( word != "BIOT" ) {
        cerr << myname << ": found '" << word << "' instead of 'BIOT'" << endl;
        return 1;
    }
    vector<BFieldCond> bslist;
    for ( int i = 0; i < nbiot; i++ ) {
        char dummy; // unused
        char cfinite;
        double xyz1[3], xyz2[3];
        double phirot; // unused
        double curr;
        input >> dummy >> cfinite
              >> xyz1[0] >> xyz1[1] >> xyz1[2]
              >> xyz2[0] >> xyz2[1] >> xyz2[2]
              >> phirot >> curr;
        bool finite = ( cfinite == 'T' );
        for ( int j = 0; j < 3; j++ ) {
            xyz1[j] *= meter;
            if ( finite ) xyz2[j] *= meter;
        }
        BFieldCond bs( finite, xyz1, xyz2, curr );
        bslist.push_back(bs);
    }
    // attach them to the zones
    for ( unsigned i = 0; i < m_zone.size(); i++ ) {
        // copy the range that belongs to this zone
        for ( int j = 0; j < nbs[i]; j++ ) {
            // Fortran -> C conversion requires "-1"
            m_zone[i].appendCond( bslist[jbs[i]+j-1] );
        }
    }

    // read and skip coil data
    int nc;
    input >> word >> nc;
    if ( word != "COIL" ) {
        cerr << myname << ": found '" << word << "' instead of 'COIL'" << endl;
        return 1;
    }
    getline( input, restofline );
    for ( int i = 0; i < nc; i++ ) {
        string coildata;
        getline( input, coildata );
    }

    // read and skip auxiliary array = list of subzones
    int nauxarr;
    input >> word >> nauxarr;
    if ( word != "AUXARR" ) {
        cerr << myname << ": found '" << word << "' instead of 'AUXARR'" << endl;
        return 1;
    }
    if ( version == 6 ) input >> word; // skip 'T'
    for ( int i = 0; i < nauxarr; i++ ) {
        int aux;
        input >> aux;
    }

    // read mesh definition
    int nmesh;
    input >> word >> nmesh;
    if ( word != "MESH" ) {
        cerr << myname << ": found '" << word << "' instead of 'MESH'" << endl;
        return 1;
    }
    vector<double> meshlist;
    for ( int i = 0; i < nmesh; i++ ) {
        double mesh;
        input >> mesh;
        meshlist.push_back(mesh);
    }
    // attach them to the zones
    for ( unsigned i = 0; i < m_zone.size(); i++ ) {
        m_zone[i].reserve( nz[i], nr[i], nphi[i] );
        for ( int j = 0; j < nz[i]; j++ ) {
            m_zone[i].appendMesh( 0, meshlist[jz[i]+j-1]*meter );
        }
        for ( int j = 0; j < nr[i]; j++ ) {
            m_zone[i].appendMesh( 1, meshlist[jr[i]+j-1]*meter );
        }
        for ( int j = 0; j < nphi[i]; j++ ) {
            m_zone[i].appendMesh( 2, meshlist[jphi[i]+j-1] );
        }
    }

    // read field values
    int nf, nzlist;
    string ftype, bytype;
    input >> word >> nf >> nzlist >> ftype >> bytype;
    if ( word != "FIELD" ) {
        cerr << myname << ": found '" << word << "' instead of 'FIELD'" << endl;
        return 1;
    }
    if ( ftype != "I2PACK" ) {
        cerr << myname << ": found '" << ftype << "' instead of 'I2PACK'" << endl;
        return 1;
    }
    if ( bytype != "FBYTE" ) {
        cerr << myname << ": found '" << bytype << "' instead of 'FBYTE'" << endl;
        return 1;
    }
    // read zone by zone
    for ( int i = 0; i < nzlist; i++ ) {
        int izone, idzone, nfzone;
        input >> izone >> idzone >> nfzone;
        izone--; // fortran -> C++
        if ( idzone != m_zone[izone].id() ) {
            cerr << myname << ": zone id " << idzone << " != " << m_zone[izone].id() << endl;
            return 2;
        }

        vector<int> data[3];

        // for field data in 2 bytes
        for ( int j = 0; j < 3; j++ ) { // repeat z, r, phi
            int ierr = read_packed_data( input, data[j] );
            if ( ierr != 0 ) return ierr;
            for ( int k = 0; k < nfzone; k++ ) {
                // recover sign
                data[j][k] = ( data[j][k]%2==0 ) ? data[j][k]/2 : -(data[j][k]+1)/2;
                // second-order diff
                if ( k >= 2 ) data[j][k] += 2*data[j][k-1] - data[j][k-2];
            }
        }
        // store
        for ( int k = 0; k < nfzone; k++ ) {
            BFieldVector<short> B( data[0][k], data[1][k], data[2][k] );
            m_zone[izone].appendField( B );
        }

        // skip fbyte
        char c;
        while ( input.good() ) {
            input >> c;
            if ( input.eof() || c == '}' ) break;
        }
    }

    // build the LUTs
    buildLUT();

    return 0;
}

//
// wrire the map to a ROOT file
//
void
BFieldMap::writeMap( TFile* rootfile )
{
    if ( rootfile == 0 ) return; // no file
    if ( rootfile->cd() == false ) return; // could not make it current directory
    // define the tree
    TTree* tree = new TTree( "BFieldMap", "BFieldMap version 6" );
    TTree* tmax = new TTree( "BFieldMapSize", "Buffer size information" );
    int id;
    double zmin, zmax, rmin, rmax, phimin, phimax;
    double bscale;
    int ncond;
    bool *finite;
    double *p1x, *p1y, *p1z, *p2x, *p2y, *p2z;
    double *curr;
    int nmeshz, nmeshr, nmeshphi;
    double *meshz, *meshr, *meshphi;
    int nfield;
    short *fieldz, *fieldr, *fieldphi;
    // prepare arrays - need to know the maximum sizes
    unsigned maxcond(0), maxmeshz(0), maxmeshr(0), maxmeshphi(0), maxfield(0);
    for ( unsigned i = 0; i < m_zone.size(); i++ ) {
        maxcond = max( maxcond, m_zone[i].ncond() );
        maxmeshz = max( maxmeshz, m_zone[i].nmesh(0) );
        maxmeshr = max( maxmeshr, m_zone[i].nmesh(1) );
        maxmeshphi = max( maxmeshphi, m_zone[i].nmesh(2) );
        maxfield = max( maxfield, m_zone[i].nfield() );
    }
    // store the maximum sizes
    tmax->Branch( "maxcond", &maxcond, "maxcond/i" );
    tmax->Branch( "maxmeshz", &maxmeshz, "maxmeshz/i" );
    tmax->Branch( "maxmeshr", &maxmeshr, "maxmeshr/i" );
    tmax->Branch( "maxmeshphi", &maxmeshphi, "maxmeshphi/i" );
    tmax->Branch( "maxfield", &maxfield, "maxfield/i" );
    tmax->Fill();
    // prepare buffers
    finite = new bool[maxcond];
    p1x = new double[maxcond];
    p1y = new double[maxcond];
    p1z = new double[maxcond];
    p2x = new double[maxcond];
    p2y = new double[maxcond];
    p2z = new double[maxcond];
    curr = new double[maxcond];
    meshz = new double[maxmeshz];
    meshr = new double[maxmeshr];
    meshphi = new double[maxmeshphi];
    fieldz = new short[maxfield];
    fieldr = new short[maxfield];
    fieldphi = new short[maxfield];
    // define the tree branches
    tree->Branch( "id", &id, "id/I" );
    tree->Branch( "zmin", &zmin, "zmin/D" );
    tree->Branch( "zmax", &zmax, "zmax/D" );
    tree->Branch( "rmin", &rmin, "rmin/D" );
    tree->Branch( "rmax", &rmax, "rmax/D" );
    tree->Branch( "phimin", &phimin, "phimin/D" );
    tree->Branch( "phimax", &phimax, "phimax/D" );
    tree->Branch( "bscale", &bscale, "bscale/D" );
    tree->Branch( "ncond", &ncond, "ncond/I" );
    tree->Branch( "finite", finite, "finite[ncond]/O" );
    tree->Branch( "p1x", p1x, "p1x[ncond]/D" );
    tree->Branch( "p1y", p1y, "p1y[ncond]/D" );
    tree->Branch( "p1z", p1z, "p1z[ncond]/D" );
    tree->Branch( "p2x", p2x, "p2x[ncond]/D" );
    tree->Branch( "p2y", p2y, "p2y[ncond]/D" );
    tree->Branch( "p2z", p2z, "p2z[ncond]/D" );
    tree->Branch( "curr", curr, "curr[ncond]/D" );
    tree->Branch( "nmeshz", &nmeshz, "nmeshz/I" );
    tree->Branch( "meshz", meshz, "meshz[nmeshz]/D" );
    tree->Branch( "nmeshr", &nmeshr, "nmeshr/I" );
    tree->Branch( "meshr", meshr, "meshr[nmeshr]/D" );
    tree->Branch( "nmeshphi", &nmeshphi, "nmeshphi/I" );
    tree->Branch( "meshphi", meshphi, "meshphi[nmeshphi]/D" );
    tree->Branch( "nfield", &nfield, "nfield/I" );
    tree->Branch( "fieldz", fieldz, "fieldz[nfield]/S" );
    tree->Branch( "fieldr", fieldr, "fieldr[nfield]/S" );
    tree->Branch( "fieldphi", fieldphi, "fieldphi[nfield]/S" );
    //tree->Branch( "fbyte", fbyte, "fbyte[nfield]/b" );
    // loop over zones to write
    for ( unsigned i = 0; i < m_zone.size(); i++ ) {
        const BFieldZone z = m_zone[i];
        id = z.id();
        zmin = z.zmin(); zmax = z.zmax();
        rmin = z.rmin(); rmax = z.rmax();
        phimin = z.phimin(); phimax = z.phimax();
        bscale = z.bscale();
        ncond = z.ncond();
        for ( int j = 0; j < ncond; j++ ) {
            const BFieldCond c = z.cond(j);
            finite[j] = c.finite();
            p1x[j] = c.p1(0);
            p1y[j] = c.p1(1);
            p1z[j] = c.p1(2);
            p2x[j] = c.p2(0);
            p2y[j] = c.p2(1);
            p2z[j] = c.p2(2);
            curr[j] = c.curr();
        }
        nmeshz = z.nmesh(0);
        for ( int j = 0; j < nmeshz; j++ ) {
            meshz[j] = z.mesh(0,j);
        }
        nmeshr = z.nmesh(1);
        for ( int j = 0; j < nmeshr; j++ ) {
            meshr[j] = z.mesh(1,j);
        }
        nmeshphi = z.nmesh(2);
        for ( int j = 0; j < nmeshphi; j++ ) {
            meshphi[j] = z.mesh(2,j);
        }
        nfield = z.nfield();
        for ( int j = 0; j < nfield; j++ ) {
            const BFieldVector<short> f = z.field(j);
            fieldz[j] = f.z();
            fieldr[j] = f.r();
            fieldphi[j] = f.phi();
        }
        tree->Fill();
    }
    rootfile->Write();
    // clean up
    delete[] finite;
    delete[] p1x;
    delete[] p1y;
    delete[] p1z;
    delete[] p2x;
    delete[] p2y;
    delete[] p2z;
    delete[] curr;
    delete[] meshz;
    delete[] meshr;
    delete[] meshphi;
    delete[] fieldz;
    delete[] fieldr;
    delete[] fieldphi;
}

//
// read the map from a ROOT file.
// returns 0 if successful.
//
int
BFieldMap::readMap( TFile* rootfile )
{
    if ( rootfile == 0 ) return 1; // no file
    if ( rootfile->cd() == false ) return 2; // could not make it current directory
    // open the tree
    TTree* tree = (TTree*)rootfile->Get("BFieldMap");
    if ( tree == 0 ) return 3; // no tree
    int id;
    double zmin, zmax, rmin, rmax, phimin, phimax;
    double bscale;
    int ncond;
    bool *finite;
    double *p1x, *p1y, *p1z, *p2x, *p2y, *p2z;
    double *curr;
    int nmeshz, nmeshr, nmeshphi;
    double *meshz, *meshr, *meshphi;
    int nfield;
    short *fieldz, *fieldr, *fieldphi;
    // define the fixed-sized branches first
    tree->SetBranchAddress( "id", &id );
    tree->SetBranchAddress( "zmin", &zmin );
    tree->SetBranchAddress( "zmax", &zmax );
    tree->SetBranchAddress( "rmin", &rmin );
    tree->SetBranchAddress( "rmax", &rmax );
    tree->SetBranchAddress( "phimin", &phimin );
    tree->SetBranchAddress( "phimax", &phimax );
    tree->SetBranchAddress( "bscale", &bscale );
    tree->SetBranchAddress( "ncond", &ncond );
    tree->SetBranchAddress( "nmeshz", &nmeshz );
    tree->SetBranchAddress( "nmeshr", &nmeshr );
    tree->SetBranchAddress( "nmeshphi", &nmeshphi );
    tree->SetBranchAddress( "nfield", &nfield );
    // prepare arrays - need to know the maximum sizes
    // open the tree of buffer sizes (may not exist in old maps)
    unsigned maxcond(0), maxmeshz(0), maxmeshr(0), maxmeshphi(0), maxfield(0);
    TTree* tmax = (TTree*)rootfile->Get("BFieldMapSize");
    if ( tmax != 0 ) {
        tmax->SetBranchAddress( "maxcond", &maxcond );
        tmax->SetBranchAddress( "maxmeshz", &maxmeshz );
        tmax->SetBranchAddress( "maxmeshr", &maxmeshr );
        tmax->SetBranchAddress( "maxmeshphi", &maxmeshphi );
        tmax->SetBranchAddress( "maxfield", &maxfield );
        tmax->GetEntry(0);
    } else { // "BFieldMapSize" tree does not exist
        for ( int i = 0; i < tree->GetEntries(); i++ ) {
            tree->GetEntry(i);
            maxcond = max( maxcond, unsigned(ncond) );
            maxmeshz = max( maxmeshz, unsigned(nmeshz) );
            maxmeshr = max( maxmeshr, unsigned(nmeshr) );
            maxmeshphi = max( maxmeshphi, unsigned(nmeshphi) );
            maxfield = max( maxfield, unsigned(nfield) );
        }
    }
    finite = new bool[maxcond];
    p1x = new double[maxcond];
    p1y = new double[maxcond];
    p1z = new double[maxcond];
    p2x = new double[maxcond];
    p2y = new double[maxcond];
    p2z = new double[maxcond];
    curr = new double[maxcond];
    meshz = new double[maxmeshz];
    meshr = new double[maxmeshr];
    meshphi = new double[maxmeshphi];
    fieldz = new short[maxfield];
    fieldr = new short[maxfield];
    fieldphi = new short[maxfield];
    // define the variable length branches
    tree->SetBranchAddress( "finite", finite );
    tree->SetBranchAddress( "p1x", p1x );
    tree->SetBranchAddress( "p1y", p1y );
    tree->SetBranchAddress( "p1z", p1z );
    tree->SetBranchAddress( "p2x", p2x );
    tree->SetBranchAddress( "p2y", p2y );
    tree->SetBranchAddress( "p2z", p2z );
    tree->SetBranchAddress( "curr", curr );
    tree->SetBranchAddress( "meshz", meshz );
    tree->SetBranchAddress( "meshr", meshr );
    tree->SetBranchAddress( "meshphi", meshphi );
    tree->SetBranchAddress( "fieldz", fieldz );
    tree->SetBranchAddress( "fieldr", fieldr );
    tree->SetBranchAddress( "fieldphi", fieldphi );
    //tree->SetBranchAddress( "fbyte", fbyte );
    // reserve the space for m_zone so that it won't move as the vector grows
    m_zone.reserve( tree->GetEntries() );
    // read all tree and store
    for ( int i = 0; i < tree->GetEntries(); i++ ) {
        tree->GetEntry(i);
        BFieldZone z( id, zmin, zmax, rmin, rmax, phimin, phimax, bscale );
        z.reserve( nmeshz, nmeshr, nmeshphi );
        m_zone.push_back(z);
        for ( int j = 0; j < ncond; j++ ) {
            double p1[3], p2[3];
            p1[0] = p1x[j];
            p1[1] = p1y[j];
            p1[2] = p1z[j];
            p2[0] = p2x[j];
            p2[1] = p2y[j];
            p2[2] = p2z[j];
            BFieldCond cond( finite[j], p1, p2, curr[j] );
            m_zone.back().appendCond(cond);
        }
        for ( int j = 0; j < nmeshz; j++ ) {
            m_zone.back().appendMesh( 0, meshz[j] );
        }
        for ( int j = 0; j < nmeshr; j++ ) {
            m_zone.back().appendMesh( 1, meshr[j] );
        }
        for ( int j = 0; j < nmeshphi; j++ ) {
            m_zone.back().appendMesh( 2, meshphi[j] );
        }
        for ( int j = 0; j < nfield; j++ ) {
            BFieldVector<short> field( fieldz[j], fieldr[j], fieldphi[j] );
            m_zone.back().appendField( field );
            //m_zone.back().appendFbyte( fbyte[j] );
        }
    }
    // clean up
    tree->Delete();
    delete[] finite;
    delete[] p1x;
    delete[] p1y;
    delete[] p1z;
    delete[] p2x;
    delete[] p2y;
    delete[] p2z;
    delete[] curr;
    delete[] meshz;
    delete[] meshr;
    delete[] meshphi;
    delete[] fieldz;
    delete[] fieldr;
    delete[] fieldphi;
    //delete[] fbyte;
    // build the LUTs
    buildLUT();

    return 0;
}

//
// utility function used by readMap()
//
int
BFieldMap::read_packed_data( istream& input, vector<int>& data )
{
    const string myname("BFieldMap::read_packed_data()");

    data.resize(0);
    char mode = 'u';
    char c;
    while ( input.good() ) {
        input >> c;
        if ( input.eof() ) return 0;
        else if (c == '}') { // end of record
            return 0;
        }
        else if (c == 'z') { // series of zeros
            int n;
            int ierr = read_packed_int( input, n );
            if ( ierr != 0 ) return ierr;
            for ( int i = 0; i < n; i++ ) {
                data.push_back(0);
            }
        }
        else if (c >= 'u' && c <= 'y') { // mode change
            mode = c;
        }
        else if (c <= ' ' || c > 'z') {
            cerr << myname << ": unexpected letter '" << c << "' in input" << endl;
            return 3;
        }
        else { // normal letter in the range '!' - 't'
            switch (mode) {
            case 'u':
                {
                    input.putback( c );
                    int n;
                    int ierr = read_packed_int( input, n );
                    if ( ierr != 0 ) return ierr;
                    data.push_back(n);
                }
                break;
            case 'v':
                {
                    int n = c - '!';
                    for ( int i = 0; i < 4; i++ ) {
                        input >> c;
                        data.push_back(c - '!' + 84*(n%3));
                        n = n/3;
                    }
                }
                break;
            case 'w':
                data.push_back(c - '!');
                break;
            case 'x':
                {
                    int n = c - '!';
                    data.push_back(n/9);
                    data.push_back(n%9);
                }
                break;
            case 'y':
                {
                    int n = c - '!';
                    data.push_back(n/27);
                    data.push_back((n/9)%3);
                    data.push_back((n/3)%3);
                    data.push_back(n%3);
                }
                break;
            }
        }
    }
    return 0;
}

//
// utility function used by read_packed_data()
//
int
BFieldMap::read_packed_int( istream &input, int &n )
{
    const string myname("BFieldMap::read_packed_int()");
    n = 0;
    char c;
    input >> c;
    while ( c >= '!' && c <= 'J') {
        n = 42*n + c - '!';
        input >> c;
    }
    if ( c >= 'K' && c <= 't' ) {
        n = 42*n + c - 'K';
    } else {
        cerr << myname << ": unexpected letter '" << c << "' in input" << endl;
        return 4;
    }
    return 0;
}

//
// Search for the zone that contains a point (z, r, phi)
// This is a linear-search version, used only to construct the LUT.
//
const BFieldZone*
BFieldMap::findZoneSlow( double z, double r, double phi ) const
{
    for ( int j = m_zone.size()-1; j >= 0; --j ) {
        if ( m_zone[j].inside( z, r, phi ) ) return &m_zone[j];
    }
    return 0;
}

//
// Search for the zone that contains a point (z, r, phi)
// Fast version utilizing the LUT.
//
const BFieldZone*
BFieldMap::findZone( double z, double r, double phi ) const
{
    // make sure it's inside the largest zone
    if ( z < m_edge[0].front() || z > m_edge[0].back() || r > m_edge[1].back() ) return 0;
    // find the edges of the zone
    // z
    const vector<double>& edgez(m_edge[0]);
    int iz = int(_invq[0]*(z-edgez.front())); // index to LUT
    iz = m_edgeLUT[0][iz]; // tentative index from LUT
    if ( z > edgez[iz+1] ) iz++;
    // r
    const vector<double>& edger(m_edge[1]);
    int ir = int(_invq[1]*r); // index to LUT - note minimum r is always 0
    ir = m_edgeLUT[1][ir]; // tentative index from LUT
    if ( r > edger[ir+1] ) ir++;
    // phi
    const vector<double>& edgephi(m_edge[2]);
    int iphi = int(_invq[2]*(phi+M_PI)); // index to LUT - minimum phi is -pi
    iphi = m_edgeLUT[2][iphi]; // tentative index from LUT
    if ( phi > edgephi[iphi+1] ) iphi++;
    // use LUT to get the zone
    int nr = m_edge[1].size() - 1;
    int nphi = m_edge[2].size() - 1;
    return m_zoneLUT[(iz*nr+ir)*nphi+iphi];
}

//
// Returns the magnetic field at any position.
//
void
BFieldMap::getB( const double *xyz, double *B, double * ) const
{
    static const double r2max(14000.*14000.);
    static const double zmax(23000.);
    static const double r2beam(60.*60.);
    static const double zbeam(12850.);
    static const double defaultB(1e-8); // 0.1 gauss in kT
    // is the position inside the valid field volume?
    double z = xyz[2];
    double r2 = xyz[0]*xyz[0] + xyz[1]*xyz[1];
    if ( abs(z) > zmax || r2 > r2max || ( abs(z) > zbeam && r2 < r2beam ) ) {
        B[0] = B[1] = B[2] = defaultB;
        return;
    }
    // convert to cylindrical coordinates
    double r = sqrt(r2);
    double phi = atan2(xyz[1], xyz[0]);
    // test the cache
    if ( ! m_cache.inside( z, r, phi ) ) {
        // outside the last cached bin
        // search for the zone
        m_lastzone = findZone( z, r, phi );
        if ( m_lastzone == 0 ) {
            // outsize all zones (should not happen)
            B[0] = B[1] = B[2] = defaultB;
            return;
        }
        m_lastzone->getCache( z, r, phi, m_cache );
    }
    m_cache.getB( z, r, phi, B );
    m_lastzone->addBiotSavart( xyz, B );
}

//
// Build the look-up table used by FindZone().
// Called by readMap()
// It also calls buildLUT() for all zones.
//
void
BFieldMap::buildLUT()
{
    // make lists of (z,r,phi) edges of all zones
    for ( int j = 0; j < 3; j++ ) { // z, r, phi
        for ( unsigned i = 0; i < m_zone.size(); i++ ) {
            double e[2];
            e[0] = m_zone[i].min(j);
            e[1] = m_zone[i].max(j);
            for ( int k = 0; k < 2; k++ ) {
                // for the phi edge, fit into [-pi,pi]
                if ( j==2 && e[k] > M_PI ) e[k] -= 2.0*M_PI;
                m_edge[j].push_back(e[k]);
            }
        }
        // add -pi and +pi to phi, just in case
        m_edge[2].push_back(-M_PI);
        m_edge[2].push_back(M_PI);
        // sort
        sort( m_edge[j].begin(), m_edge[j].end() );
        // remove duplicates
        // must do this by hand to allow small differences due to rounding in phi
        int i = 0;
        for ( unsigned k = 1; k < m_edge[j].size(); k++ ) {
            if ( fabs(m_edge[j][i] - m_edge[j][k]) < 1.0e-6 ) continue;
            m_edge[j][++i] = m_edge[j][k];
        }
        m_edge[j].resize( i+1 );
        // because of the small differences allowed in the comparison above,
        // m_edge[][] values do not exactly match the m_zone[] boundaries.
        // we have to fix this up in order to avoid invalid field values
        // very close to the zone boundaries.
        for ( unsigned i = 0; i < m_zone.size(); i++ ) {
            for ( unsigned k = 0; k < m_edge[j].size(); k++ ) {
                if ( fabs(m_zone[i].min(j) - m_edge[j][k]) < 1.0e-6 ) {
                    m_zone[i].adjustMin(j,m_edge[j][k]);
                }
                if ( fabs(m_zone[i].max(j) - m_edge[j][k]) < 1.0e-6 ) {
                    m_zone[i].adjustMax(j,m_edge[j][k]);
                }
            }
        }
    }
    // build LUT for edge finding
    for ( int j = 0; j < 3; j++ ) { // z, r, phi
        // find the size of the smallest interval
        double width = m_edge[j].back() - m_edge[j].front();
        double q(width);
        for ( unsigned i = 0; i < m_edge[j].size()-1; i++ ) {
            q = min( q, m_edge[j][i+1] - m_edge[j][i] );
        }
        // find the number of cells in the LUT
        int n = int(width/q) + 1;
        q = width/(n+0.5);
        _invq[j] = 1.0/q;
        n++;
        // fill the LUT
        int m = 0;
        for ( int i = 0; i < n; i++ ) { // index of LUT
            if ( q*i+m_edge[j].front() > m_edge[j][m+1] ) m++;
            m_edgeLUT[j].push_back(m);
        }
    }
    // build LUT for zone finding
    int nz = m_edge[0].size() - 1;
    int nr = m_edge[1].size() - 1;
    int nphi = m_edge[2].size() - 1;
    for ( int iz = 0; iz < nz; iz++ ) {
        double z = 0.5*(m_edge[0][iz]+m_edge[0][iz+1]);
        for ( int ir = 0; ir < nr; ir++ ) {
            double r = 0.5*(m_edge[1][ir]+m_edge[1][ir+1]);
            for ( int iphi = 0; iphi < nphi; iphi++ ) {
                double phi = 0.5*(m_edge[2][iphi]+m_edge[2][iphi+1]);
                const BFieldZone* zone = findZoneSlow( z, r, phi );
                m_zoneLUT.push_back( zone );
            }
        }
    }
    // build LUT in each zone
    for ( unsigned i = 0; i < m_zone.size(); i++ ) {
        m_zone[i].buildLUT();
    }
}

