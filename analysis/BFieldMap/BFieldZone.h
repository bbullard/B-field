//
// BFieldZone.h
//
// A "zone" inside the toroid field map
//
// Masahiro Morii, Harvard University
//
#ifndef BFIELDZONE_H
#define BFIELDZONE_H

#include <vector>
#include "BFieldMesh.h"
#include "BFieldCond.h"

class BFieldZone : public BFieldMesh<short> {
public:
    // constructor
    BFieldZone( int id, double zmin, double zmax, double rmin, double rmax, double phimin, double phimax,
                double scale )
        : BFieldMesh<short>(zmin,zmax,rmin,rmax,phimin,phimax,scale), m_id(id) {;}
    // add elements to vectors
    void appendCond( const BFieldCond& cond ) { m_cond.push_back(cond); }
    // compute Biot-Savart magnetic field and add to B[3]
    void addBiotSavart( const double *xyz, double *B, double *deriv=0 ) const;
    // accessors
    int id() const { return m_id; }
    unsigned ncond() const { return m_cond.size(); }
    const BFieldCond& cond(int i) const { return m_cond[i]; }
private:
    int m_id;          // zone ID number
    std::vector<BFieldCond> m_cond;            // list of current conductors
};

#endif
