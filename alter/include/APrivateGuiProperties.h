#ifndef GUIPROPERTIES_H
#define GUIPROPERTIES_H

#include "../SDK/ALog"
#include "../SDK/AProperty"
#include "../SDK/AFileDialog"

#include "XFileSequence"

#include "math.h"
#include "QCheckBox"
#include "QSpinBox"
#include "QToolButton"
#include "QHBoxLayout"
#include "QLineEdit"
#include "XColourWidget"
#include "XVector2DWidget"
#include "XVector3DWidget"
#include "XVector4DWidget"
#include "XFloatWidget"
#include "QSettings"
#include "QTextEdit"
#include "QStackedWidget"

/** \cond INTERNAL
  */

namespace aGP
{
/** \brief APrivateBoolProperty Sets and displays a GUI bool value.
  */
class APrivateBoolProperty : public QCheckBox
    {
    Q_OBJECT
public:
    /** Create a boolProperty
      */
    APrivateBoolProperty( AProperty *d )
        {
        data = d;
        connect( data, SIGNAL(onPropertyChange(AProperty*)), this, SLOT(propertyChanged(AProperty*)) );
        connect( this, SIGNAL(toggled(bool)), this, SLOT(guiChanged(bool)));
        propertyChanged( d );
        }
private slots:
    virtual void guiChanged( bool val )
        { *data = val; }
    virtual void propertyChanged( AProperty *in )
        { setChecked( in->value().toBool() ); }
private:
    AProperty *data;
    };

/** \brief APrivateIntProperty Sets and displays a GUI int value.
  */
class APrivateIntProperty : public QSpinBox
    {
    Q_OBJECT
public:
    /** Create an intProperty
      */
    APrivateIntProperty( AProperty *d ) : QSpinBox( )
        {
        data = d;
        connect( data, SIGNAL(onPropertyChange(AProperty*)), this, SLOT(propertyChanged(AProperty*)) );
        connect( this, SIGNAL(valueChanged(int)), this, SLOT(guiChanged(int)));
        setValue( d->value().toDouble() );
        setMinimum( -0x7FFFFFF );
        setMaximum( 0x7FFFFFF );
        }
private slots:
    virtual void guiChanged( int val )
        { *data = val; }
    virtual void propertyChanged( AProperty *in )
        { setValue( in->value().toInt() ); }
private:
    AProperty *data;
    };

/** \brief APrivateFloatProperty Sets and displays a GUI float value.
  */
class APrivateFloatProperty : public XFloatWidget
    {
    Q_OBJECT
public:
    /** Create a float property
      */
    APrivateFloatProperty( AProperty *d ) : XFloatWidget( d->value().toDouble() )
        {
        data = d;
        connect( data, SIGNAL(onPropertyChange(AProperty*)), this, SLOT(propertyChanged(AProperty*)) );
        connect( this, SIGNAL(valueChanged(double)), this, SLOT(guiChanged(double)));
        }
private slots:
    virtual void guiChanged( double val )
        { *data = val; }
    virtual void propertyChanged( AProperty *in )
        { setValue( in->value().toDouble() ); }
private:
    AProperty *data;
    };

/** \brief APrivateVector2DProperty Sets and displays a GUI for a 3D vector.
  */
class APrivateVector2DProperty : public QWidget
    {
    Q_OBJECT
public:
    /** create a vectorProperty */
    APrivateVector2DProperty( AProperty *d )
            : _stack( new QStackedWidget( this ) ),
            _vec( new XVector2DWidget( d->value().toVector2D() ) ),
            _sca( new XFloatWidget( d->value().toVector2D().x() ) )
        {
        setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Fixed );

        QHBoxLayout *layout( new QHBoxLayout( this ) );
        layout->setContentsMargins( 0, 0, 0, 0 );

        _button = new QToolButton( this );
        _button->setIconSize( QSize( 10, 10 ) );
        _button->setCheckable( true );
        _button->setAutoRaise( true );

        QVBoxLayout *buttonLayout( new QVBoxLayout( ) );
        buttonLayout->addWidget( _button );
        buttonLayout->addStretch();

        layout->addLayout( buttonLayout );
        layout->addWidget( _stack );

        _stack->addWidget( _sca );
        _stack->addWidget( _vec );
        data = d;

        toggleMode( data->attributes()["expandGUIMode"].toInt() );

