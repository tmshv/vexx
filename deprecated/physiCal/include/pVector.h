#ifndef PVECTOR_H
#define PVECTOR_H

#include "physiCalGlobal.h"

BEGIN_PHYSICAL_NAMESPACE

//! Basic 3D Vector and Point Class
class PHYSICAL_EXPORT pVector
    {
public:
    //! get the x component
    inline void x( pReal in )
        { cmp[0] = in; }
    //! get the y component
    inline void y( pReal in )
        { cmp[1] = in; }
    //! get the z component
    inline void z( pReal in )
        { cmp[2] = in; }
    //! get the x component
    inline pReal& x()
        { return cmp[0]; }
    //! get the y component
    inline pReal& y()
        { return cmp[1]; }
    //! get the z component
    inline pReal& z()
        { return cmp[2]; }
    //! get the x component
    inline const pReal& x() const
        { return cmp[0]; }
    //! get the y component
    inline const pReal& y() const
        { return cmp[1]; }
    //! get the z component
    inline const pReal& z() const
        { return cmp[2]; }

    //Initiators
    //! create a vector with components [com,com,com]
    pVector( pReal com=0.0 );
    //! copy a vector
    pVector( const pVector &in );
    //! create a vector [x,y,z]
    pVector( pReal x, pReal y, pReal z );
    //! create a vector from a jLib triple
    pVector( const jLib::math::triple & );

    //! the size of this vector ( deprecated, always 3 )
    unsigned int size() const { return _size; }

    //! get this vector as a jLib triple
    operator jLib::math::triple() const
        {
        return jLib::math::triple( x(), y(), z() );
        }

    //Vector Operations
    //! cross this vector and vector b
    pVector cross( const pVector &b ) const;
    //! find the dot product of this and b
    pReal dot( const pVector &b ) const;
    //! find the length of this vector
    pReal length() const;
    //! find the length of this vector squared ( faster than length() )
    pReal lengthSquared() const;
    //! find this vector normalised
    pVector normalise() const;
    //! find this vector scaled by in ie newLength = length() * in
    pVector scale( pReal in ) const;
    //! find this vector scaled by in ie newLength = in
    pVector scaleTo( pReal in ) const;
    //! find the negative of this vector
    pVector negate() const;
    //! find the components of this vector squared
    pVector squared() const;
    //! find the components of this vector square rooted
    pVector sqroot() const;

    //Access Methods
    //! find the component x of this vector
    pReal& operator[]( unsigned int x );
    //! find the component x of this vector
    pReal operator[]( unsigned int x ) const;
    //! find the a pointer to the x component ( y and z guaranteed to follow )
    inline pReal *ptr()
        { return cmp; }

    //Equality
    //! assign this vector from vector in
    pVector &operator =( pReal in );
    //! test for component wise equality
    JBOOL operator ==( pReal in ) const;
    //! test for component wise inequality
    JBOOL operator !=( pReal in ) const;
    //! assign this vector from vector in
    pVector &operator =( pVector in );
    //! test for component wise equality
    JBOOL operator ==( pVector in ) const;
    //! test for component wise inequality
    JBOOL operator !=( pVector in ) const;

    //! negate this vector
    pVector operator -() const;

    //Operate by vector on other
    pVector operator +( pVector other ) const;
    pVector operator -( pVector other ) const;
    pVector operator *( pVector other ) const;
    pVector operator /( pVector other ) const;

    //Operate by scalar on other
    pVector operator /( pReal other ) const;
    pVector operator *( pReal other ) const;
    pVector operator +( pReal other ) const;
    pVector operator -( pReal other ) const;

    //Operate by vector on self.
    pVector operator +=( pVector other );
    pVector operator -=( pVector other );
    pVector operator *=( pVector other );
    pVector operator /=( pVector other );

    //Operate by scalar on self.
    pVector operator +=( pReal other );
    pVector operator -=( pReal other );
    pVector operator *=( pReal other );
    pVector operator /=( pReal other );

protected:
    unsigned int _size;
    pReal cmp[4];
    };

// stream this vector
std::ostream &PHYSICAL_EXPORT operator<<( std::ostream &output, const pVector &other );
std::istream &PHYSICAL_EXPORT operator>>( std::istream &input, pVector &osther );

// operations like 5 * vec
pVector PHYSICAL_EXPORT operator/( const pReal &, const pVector & );
pVector PHYSICAL_EXPORT operator*( const pReal &, const pVector & );
pVector PHYSICAL_EXPORT operator+( const pReal &, const pVector & );
pVector PHYSICAL_EXPORT operator-( const pReal &, const pVector & );

END_PHYSICAL_NAMESPACE

#endif // PVECTOR_H
