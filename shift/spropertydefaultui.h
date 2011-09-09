#ifndef SPROPERTYDEFAULTUI_H
#define SPROPERTYDEFAULTUI_H

#include "sglobal.h"
#include "QCheckBox"
#include "QSpinBox"
#include "QLineEdit"
#include "QTextEdit"
#include "XFloatWidget"
#include "XVector2DWidget"
#include "XVector3DWidget"
#include "XColourWidget"
#include "sbaseproperties.h"

#include "sproperty.h"
#include "XProperty"


namespace SPropertyDefaultUI
{
template <typename T, typename U> class SUIBase : private SDirtyObserver
  {
XProperties:
  XProperty(bool, isAlreadySetting, setAlreadySetting);

public:
  SUIBase(SProperty *p) : _isAlreadySetting(false), _value(p->castTo<T>()), _dirty(false)
    {
    xAssert(_value);
    }
  T *propertyValue() {return _value;}
  virtual void syncGUI() = 0;

private:
  virtual void onPropertyDirtied(const SProperty* prop)
    {
    if(prop == _value)
      {
      _dirty = true;
      }
    }
  virtual void actOnChanges()
    {
    if(_dirty)
      {
      _isAlreadySetting = true;
      syncGUI();
      _isAlreadySetting = false;
      _dirty = true;
      }
    }
  T *_value;
  bool _dirty;
  };

class Bool : public QCheckBox, private SUIBase<BoolProperty, BoolProperty::Change>
  {
  Q_OBJECT
public:
  Bool( SProperty *prop, bool readOnly, QWidget *parent ) : QCheckBox(parent), SUIBase<BoolProperty, BoolProperty::Change>(prop)
    {
    connect( this, SIGNAL(toggled(bool)), this, SLOT(guiChanged(bool)));
    syncGUI();
    setDisabled(readOnly);
    }

  Q_SLOT void guiChanged( bool val ) { propertyValue()->assign(val); }
  void syncGUI() { setChecked( propertyValue()->value() ); }
  };

class Int32 : public QSpinBox, private SUIBase<IntProperty, IntProperty::Change>
 {
 Q_OBJECT
public:
 Int32( SProperty *prop, bool readOnly, QWidget *parent ) : QSpinBox(parent), SUIBase<IntProperty, IntProperty::Change>(prop)
   {
   connect( this, SIGNAL(valueChanged(int)), this, SLOT(guiChanged(int)));
   setAlreadySetting(true);
   syncGUI();
   setAlreadySetting(false);
   setMinimum( -0x7FFFFFF );
   setMaximum( 0x7FFFFFF );
   setReadOnly(readOnly);
   }

 Q_SLOT void guiChanged( int val ) { propertyValue()->assign((xint32)val); }
 void syncGUI() { setValue(propertyValue()->value()); }
 };

class UInt32 : public QSpinBox, private SUIBase<UnsignedIntProperty, UnsignedIntProperty::Change>
 {
 Q_OBJECT
public:
 UInt32( SProperty *prop, bool readOnly, QWidget *parent ) : QSpinBox(parent), SUIBase<UnsignedIntProperty, UnsignedIntProperty::Change>(prop)
   {
   connect( this, SIGNAL(valueChanged(int)), this, SLOT(guiChanged(int)));
   setAlreadySetting(true);
   syncGUI();
   setAlreadySetting(false);
   setMinimum( -0x7FFFFFF );
   setMaximum( 0x7FFFFFF );
   setReadOnly(readOnly);
   }

 Q_SLOT void guiChanged( int val ) { propertyValue()->assign((xuint32)val); }
 void syncGUI() { setValue(propertyValue()->value()); }
 };

class Int64 : public QSpinBox, private SUIBase<LongIntProperty, LongIntProperty::Change>
 {
 Q_OBJECT
public:
 Int64( SProperty *prop, bool readOnly, QWidget *parent ) : QSpinBox(parent), SUIBase<LongIntProperty, LongIntProperty::Change>(prop)
   {
   connect( this, SIGNAL(valueChanged(int)), this, SLOT(guiChanged(int)));
   setAlreadySetting(true);
   syncGUI();
   setAlreadySetting(false);
   setMinimum( -0x7FFFFFF );
   setMaximum( 0x7FFFFFF );
   setReadOnly(readOnly);
   }

 Q_SLOT void guiChanged( int val ) { propertyValue()->assign((xint64)val); }
 void syncGUI() { setValue(propertyValue()->value()); }
 };

class UInt64 : public QSpinBox, private SUIBase<LongUnsignedIntProperty, LongUnsignedIntProperty::Change>
 {
 Q_OBJECT
public:
 UInt64( SProperty *prop, bool readOnly, QWidget *parent ) : QSpinBox(parent), SUIBase<LongUnsignedIntProperty, LongUnsignedIntProperty::Change>(prop)
   {
   connect( this, SIGNAL(valueChanged(int)), this, SLOT(guiChanged(int)));
   setAlreadySetting(true);
   syncGUI();
   setAlreadySetting(false);
   setMinimum( -0x7FFFFFF );
   setMaximum( 0x7FFFFFF );
   setReadOnly(readOnly);
   }

