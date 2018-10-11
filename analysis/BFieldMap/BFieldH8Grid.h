//
// BFieldH8Grid.h
//
// Masahiro Morii, Harvard University
//
#ifndef BFIELDH8GRID_H
#define BFIELDH8GRID_H

#include <vector>
#include <iostream>

class BFieldH8Grid {
public:
    BFieldH8Grid();
    void readMap( std::istream& input );
    void getB( const double *xyz, double *B, double *deriv=0 ) const;
    bool defined() const { return ( m_n[0] > 0 ); }
    bool inside( const double *xyz ) const
    { return ( xyz[0]>=m_min[0] && xyz[0]<=m_max[0] &&
               xyz[1]>=m_min[1] && xyz[1]<=m_max[1] &&
               xyz[2]>=m_min[2] && xyz[2]<=m_max[2] ); }
    void setOffset( const double *dxyz );
private:
    int    m_n[3];              // number of grid points
    double m_min[3], m_max[3];  // range in x,y,z (mm)
    double m_d[3];              // offset in x,y,z (mm)
    std::vector<double> m_B[3]; // Bz,By,Bz (kT)
};

#endif
