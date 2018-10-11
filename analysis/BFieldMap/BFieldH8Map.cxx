//
// BFieldH8Map.cxx
//
// Masahiro Morii, Harvard University
//
#include "BFieldH8Map.h"

void
BFieldH8Map::readMap( std::istream& input )
{
    // skip the file header line
    char line[256];
    input.getline( line, 256 );
    // read grids
    while (1) {
        BFieldH8Grid grid;
        grid.readMap( input );
        if ( grid.defined() ) {
            m_grid.push_back( grid );
        } else {
            return;
        }
    }
}

void
BFieldH8Map::getB( const double *xyz, double *B, double *deriv ) const
{
    for ( unsigned i = 0; i < m_grid.size(); i++ ) {
        // find the grid that contains xyz
        if ( m_grid[i].inside( xyz ) ) {
            m_grid[i].getB( xyz, B, deriv );
            return;
        }
    }
    // xyz is outside all grids
    B[0] = B[1] = B[2] = 0.0;
    if ( deriv != 0 ) {
        for ( int j = 0; j < 9; j++ ) deriv[j] = 0.0;
    }
    return;
}