        connect( _button, SIGNAL(clicked(bool)), this, SLOT(toggleMode(bool)) );
        connect( data, SIGNAL(onPropertyChange(AProperty*)), this, SLOT(propertyChanged(AProperty*)) );
        connect( _vec, SIGNAL(valueChanged(XVector2D)), this, SLOT(guiChanged(XVector2D)));
        connect( _sca, SIGNAL(valueChanged(double)), this, SLOT(guiChanged(double)));
        }
    virtual QSize sizeHint() const
        {
        return QSize( QWidget::sizeHint().width(), maximumHeight() );
        }
private slots:
    void toggleMode( bool b )
        {
        _button->setChecked( b );
        _stack->setCurrentIndex( b );
        setMaximumHeight( _stack->currentWidget()->sizeHint().height() );
        data->attribute( "expandGUIMode" ) = (int)b;

        if( b )
            {
            _button->setIcon( QIcon( ":/app/close.svg") );
            }
        else
            {
            _button->setIcon( QIcon( ":/app/open.svg") );
            }
        }
    void guiChanged( XVector2D val )
        { *data = val; }
    void guiChanged( double val )
        { *data = XVector2D( val, val ); }
    void propertyChanged( AProperty *in )
        {
        _vec->setValue( in->value().toVector2D() );
        _sca->setValue( in->value().toDouble() );
        }
private:
    QToolButton *_button;
    AProperty *data;
    QStackedWidget *_stack;
    XVector2DWidget *_vec;
    XFloatWidget *_sca;
    };

/** \brief APrivateVector3DProperty Sets and displays a GUI for a 3D vector.
  */
class APrivateVector3DProperty : public QWidget
    {
    Q_OBJECT
public:
    /** create a vectorProperty */
    APrivateVector3DProperty( AProperty *d )
            : _stack( new QStackedWidget( this ) ),
            _vec( new XVector3DWidget( d->value().toVector3D() ) ),
            _sca( new XFloatWidget( d->value().toVector3D().x() ) )
        {
        setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Fixed );

        QHBoxLayout *layout( new QHBoxLayout( this ) );
        layout->setContentsMargins( 0, 0, 0, 0 );

        _button = new QToolButton( this );
        _button->setIconSize( QSize( 10, 10 ) );
        _button->setCheckable( true );
        _button->setAutoRaise( true );

        QVBoxLayout *buttonLayout( new QVBoxLayout( ) );
        buttonLayout->addWidget( _button );
        buttonLayout->addStretch();

        layout->addLayout( buttonLayout );
        layout->addWidget( _stack );

        _stack->addWidget( _sca );
        _stack->addWidget( _vec );
        data = d;

        toggleMode( data->attributes()["expandGUIMode"].toInt() );

        connect( _button, SIGNAL(clicked(bool)), this, SLOT(toggleMode(bool)) );
        connect( data, SIGNAL(onPropertyChange(AProperty*)), this, SLOT(propertyChanged(AProperty*)) );
        connect( _vec, SIGNAL(valueChanged(XVector3D)), this, SLOT(guiChanged(XVector3D)));
        connect( _sca, SIGNAL(valueChanged(double)), this, SLOT(guiChanged(double)));
        }
    virtual QSize sizeHint() const
        {
        return QSize( QWidget::sizeHint().width(), maximumHeight() );
        }
private slots:
    void toggleMode( bool b )
        {
        _button->setChecked( b );
        _stack->setCurrentIndex( b );
        setMaximumHeight( _stack->currentWidget()->sizeHint().height() );
        data->attribute( "expandGUIMode" ) = (int)b;

        if( b )
            {
            _button->setIcon( QIcon( ":/app/close.svg") );
            }
        else
            {
            _button->setIcon( QIcon( ":/app/open.svg") );
            }
        }
    void guiChanged( XVector3D val )
        { *data = val; }
    void guiChanged( double val )
        { *data = XVector3D( val, val, val ); }
    void propertyChanged( AProperty *in )
        {
        _vec->setValue( in->value().toVector3D() );
        _sca->setValue( in->value().toDouble() );
        }
private:
    QToolButton *_button;
    AProperty *data;
    QStackedWidget *_stack;
    XVector3DWidget *_vec;
    XFloatWidget *_sca;
    };

/** \brief APrivateVector4DProperty Sets and displays a GUI for a 3D vector.
  */
