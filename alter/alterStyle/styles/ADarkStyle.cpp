#include "ADarkStyle.h"
#include "QPainter"
#include "QStyleOption"
#include "QApplication"
#include "QDebug"
#include "XColour"

ADarkStyle::ADarkStyle()
        : menuLoop(":/images/menuLoop.png"),
        radioDown(":/images/radioDown.png"),
        radioUp(":/images/radioUp.png"),
        radioOver(":/images/radioOver.png"),
        checkUp(":/images/checkboxUp.png"),
        checkDown(":/images/checkboxDown.png"),
        checkOver(":/images/checkboxOver.png"),
        checkTri(":/images/checkboxChecked.png")
    {
    }

void ADarkStyle::polish( QApplication *app )
    {
    QFont fnt( app->font( ) );
    fnt.setPixelSize( 11 );
    app->setFont( fnt );
    }

void ADarkStyle::polish(QPalette &palette)
    {
    QCleanlooksStyle::polish( palette );

    QColor highlight( 11, 6, 122 );

    QColor base( 85, 85, 85 );
    QColor window( base.darker( 160 ) );
    QColor highlightText( highlight.darker( 300 ) );
    QColor disabledText( base.lighter( 200 ) );
    QColor text( base.lighter( 350 ) );
    QColor button( base.darker( 200 ) );


    palette.setBrush( QPalette::Window, window );
    palette.setBrush( QPalette::Base, window );
    palette.setBrush( QPalette::AlternateBase, base.darker( 20 ) );
    palette.setBrush( QPalette::Highlight, highlight );
    palette.setBrush( QPalette::HighlightedText, highlightText );

    palette.setBrush( QPalette::WindowText, text );
    palette.setBrush( QPalette::Disabled, QPalette::WindowText, disabledText );

    palette.setBrush( QPalette::Text, text );
    palette.setBrush( QPalette::Disabled, QPalette::Text, disabledText );

    palette.setBrush( QPalette::ButtonText, text );
    palette.setBrush( QPalette::Disabled, QPalette::ButtonText, disabledText );

    palette.setBrush( QPalette::BrightText, highlightText );

    palette.setBrush( QPalette::Light, button.lighter( 160 ) );
    palette.setBrush( QPalette::Midlight, button.lighter( 130 ) );
    palette.setBrush( QPalette::Button, button );
    palette.setBrush( QPalette::Mid, button.darker( 120 ) );
    palette.setBrush( QPalette::Dark, button.darker( 140 ) );
    palette.setBrush( QPalette::Shadow, button.darker( 190 ) );
    }

void ADarkStyle::drawControl( ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget ) const
    {
    switch( element )
        {
        case CE_MenuBarItem:
        case CE_PushButtonBevel:
            painter->fillRect( option->rect, Qt::red );
            break;

        case CE_MenuBarEmptyArea:
            painter->save();
            painter->setRenderHint( QPainter::SmoothPixmapTransform, false );
            painter->drawPixmap( option->rect, menuLoop );
            painter->restore();
            break;
        default:
            QCleanlooksStyle::drawControl( element, option, painter, widget );
        }
    }

void ADarkStyle::drawComplexControl( ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget ) const
    {
    switch( control )
        {
        case CC_SpinBox:
        case CC_Slider:
        case CC_Dial:
        case CC_ComboBox:
        case CC_ScrollBar:
            painter->fillRect( option->rect, Qt::green );
            break;
        default:
            QCleanlooksStyle::drawComplexControl( control, option, painter, widget );
        }
    }

void ADarkStyle::drawPrimitive( PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget ) const
    {
    switch( element )
        {
        case PE_IndicatorRadioButton:
            painter->save();
            painter->setRenderHint( QPainter::SmoothPixmapTransform, false );
            if( (option->state&State_On) != 0 )
                {
                painter->drawPixmap( option->rect, radioDown );
                }
            else if( !widget || !widget->hasFocus() )
                {
                painter->drawPixmap( option->rect, radioUp );
                }
            else
                {
                painter->drawPixmap( option->rect, radioOver );
                }
            painter->restore();
            break;
        case PE_IndicatorCheckBox:
            painter->save();
            painter->setRenderHint( QPainter::SmoothPixmapTransform, false );
            if( (option->state&State_On) != 0 )
                {
                painter->drawPixmap( option->rect, checkDown );
                }
            if( (option->state&State_NoChange) != 0 )
                {
                painter->drawPixmap( option->rect, checkTri );
                }
            else if( !widget || !widget->hasFocus() )
                {
                painter->drawPixmap( option->rect, checkOver );
                }
            else
                {
                painter->drawPixmap( option->rect, checkUp );
                }
            painter->restore();
            break;
        case PE_FrameLineEdit:
            {
            painter->save();
            QRect rct( option->rect );
            painter->setPen( Qt::transparent );
            painter->setBrush( option->palette.color( QPalette::Base ) );
            painter->drawRect( rct );

            painter->setBrush( Qt::transparent );
            painter->setPen( option->palette.color( QPalette::Dark ) );

            if( widget && widget->hasFocus() )
                {
                painter->setPen( option->palette.color( QPalette::Highlight ).darker( 150 ) );
                }

            painter->drawLine( option->rect.topLeft(), option->rect.bottomLeft() );
            painter->drawLine( option->rect.topLeft(), option->rect.topRight() );

            if( widget && widget->hasFocus() )
                {
                painter->setPen( option->palette.color( QPalette::Dark ) );
                }
            else
                {
                painter->setPen( option->palette.color( QPalette::Mid ) );
                }

            painter->drawLine( option->rect.bottomRight(), option->rect.topRight() );
            painter->drawLine( option->rect.bottomRight(), option->rect.bottomLeft() );

            painter->restore();
            break;
            }
        case PE_FrameGroupBox:
            {
            painter->save();
            QRect rct( option->rect );
            rct.adjust( 1, 1, -2, -2 );
            painter->setRenderHint( QPainter::Antialiasing, true );
            painter->setPen( option->palette.color( QPalette::Dark ) );
            painter->drawRoundedRect( rct, 5, 5 );
            painter->restore();
            break;
            }
        default:
            QCleanlooksStyle::drawPrimitive(element, option, painter, widget);
        }
    }

QSize ADarkStyle::sizeFromContents( ContentsType type, const QStyleOption *option, const QSize &contentsSize, const QWidget *widget ) const
    {
    QSize ret( QCleanlooksStyle::sizeFromContents( type, option, contentsSize, widget ) );
    switch( type )
        {
        case CT_RadioButton:
            ret = QSize( 12, 12 );
            break;
        case CT_CheckBox:
            ret = QSize( 13, 13 );
            break;
        case CT_PushButton:
            ret.setHeight( ret.height() - 5 );
            break;
        case CT_ProgressBar:
        case CT_ComboBox:
        case CT_LineEdit:
            ret.setHeight( ret.height() - 5 );
            break;
        default: break;
        }

    return ret;
    }

int ADarkStyle::pixelMetric( PixelMetric metric, const QStyleOption *option, const QWidget *widget ) const
    {
    return QCleanlooksStyle::pixelMetric( metric, option, widget );
    }
