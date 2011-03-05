#ifndef JLIB_VARIANT_H
#define JLIB_VARIANT_H

#ifdef QT_CORE_LIB
    #include "QVariant"
#endif
#include "jLib.global.h"
#include "math/math.h"

BEGIN_JLIB_NAMESPACE

#define JLIB_VARIANT_CONSTRUCTOR( name, type )              \
inline variant( type in ) : _type( Null )                   \
    {                                                       \
    from##name( in );                                       \
    }                                                       \
void from##name( type in );                                 \
type to##name( ) const;

#define JLIB_VARIANT_REFERENCE_CONSTRUCTOR( name, type )    \
inline variant( const type &in ) : _type( Null )            \
    {                                                       \
    from##name( in );                                       \
    }                                                       \
void from##name( const type &in );                          \
type to##name( ) const;

class JLIB_EXPORT variant
    {
public:
    enum variantType { Null,
                       String,
                       Bool,
                       Int,
                       UnsignedInt,
                       Char,
                       Double,
                       VectorX,
                       Triple=VectorX,
                       Colour=VectorX,
                       Quaternion=VectorX,
                       List,
                       QtVariant,
                       QtString = String };
    jProperty( variantType, type )
public:
    variant();
    variant( const variant & );
    ~variant();
    bool isNull() const;
    void clear();
    variant &operator =( const variant & );
    void fromVariant( const variant & );

    JLIB_VARIANT_CONSTRUCTOR( Bool, bool )
    JLIB_VARIANT_CONSTRUCTOR( Int, int )
    JLIB_VARIANT_CONSTRUCTOR( UnsignedInt, unsigned int )
    JLIB_VARIANT_CONSTRUCTOR( Char, char )
    JLIB_VARIANT_CONSTRUCTOR( Double, double )
    JLIB_VARIANT_CONSTRUCTOR( CharString, const char * )
    JLIB_VARIANT_REFERENCE_CONSTRUCTOR( String, string )
    JLIB_VARIANT_REFERENCE_CONSTRUCTOR( Triple, math::triple )
    JLIB_VARIANT_REFERENCE_CONSTRUCTOR( Colour, math::colour4 )
    JLIB_VARIANT_REFERENCE_CONSTRUCTOR( Quaternion, math::quaternion )
    JLIB_VARIANT_REFERENCE_CONSTRUCTOR( VectorX, math::vectorX )
    JLIB_VARIANT_REFERENCE_CONSTRUCTOR( List, list <variant> )

#ifdef QT_CORE_LIB
    JLIB_VARIANT_REFERENCE_CONSTRUCTOR( QtVariant, QVariant )
    JLIB_VARIANT_REFERENCE_CONSTRUCTOR( QtString, QString )
#endif

private:
    union
        {
        bool _bool;
        int _int;
        int _uint;
        char _char;
        double _double;
        void *_other;
        };
    };

END_JLIB_NAMESPACE

#endif
