#ifndef PROPERTYDEVICE_H
#define PROPERTYDEVICE_H

class APropertyDevice;

#include "AProperty.h"

class propertiesDock;

template <typename T> T device_cast( APropertyDevice * );

class ALTER_EXPORT APropertyDevice : public QObject
    {
    Q_OBJECT
public:
    APropertyDevice();
    virtual ~APropertyDevice();
    void addProperty( QString, AVariant, int=propertyInfo::String, QString="" );
    void removeProperty( QString );

    const AProperty &property( QString ) const;
    AProperty &property( QString );

    bool propertyExists( QString ) const;
    unsigned int numberOfProperties() const;
    QString nameFromDisplayName( QString ) const;

    typedef QHash <QString, AProperty *> propertyMap;
    void properties( const propertyMap & );
    const propertyMap &properties() const;
    void clearProperties();

    /** Cast this device safely and quickly to another type
      */
    template <typename T> T castTo()
        {
        // we can use a device cast here
        return device_cast <T> ( this );
        }

Q_SIGNALS:
    void propertiesChanged();
    void entityChanged( APropertyDevice *, AProperty * );
    /** \cond INTERNAL */
    void onCommand( APrivateCommand ** );
    /** \endcond */

protected slots:
    virtual void prePropertyChanged( AProperty * );

private:
    propertyMap _properties;
    propertiesDock *_active;

    /** \cond INTERNAL */
    friend class APrivateDeviceCommand;
    friend class APrivateArrayCommand;
    /** \endcond */
    };

/** Cast a device \p in to a higher type
  \note Returns 0 if cast isn't possible
  */
template <typename T> T device_cast( APropertyDevice *in )
    {
    // use a qobject_cast!
    return qobject_cast <T> (in);
    }

#endif
