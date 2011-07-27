#ifndef PROPERTYWIDGETFACTORY_H
#define PROPERTYWIDGETFACTORY_H

#include "../SDK/AVariant"

/** \cond INTERNAL
  */

class AProperty;
class QWidget;

typedef QWidget *(*guiPropertyTypeFunction)( AProperty * );
typedef bool (*guiPropertyValidatorFunction)( AVariant & );

class APrivatePropertyWidgetFactory
    {
public:
    QWidget *widgetFor( AProperty *property );
    QWidget *widgetFor( AProperty *property, int forceType );

    static void registerType( int, guiPropertyTypeFunction, guiPropertyValidatorFunction=0 );
    static void unregisterType( int );
    static bool tryCustomValidator( int, AVariant & );
    };

/** \endcond
  */

#endif // PROPERTYWIDGETFACTORY_H
