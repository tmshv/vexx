#ifndef JLIB_PROPERTIES_H
#define JLIB_PROPERTIES_H

#define _JLIB_FN_GET( type, name, member )      \
    inline type name ( ) const                  \
        { return member; }

#define _JLIB_FN_SET( type, name, member )      \
    inline void name ( type member##in )        \
        { member = member##in; }

#define _JLIB_FN_REF( type, name, member )      \
    inline type & name ( )                      \
        { return member; }

#define _JLIB_FN_CONST_REF( type, name, member ) \
    inline const type & name ( ) const          \
        { return member; }

#define _JLIB_FN_PTR_GET( type, name, member )  \
    inline type &name ( ) const                 \
        { return *member; }

#define _JLIB_FN_PTR_SET( type, name, member )  \
    inline void name ( type &member##in )       \
        { member = &member##in; }

#define _JLIB_FN_GET_PROPERTY( type, name ) _JLIB_FN_GET( type, name, _##name )
#define _JLIB_FN_SET_PROPERTY( type, name ) _JLIB_FN_SET( type, name, _##name )

#define _JLIB_FN_REF_PROPERTY( type, name ) _JLIB_FN_REF( type, name, _##name )
#define _JLIB_FN_CONST_REF_PROPERTY( type, name ) _JLIB_FN_CONST_REF( type, name, _##name )


#define _JLIB_FN_GET_PTR_PROPERTY( type, name ) _JLIB_FN_PTR_GET( type, name, _##name )
#define _JLIB_FN_SET_PTR_PROPERTY( type, name ) _JLIB_FN_PTR_SET( type, name, _##name )

#define _JLIB_MB_PROPERTY( type, name ) type _##name;


#define jMBProperty( type, name ) _JLIB_MB_PROPERTY( type, name )


#define jSpecificROProperty( type, name, memberMode, fnMode )          \
    memberMode:                                 \
    _JLIB_MB_PROPERTY( type, name )             \
    fnMode:                                     \
    _JLIB_FN_GET_PROPERTY( type, name )

#define jROProperty( type, name ) jSpecificROProperty( type, name, private, public )


#define jSpecificWOProperty( type, name, memberMode, fnMode )          \
    memberMode:                                 \
    _JLIB_MB_PROPERTY( type, name )             \
    fnMode:                                     \
    _JLIB_FN_SET_PROPERTY( type, name )

#define jWOProperty( type, name ) jSpecificWOProperty( type, name, private, public )


#define jSpecificProperty( type, name, memberMode, fnMode )         \
    memberMode:                                 \
    _JLIB_MB_PROPERTY( type, name )             \
    fnMode:                                     \
    _JLIB_FN_GET_PROPERTY( type, name )         \
    _JLIB_FN_SET_PROPERTY( type, name )

#define jSpecificPtrProperty( type, name, memberMode, fnMode )         \
    memberMode:                                 \
    _JLIB_MB_PROPERTY( type *, name )           \
    fnMode:                                     \
    _JLIB_FN_GET_PTR_PROPERTY( type, name )     \
    _JLIB_FN_SET_PTR_PROPERTY( type, name )

#define jSpecificRefProperty( type, name, memberMode, fnMode )         \
    memberMode:                                 \
    _JLIB_MB_PROPERTY( type, name )             \
    fnMode:                                     \
    _JLIB_FN_REF_PROPERTY( type, name )         \
    _JLIB_FN_CONST_REF_PROPERTY( type, name )   \

#define jSpecificRORefProperty( type, name, memberMode, fnMode )         \
    memberMode:                                 \
    _JLIB_MB_PROPERTY( type, name )             \
    fnMode:                                     \
    _JLIB_FN_CONST_REF_PROPERTY( type, name )   \

#define jProperty( type, name ) jSpecificProperty( type, name, private, public )

#define jPtrProperty( type, name ) jSpecificPtrProperty( type, name, private, public )

#define jRefProperty( type, name ) jSpecificRefProperty( type, name, private, public )

#define jRORefProperty( type, name ) jSpecificRORefProperty( type, name, private, public )

#endif
