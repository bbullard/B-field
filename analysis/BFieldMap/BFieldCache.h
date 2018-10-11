//
// BFieldCache.h
//
// Cashe of one bin of the magnetic field map.
// Defined by ranges in z, r, phi, and the B vectors at the 8 corners of the "bin".
//
// Masahiro Morii, Harvard University
//
#ifndef BFIELDCACHE_H
#define BFIELDCACHE_H

#include <cmath>
#include "BFieldVector.h"

class BFieldCache {
public:
    // default constructor sets unphysical boundaries, so that inside() will fail
    BFieldCache() : m_phimin(0.0), m_phimax(-1.0) {;}
    // set the z, r, phi range that defines the bin
    void setRange( double zmin, double zmax, double rmin, double rmax, double phimin, double phimax )
    { m_zmin = zmin; m_zmax = zmax; m_rmin = rmin; m_rmax = rmax; m_phimin = phimin; m_phimax = phimax; }
    // set the field values at each corner
    void setField( int i, BFieldVector<double> field ) { m_field[i] = field; }
    void setField( int i, BFieldVector<short> field ) { m_field[i].set( field.z(), field.r(), field.phi() ); }
    // set the multiplicative factor for the field vectors
    void setBscale( double bscale ) { m_scale = bscale; }
    // test if (z, r, phi) is inside this bin
    bool inside( double z, double r, double phi ) const
    { if ( phi < m_phimin ) phi += 2.0*M_PI;
      return ( phi >= m_phimin && phi <= m_phimax && z >= m_zmin && z <= m_zmax && r >= m_rmin && r <= m_rmax ); }
    // interpolate the field and return B[3].
    // also compute field derivatives if deriv[9] is given.
    void getB( double z, double r, double phi, double *B, double *derive=0 ) const;
private:
    double m_zmin, m_zmax;
    double m_rmin, m_rmax;
    double m_phimin, m_phimax;
    BFieldVector<double> m_field[8];
    double m_scale;
};

#endif