 Q_SLOT void guiChanged( int val ) { propertyValue()->assign((xuint64)val); }
 void syncGUI() { setValue(propertyValue()->value()); }
 };

class Float : public XFloatWidget, private SUIBase<FloatProperty, FloatProperty::Change>
  {
  Q_OBJECT
public:
  Float( SProperty *prop, bool readOnly, QWidget *parent ) : XFloatWidget(0.0f, parent), SUIBase<FloatProperty, FloatProperty::Change>(prop)
    {
    connect( this, SIGNAL(valueChanged(double)), this, SLOT(guiChanged(double)));
    setReadOnly(readOnly);
    syncGUI();
    }

  Q_SLOT void guiChanged( double val ) { propertyValue()->assign((float)val); }
  void syncGUI() { setValue(propertyValue()->value()); }
  };

class Double : public XFloatWidget, private SUIBase<DoubleProperty, DoubleProperty::Change>
  {
  Q_OBJECT
public:
  Double( SProperty *prop, bool readOnly, QWidget *parent ) : XFloatWidget(0.0f, parent), SUIBase<DoubleProperty, DoubleProperty::Change>(prop)
    {
    connect( this, SIGNAL(valueChanged(double)), this, SLOT(guiChanged(double)));
    setReadOnly(readOnly);
    syncGUI();
    }

  Q_SLOT void guiChanged( double val ) { propertyValue()->assign(val); }
  void syncGUI() { setValue( propertyValue()->value() ); }
  };

class String : public QLineEdit, private SUIBase<StringProperty, StringProperty::Change>
  {
  Q_OBJECT
public:
  String( SProperty *prop, bool readOnly, QWidget *parent ) : QLineEdit(parent), SUIBase<StringProperty, StringProperty::Change>(prop)
    {
    connect( this, SIGNAL(editingFinished()), this, SLOT(guiChanged()) );
    syncGUI();
    setReadOnly(readOnly);
    }

  Q_SLOT virtual void guiChanged( ) { propertyValue()->assign(text()); }
  void syncGUI() { setText(propertyValue()->value()); }
  };

class LongString : public QTextEdit, private SUIBase<StringProperty, StringProperty::Change>
  {
  Q_OBJECT
public:
  LongString( SProperty *prop, bool readOnly, QWidget *parent ) : QTextEdit(parent), SUIBase<StringProperty, StringProperty::Change>(prop)
    {
    connect( this, SIGNAL(textChanged()), this, SLOT(guiChanged()) );
    syncGUI();
    setAcceptRichText( FALSE );
    setReadOnly(readOnly);
    }

  Q_SLOT virtual void guiChanged( ) { propertyValue()->assign(toPlainText());}
  void syncGUI() { setText( propertyValue()->value() ); }
  };

class Vector2D : public XVector2DWidget, private SUIBase<Vector2DProperty, Vector2DProperty::Change>
  {
  Q_OBJECT
public:
  Vector2D( SProperty *prop, bool readOnly, QWidget *parent ) : XVector2DWidget( XVector2D(), QStringList(), parent ), SUIBase<Vector2DProperty, Vector2DProperty::Change>(prop)
    {
    connect( this, SIGNAL(valueChanged(XVector2D)), this, SLOT(guiChanged(XVector2D)));
    setReadOnly(readOnly);
    }

private slots:
  void guiChanged( XVector2D val ) { propertyValue()->assign(val); }

private:
  void syncGUI() { setValue( propertyValue()->value() ); }
  };


class Vector3D : public XVector3DWidget, private SUIBase<Vector3DProperty, Vector3DProperty::Change>
  {
  Q_OBJECT
public:
  Vector3D( SProperty *prop, bool readOnly, QWidget *parent ) : XVector3DWidget( XVector3D(), QStringList(), parent), SUIBase<Vector3DProperty, Vector3DProperty::Change>(prop)
    {
    connect( this, SIGNAL(valueChanged(XVector3D)), this, SLOT(guiChanged(XVector3D)));
    setReadOnly(readOnly);
    syncGUI();
    }

private slots:
  void guiChanged( XVector3D val ) { propertyValue()->assign(val); }

private:
  void syncGUI() { setValue( propertyValue()->value() ); }
  };

class Colour : public XColourWidget, private SUIBase<ColourProperty, ColourProperty::Change>
  {
  Q_OBJECT
public:
  Colour( SProperty *prop, bool X_UNUSED(readOnly), QWidget *parent ) : XColourWidget( XColour(), false, parent ), SUIBase<ColourProperty, ColourProperty::Change>(prop)
    {
    connect( this, SIGNAL(colourChanged(XColour)), this, SLOT(guiChanged(XColour)));
    // setReadOnly(readOnly); <- implement this...
    syncGUI();
    }
private slots:
  virtual void guiChanged( const XColour &col ) { propertyValue()->assign(col); }

private:
  void syncGUI() { setColour( propertyValue()->value() ); }
  };

#if 0
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
#endif
}

#endif // SPROPERTYDEFAULTUI_H
