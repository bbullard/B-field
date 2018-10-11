//
// BFieldCache.cxx
//
#include "BFieldCache.h"

//
// Interpolate the field to return the B vetor at (z, r, phi)
// Also compute field derivatives dBx/dx etc. if deriv[9] is given.
//
void
BFieldCache::getB( double z, double r, double phi, double *B, double *deriv ) const
{
    // make sure phi is inside [m_phimin,m_phimax]
    if ( phi < m_phimin ) phi += 2*M_PI;
    // fractional position inside this mesh
    double fz = (z-m_zmin) / (m_zmax-m_zmin);
    double gz = 1.0 - fz;
    double fr = (r-m_rmin) / (m_rmax-m_rmin);
    double gr = 1.0 - fr;
    double fphi = (phi-m_phimin) / (m_phimax-m_phimin);
    double gphi = 1.0 - fphi;
    // interpolate field values in z, r, phi
    double Bzrphi[3];
    for ( int i = 0; i < 3; i++ ) { // z, r, phi
        Bzrphi[i] = m_scale*( gz*( gr*( gphi*m_field[0][i] + fphi*m_field[1][i] ) +
                                   fr*( gphi*m_field[2][i] + fphi*m_field[3][i] ) ) +
                              fz*( gr*( gphi*m_field[4][i] + fphi*m_field[5][i] ) +
                                   fr*( gphi*m_field[6][i] + fphi*m_field[7][i] ) ) );
    }
    // convert (Bz,Br,Bphi) to (Bx,By,Bz)
    double c = cos(phi);
    double s = sin(phi);
    B[0] = Bzrphi[1]*c - Bzrphi[2]*s;
    B[1] = Bzrphi[1]*s + Bzrphi[2]*c;
    B[2] = Bzrphi[0];

    // compute field derivatives if requested
    if ( deriv ) {
        double sz = m_scale/(m_zmax-m_zmin);
        double sr = m_scale/(m_rmax-m_rmin);
        double sphi = m_scale/(m_phimax-m_phimin);
        double dBdz[3], dBdr[3], dBdphi[3];
        for ( int j = 0; j < 3; j++ ) { // Bz, Br, Bphi components
            dBdz[j]   = sz*( gr*( gphi*(m_field[4][j]-m_field[0][j]) +
                                  fphi*(m_field[5][j]-m_field[1][j]) ) +
                             fr*( gphi*(m_field[6][j]-m_field[2][j]) +
                                  fphi*(m_field[7][j]-m_field[3][j]) ) );
            dBdr[j]   = sr*( gz*( gphi*(m_field[2][j]-m_field[0][j]) +
                                  fphi*(m_field[3][j]-m_field[1][j]) ) +
                             fz*( gphi*(m_field[6][j]-m_field[4][j]) +
                                  fphi*(m_field[7][j]-m_field[5][j]) ) );
            dBdphi[j] = sphi*( gz*( gr*(m_field[1][j]-m_field[0][j]) +
                                    fr*(m_field[3][j]-m_field[2][j]) ) +
                               fz*( gr*(m_field[5][j]-m_field[4][j]) +
                                    fr*(m_field[7][j]-m_field[6][j]) ) );
        }
        // convert to cartesian coordinates
        double cc = c*c;
        double cs = c*s;
        double ss = s*s;
        deriv[0] = cc*dBdr[1] - cs*dBdr[2] - cs*dBdphi[1]/r + ss*dBdphi[2]/r + s*B[1]/r;
        deriv[1] = cs*dBdr[1] - ss*dBdr[2] + cc*dBdphi[1]/r - cs*dBdphi[2]/r - c*B[1]/r;
        deriv[2] = c*dBdz[1] - s*dBdz[2];
        deriv[3] = cs*dBdr[1] + cc*dBdr[2] - ss*dBdphi[1]/r - cs*dBdphi[2]/r - s*B[0]/r;
        deriv[4] = ss*dBdr[1] + cs*dBdr[2] + cs*dBdphi[1]/r + cc*dBdphi[2]/r + c*B[0]/r;
        deriv[5] = s*dBdz[1] + c*dBdz[2];
        deriv[6] = c*dBdr[0] - s*dBdphi[0]/r;
        deriv[7] = s*dBdr[1] + c*dBdphi[0]/r;
        deriv[8] = dBdz[0];
    }
}

