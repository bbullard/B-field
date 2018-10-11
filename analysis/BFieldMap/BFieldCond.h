//
// BFieldCond.h
//
// Current conductors used in BFieldZone.
// It may be finite : p1 to p2.
// It may be infinite : passing p1 and direction parallel to p2.
//
// Masahiro Morii, Harvard University
//
#ifndef BFIELDCOND_H
#define BFIELDCOND_H

class BFieldCond {
public:
    // constructor
    BFieldCond( bool finite, const double *p1, const double *p2, double curr );
    // compute magnetic field, plus derivatives if requested, and add
    void addBiotSavart( const double *xyz, double *B, double *deriv=0 ) const;
    // accessors
    bool finite() const { return m_finite; }
    double p1( int i ) const { return m_p1[i]; }
    double p2( int i ) const { return m_p2[i]; }
    double curr() const { return m_curr; }
private:
    bool m_finite;  // true if the conductor is finite in length
    double m_p1[3]; // one end of a finite conductor, or one point on an infinite conductor
    double m_p2[3]; // the other end of a finite conductor, or the direction vector of an infinite conductor
    double m_u[3];  // direction vector (= m_p2 if infinite)
    double m_curr;  // current (in A) flowing through the conductor
};

#endif
