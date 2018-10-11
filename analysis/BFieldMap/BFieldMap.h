//
// BFieldMap.h
//
// Magnetic field map for the ATLAS detector
// The map can be read from an ASCII text file (a la John Hart)
// or from a ROOT file.
//
// Masahiro Morii, Harvard University
//
#ifndef BFIELDMAP_H
#define BFIELDMAP_H

#include <vector>
#include <iostream>
#include "TFile.h"
#include "BFieldZone.h"

class BFieldMap {
public:
    // constructor
    BFieldMap() : m_lastzone(0) {;}
    // compute magnetic field
    void getB( const double *xyz, double *B, double *deriv=0 ) const;
    // read/write map from/to file
    int readMap( const char* filename );
    int readMap( std::istream& input );
    int readMap( TFile* rootfile );
    void writeMap( TFile* rootfile );
    // append a zone
    void appendZone( BFieldZone zone ) { m_zone.push_back( zone ); }
    // access zones
    int nzone() const { return m_zone.size(); }
    const BFieldZone& zone( int i ) const { return m_zone[i]; }
private:
    // data members
    std::vector<BFieldZone> m_zone;
    // data members used in zone-finding
    std::vector<double> m_edge[3]; // zone boundaries in z, r, phi
    std::vector<int> m_edgeLUT[3]; // look-up table for zone edges
    double _invq[3]; // 1/stepsize in m_edgeLUT
    std::vector<const BFieldZone*> m_zoneLUT; // look-up table for zones
    // cache for speed
    mutable BFieldCache m_cache;
    mutable const BFieldZone* m_lastzone;
    // utility functions
    int read_packed_data( std::istream& input, std::vector<int>& data );
    int read_packed_int( std::istream& input, int &n );
    void buildLUT(); // called from map-reading functions
    const BFieldZone* findZone( double z, double r, double phi ) const;
    const BFieldZone* findZoneSlow( double z, double r, double phi ) const;
};

#endif