class APrivateVector4DProperty : public QWidget
    {
    Q_OBJECT
public:
    /** create a vectorProperty */
    APrivateVector4DProperty( AProperty *d )
            : _stack( new QStackedWidget( this ) ),
            _vec( new XVector4DWidget( d->value().toVector4D() ) ),
            _sca( new XFloatWidget( d->value().toVector4D().x() ) )
        {
        setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Fixed );

        QHBoxLayout *layout( new QHBoxLayout( this ) );
        layout->setContentsMargins( 0, 0, 0, 0 );

        _button = new QToolButton( this );
        _button->setIconSize( QSize( 10, 10 ) );
        _button->setCheckable( true );
        _button->setAutoRaise( true );

        QVBoxLayout *buttonLayout( new QVBoxLayout( ) );
        buttonLayout->addWidget( _button );
        buttonLayout->addStretch();

        layout->addLayout( buttonLayout );
        layout->addWidget( _stack );

        _stack->addWidget( _sca );
        _stack->addWidget( _vec );
        data = d;

        toggleMode( data->attributes()["expandGUIMode"].toInt() );

        connect( _button, SIGNAL(clicked(bool)), this, SLOT(toggleMode(bool)) );
        connect( data, SIGNAL(onPropertyChange(AProperty*)), this, SLOT(propertyChanged(AProperty*)) );
        connect( _vec, SIGNAL(valueChanged(XVector4D)), this, SLOT(guiChanged(XVector4D)));
        connect( _sca, SIGNAL(valueChanged(double)), this, SLOT(guiChanged(double)));
        }
    virtual QSize sizeHint() const
        {
        return QSize( QWidget::sizeHint().width(), maximumHeight() );
        }
private slots:
    void toggleMode( bool b )
        {
        _button->setChecked( b );
        _stack->setCurrentIndex( b );
        setMaximumHeight( _stack->currentWidget()->sizeHint().height() );
        data->attribute( "expandGUIMode" ) = (int)b;

        if( b )
            {
            _button->setIcon( QIcon( ":/app/close.svg") );
            }
        else
            {
            _button->setIcon( QIcon( ":/app/open.svg") );
            }
        }
    void guiChanged( XVector4D val )
        { *data = val; }
    void guiChanged( double val )
        { *data = XVector4D( val, val, val, val ); }
    void propertyChanged( AProperty *in )
        {
        _vec->setValue( in->value().toVector4D() );
        _sca->setValue( in->value().toDouble() );
        }
private:
    QToolButton *_button;
    AProperty *data;
    QStackedWidget *_stack;
    XVector4DWidget *_vec;
    XFloatWidget *_sca;
    };

/** \brief APrivateColourProperty Sets and displays a GUI colour value.
  */
class APrivateColourProperty : public XColourWidget
    {
    Q_OBJECT
public:
    /** Create a colourProperty
      */
    APrivateColourProperty( AProperty *d, bool alpha=TRUE ) : XColourWidget( d->value().toColour(), alpha )
        {
        data = d;
        connect( data, SIGNAL(onPropertyChange(AProperty*)), this, SLOT(propertyChanged(AProperty*)) );
        connect( this, SIGNAL(colourChanged(XColour)), this, SLOT(guiChanged(XColour)));
        }
private slots:
    virtual void guiChanged( XColour col )
        { *data = col; }
    virtual void propertyChanged( AProperty *in )
        { setColour( in->value().toColour() ); }
private:
    AProperty *data;
    };

/** \brief APrivateFileProperty Sets and displays a GUI file value.
  */
class APrivateFileProperty : public QWidget
    {
    Q_OBJECT
public:
    /** Create a fileProperty
      */
    APrivateFileProperty( AProperty *d ) : _layout( new QHBoxLayout( this ) ), _label( new QLineEdit( this ) ),
            _button( new QToolButton( this ) )
        {
        data = d;

        _layout->setContentsMargins( 0, 0, 0, 0 );
        _layout->addWidget( _label );
        _layout->addWidget( _button );

        _label->setReadOnly( TRUE );
        _label->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
        _button->setText( "..." );

        connect( data, SIGNAL(onPropertyChange(AProperty*)), this, SLOT(propertyChanged(AProperty*)) );
        connect( _button, SIGNAL(clicked()), this, SLOT(guiChanged()) );
        propertyChanged( d );
        }
private slots:
    virtual void guiChanged( )
        {
        QSettings settings;
        QString file( AFileDialog::getOpenFileName( "Select File for " + data->displayName() ) );
        *data = file;
        QFileInfo fileInfo( file );
        settings.setValue( "lastDirAccessed", fileInfo.absoluteDir().absolutePath() );
        }
    virtual void propertyChanged( AProperty *in )
        {
        _label->setText( in->value().toString() );
        }
private:
    AProperty *data;
    QHBoxLayout *_layout;
    QLineEdit *_label;
    QToolButton *_button;
    };

/** \brief APrivateDirectoryProperty Sets and displays a GUI file value.
  */
