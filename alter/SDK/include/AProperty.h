#ifndef PROPERTYDATA_H
#define PROPERTYDATA_H

#include "AGlobal.h"
#include "AVariant.h"

class AProperty;
class APropertyDevice;
class AAttribute;
class AConstAttribute;
class APrivateCommand;

namespace propertyInfo
    {
    enum propertyType
        {
        Bool,
        Int,
        Double,
        Float,
        String,
        File,
        Time,
        Position,
        Direction,
        Vector,
        Scale,
        Orientation,
        Colour,
        ColourNoAlpha,
        LongString,
        Matrix3x3,
        FileSequence,
        Component2D,
        Component3D,
        Component4D,
        Directory,
        UserType = 1000
        };
    };

typedef bool (*validatorFn)( AVariant & );

class ALTER_EXPORT AProperty : public QObject
    {
    Q_OBJECT
public:
    typedef QHash <QString, AVariant> attributeMap;

    AProperty( APropertyDevice *, QString, const AVariant &, int, QString );
    AProperty &operator =( const AVariant & );
    operator AVariant() const;
    const AVariant *operator ->( ) const;

    const AVariant &value() const;

    int type() const;
    void setType( int );

    QString name() const;

    bool hidden() const;
    void hide( bool able = TRUE );

    QString displayName() const;
    void displayName( const QString & );

    const attributeMap &attributes() const;
    AAttribute attribute( QString );
    AConstAttribute attribute( QString ) const;
    void removeAttribute( QString );

    APropertyDevice *parent( );

Q_SIGNALS:
    /** \cond INTERNAL */
    void onCommand( APrivateCommand ** );
    /** \endcond */
    void onPropertyChange( AProperty * );

private:
    Q_DISABLE_COPY( AProperty );
    AVariant _value;
    void changeAttribute( QString, const AVariant & );
    static int _nextID;
    int _thisID;
    APropertyDevice *_device;
    QString _name;
    bool _informing;
    attributeMap _attributes;

    /** \cond INTERNAL */
    friend class APrivatePropertyCommand;
    friend class APrivateDeviceCommand;
    friend class APrivateAttributeCommand;
    friend class AAttribute;
    friend class AConstAttribute;
    /** \endcond */
    };

class AAttribute
    {
public:
    AAttribute( AProperty *parent, QString name ) : _parent( parent ), _name( name ) { }
    AAttribute &operator=( const AVariant &in )
        {
        _parent->changeAttribute( _name, in );
        return *this;
        }

    operator AVariant()
        {
        return _parent->_attributes[_name];
        }

    const AVariant *operator->()
        {
        return &(_parent->_attributes[_name]);
        }

private:
    AProperty *_parent;
    QString _name;
    };

class AConstAttribute
    {
public:
    AConstAttribute( const AProperty *parent, QString name ) : _parent( parent ), _name( name ) { }

    operator AVariant()
        {
        AProperty::attributeMap::const_iterator it = _parent->_attributes.find( _name );
        if( it != _parent->_attributes.end() )
            {
            return it.value();
            }
        return AVariant();
        }

    const AVariant *operator->()
        {
        AProperty::attributeMap::const_iterator it = _parent->_attributes.find( _name );
        if( it != _parent->_attributes.end() )
            {
            return &(it.value());
            }
        return 0;
        }

private:
    const AProperty *_parent;
    QString _name;
    };

#endif
