//
// BFieldZone.cxx
//
#include "BFieldZone.h"
using namespace std;

//
// Compute magnetic field due to the conductors
//
void
BFieldZone::addBiotSavart( const double *xyz, double *B, double *deriv ) const
{
    for ( unsigned i = 0; i < m_cond.size(); i++ ) {
        m_cond[i].addBiotSavart( xyz, B, deriv );
    }
}

