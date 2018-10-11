//
// BFieldMesh.h
//
// Generic 3-d mesh representing a simple field map.
// The field type is templated - it may be short (for the toroid) or double (for the solenoid)
//
// Masahiro Morii, Harvard University
//
#ifndef BFIELDMESH_H
#define BFIELDMESH_H

#include <vector>
#include <cmath>
#include <iostream>
#include "BFieldVector.h"
#include "BFieldCache.h"

template <class T>
class BFieldMesh {
public:
    // constructor
    BFieldMesh() : m_scale(1.0) {;}
    BFieldMesh( double zmin, double zmax, double rmin, double rmax, double phimin, double phimax,
                double bscale )
        : m_scale(bscale)
        { m_min[0] = zmin; m_max[0] = zmax; m_min[1] = rmin; m_max[1] = rmax; m_min[2] = phimin; m_max[2] = phimax; }
    // set ranges
    void setRange( double zmin, double zmax, double rmin, double rmax, double phimin, double phimax )
        { m_min[0] = zmin; m_max[0] = zmax; m_min[1] = rmin; m_max[1] = rmax; m_min[2] = phimin; m_max[2] = phimax; }
    // set bscale
    void setBscale( double bscale ) { m_scale = bscale; }
    // allocate space to vectors
    void reserve( int nz, int nr, int nphi );
    // add elements to vectors
    void appendMesh( int i, double mesh ) { m_mesh[i].push_back(mesh); }
    void appendField( const BFieldVector<T> & field ) { m_field.push_back(field); }
    // build LUT
    void buildLUT();
    // adjust the min/max edges to a new value
    void adjustMin( int i, double x ) { m_min[i] = x; m_mesh[i].front() = x; }
    void adjustMax( int i, double x ) { m_max[i] = x; m_mesh[i].back() = x; }
    // test if a point is inside this zone
    bool inside( double z, double r, double phi ) const;
    // find the bin
    void getCache( double z, double r, double phi, BFieldCache & cache ) const;
    // get the B field
    void getB( double z, double r, double phi, double *B ) const
    { BFieldCache cache; getCache( z, r, phi, cache ); cache.getB( z, r, phi, B ); }
    // accessors
    double min( int i ) const { return m_min[i]; }
    double max( int i ) const { return m_max[i]; }
    double zmin() const { return m_min[0]; }
    double zmax() const { return m_max[0]; }
    double rmin() const { return m_min[1]; }
    double rmax() const { return m_max[1]; }
    double phimin() const { return m_min[2]; }
    double phimax() const { return m_max[2]; }
    unsigned nmesh( int i ) const { return m_mesh[i].size(); }
    double mesh( int i, int j ) const { return m_mesh[i][j]; }
    unsigned nfield() const { return m_field.size(); }
    const BFieldVector<T> & field( int i ) const { return m_field[i]; }
    double bscale() const { return m_scale; }
private:
    double m_min[3], m_max[3];
    std::vector<double> m_mesh[3];
    std::vector< BFieldVector<T> > m_field;
    double m_scale;
    // look-up table and related variables
    std::vector<int> m_LUT[3];
    double m_invUnit[3];     // inverse unit size in the LUT
    int m_roff, m_zoff;
};

//
// Implemnetation follows
//

//
// Reserve space in the vectors to avoid unnecessary memory re-allocations.
//
template <class T>
void BFieldMesh<T>::reserve( int nz, int nr, int nphi )
{
    m_mesh[0].reserve( nz );
    m_mesh[1].reserve( nr );
    m_mesh[2].reserve( nphi );
    m_field.reserve( nz*nr*nphi );
}

//
// Test if a point (z,r,phi) is inside this mesh region.
//
template <class T>
bool BFieldMesh<T>::inside( double z, double r, double phi ) const
{
    // assume phi is in [-pi,pi].
    // phimax() is in [0,2pi], but phimin() may be < 0 if the range crosses phi = 0.
    // we have to test twice to get all possible cases.
    if ( phi < phimin() ) phi += 2.0*M_PI;
    return ( phi >= phimin() && phi <= phimax() &&
             z >= zmin() && z <= zmax() && r >= rmin() && r <= rmax() );
}

//
// Find and return the cache of the bin containing (z,r,phi)
//
template <class T>
void BFieldMesh<T>::getCache( double z, double r, double phi, BFieldCache & cache ) const
{
    // make sure phi is inside this zone
    if ( phi < phimin() ) phi += 2.0*M_PI;
    // find the mesh, and relative location in the mesh
    // z
    const std::vector<double>& mz(m_mesh[0]);
    int iz = int((z-zmin())*m_invUnit[0]); // index to LUT
    iz = m_LUT[0][iz]; // tentative mesh index from LUT
    if ( z > mz[iz+1] ) iz++;
    // r
    const std::vector<double>& mr(m_mesh[1]);
    int ir = int((r-rmin())*m_invUnit[1]); // index to LUT
    ir = m_LUT[1][ir]; // tentative mesh index from LUT
    if ( r > mr[ir+1] ) ir++;
    // phi
    const std::vector<double>& mphi(m_mesh[2]);
    int iphi = int((phi-phimin())*m_invUnit[2]); // index to LUT
    iphi = m_LUT[2][iphi]; // tentative mesh index from LUT
    if ( phi > mphi[iphi+1] ) iphi++;
    // store the bin edges
    cache.setRange( mz[iz], mz[iz+1], mr[ir], mr[ir+1], mphi[iphi], mphi[iphi+1] );
    // store the B field at the 8 corners
    int im0 = iz*m_zoff+ir*m_roff+iphi; // index of the first corner
    cache.setField( 0, m_field[im0              ] );
    cache.setField( 1, m_field[im0            +1] );
    cache.setField( 2, m_field[im0      +m_roff  ] );
    cache.setField( 3, m_field[im0      +m_roff+1] );
    cache.setField( 4, m_field[im0+m_zoff        ] );
    cache.setField( 5, m_field[im0+m_zoff      +1] );
    cache.setField( 6, m_field[im0+m_zoff+m_roff  ] );
    cache.setField( 7, m_field[im0+m_zoff+m_roff+1] );
    // store the B scale
    cache.setBscale( m_scale );
    return;
}

//
// Construct the look-up table to accelerate bin-finding.
//
template <class T>
void BFieldMesh<T>::buildLUT()
{
    for ( int j = 0; j < 3; j++ ) { // z, r, phi
        // align the m_mesh edges to m_min/max
        m_mesh[j].front() = m_min[j];
        m_mesh[j].back() = m_max[j];
        // determine the unit size, q, to be used in the LUTs
        double width = m_mesh[j].back() - m_mesh[j].front();
        double q(width);
        for ( unsigned i = 0; i < m_mesh[j].size()-1; i++ ) {
            q = std::min( q, m_mesh[j][i+1] - m_mesh[j][i] );
        }
        // find the number of units in the LUT
        int n = int(width/q) + 1;
        q = width/(n+0.5);
        m_invUnit[j] = 1.0/q; // new unit size
        n++;
        int m = 0; // mesh number
        for ( int i = 0; i < n; i++ ) { // LUT index
            if ( i*q + m_mesh[j].front() > m_mesh[j][m+1] ) m++;
            m_LUT[j].push_back(m);
        }
    }
    m_roff = m_mesh[2].size();       // index offset for incrementing r by 1
    m_zoff = m_roff*m_mesh[1].size(); // index offset for incrementing z by 1
}

#endif
