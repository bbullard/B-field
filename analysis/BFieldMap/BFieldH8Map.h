//
// BFieldH8Map.h
//
// Magnetic field map of the H8 beam test
//
// Masahiro Morii, Harvard University
//
#ifndef BFIELDH8MAP_H
#define BFIELDH8MAP_H

#include <vector>
#include <iostream>

#include "BFieldH8Grid.h"

class BFieldH8Map {
public:
    BFieldH8Map() {;}
    void readMap( std::istream& input );
    void getB( const double *xyz, double *B, double *deriv=0 ) const;
private:
    std::vector<BFieldH8Grid> m_grid;
};

#endif