class APrivateDirectoryProperty : public QWidget
    {
    Q_OBJECT
public:
    /** Create a directoryProperty
      */
    APrivateDirectoryProperty( AProperty *d ) : _layout( new QHBoxLayout( this ) ), _label( new QLineEdit( this ) ),
            _button( new QToolButton( this ) )
        {
        data = d;

        _layout->setContentsMargins( 0, 0, 0, 0 );
        _layout->addWidget( _label );
        _layout->addWidget( _button );

        _label->setReadOnly( TRUE );
        _label->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
        _button->setText( "..." );

        connect( data, SIGNAL(onPropertyChange(AProperty*)), this, SLOT(propertyChanged(AProperty*)) );
        connect( _button, SIGNAL(clicked()), this, SLOT(guiChanged()) );
        propertyChanged( d );
        }
private slots:
    virtual void guiChanged( )
        {
        QSettings settings;
        QString file( AFileDialog::getExistingDirectory( "Select Directory for " + data->displayName() ) );
        *data = file;
        settings.setValue( "lastDirAccessed", file );
        }
    virtual void propertyChanged( AProperty *in )
        {
        _label->setText( in->value().toString() );
        }
private:
    AProperty *data;
    QHBoxLayout *_layout;
    QLineEdit *_label;
    QToolButton *_button;
    };

/** \brief APrivateStringProperty Sets and displays a GUI string value with a QLineEdit.
  */
class APrivateStringProperty : public QLineEdit
    {
    Q_OBJECT
public:
    /** Create a stringProperty
      */
    APrivateStringProperty( AProperty *d )
        {
        data = d;
        connect( data, SIGNAL(onPropertyChange(AProperty*)), this, SLOT(propertyChanged(AProperty*)) );
        connect( this, SIGNAL(editingFinished()), this, SLOT(guiChanged()) );
        propertyChanged( d );
        }
private slots:
    virtual void guiChanged( )
        { *data = text(); }
    virtual void propertyChanged( AProperty *in )
        { setText( in->value().toString() ); }
private:
    AProperty *data;
    };

/** \brief APrivateLongStringProperty Sets and displays a GUI string value with a QTextEdit.
  */
class APrivateLongStringProperty : public QTextEdit
    {
    Q_OBJECT
public:
    /** Create a longStringProperty
      */
    APrivateLongStringProperty( AProperty *d )
        {
        data = d;
        connect( data, SIGNAL(onPropertyChange(AProperty*)), this, SLOT(propertyChanged(AProperty*)) );
        connect( this, SIGNAL(textChanged()), this, SLOT(guiChanged()) );
        propertyChanged( d );
        setAcceptRichText( FALSE );
        _changing = FALSE;
        }
private slots:
    virtual void guiChanged( )
        { if(!_changing){_changing=TRUE;*data = toPlainText();_changing=FALSE;} }
    virtual void propertyChanged( AProperty *in )
        {
        if(!_changing)
            {
            _changing=TRUE;
            setText( in->value().toString() );
            _changing=FALSE;
            }
        }
private:
    AProperty *data;
    bool _changing;
    };

/** \brief APrivateFileSequenceProperty creates a gui property for file sequences
  */
class APrivateFileSequenceProperty : public QWidget
    {
    Q_OBJECT
public:
    /** Create a file sequence property
      */
    APrivateFileSequenceProperty( AProperty *d ) : _layout( new QHBoxLayout( this ) ),
            _label( new QLineEdit( this ) ), _button( new QToolButton( this ) )
        {
        data = d;

        _layout->setContentsMargins( 0, 0, 0, 0 );
        _layout->addWidget( _label );
        _layout->addWidget( _button );

        _label->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
        _button->setText( "..." );

        connect( data, SIGNAL(onPropertyChange(AProperty*)), this, SLOT(propertyChanged(AProperty*)) );
        connect( _button, SIGNAL(clicked()), this, SLOT(guiChanged()) );
        connect( _label, SIGNAL(editingFinished()), this, SLOT(guiForceChanged()) );
        propertyChanged( d );
        }

private slots:
    void guiForceChanged()
        {
        *data = XFileSequence( _label->text(), XFileSequence::Parsed );
        }

    void guiChanged( )
        {
        QString fN( AFileDialog::getOpenFileName( "Choose a file from the sequence" ) );
        data->attribute("chosenFile") = fN;
        ALog << "File Sequence" << fN << data->attributes()["chosenFile"];
        *data = XFileSequence( fN, XFileSequence::Auto );
        }

    void propertyChanged( AProperty *in )
        {
        _label->setText( XFileSequence( in->value() ).parsedFilename() );
        }

private:
    AProperty *data;
    QHBoxLayout *_layout;
    QLineEdit *_label;
    QToolButton *_button;
    };

}

/** \endcond
  */

#endif // GUIPROPERTIES_H
