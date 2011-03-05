#ifndef COLOURPICKERWIDGET_H
#define COLOURPICKERWIDGET_H

#include "QWidget"

#include "AProperty"

namespace calcPropertyInfo
    {
    enum PropertyType
        {
        Colour = propertyInfo::UserType + 5,
        ColourNoAlpha = propertyInfo::UserType + 6
        };
    }

class QToolButton;
class AProperty;

class colourPickerWidget : public QWidget
    {
public:
    colourPickerWidget( AProperty * );
    ~colourPickerWidget( );

    void colourPicked( XColour, bool );

    static QWidget *create( AProperty * );

private:
    QToolButton *_button;
    AProperty *_property;
    };

#endif // COLOURPICKERWIDGET_H
