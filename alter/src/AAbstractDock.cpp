#include "../SDK/AAbstractDock"
#include "../SDK/ALog"
#include "APrivateWindow.h"
#include "QMenu"

/** \class AAbstractDock
  \brief \c AAbstractDock is an abstract class used to represent a "dock" in the application.
  The programmer can attach custom functionality and GUI classes to a child class and have it appear as part of the application.
  \sa APlugin::addDockedItem()
  \sa APlugin::removeDockedItem()
  */

/** \fn AAbstractDock::XProperty(QString, name, setName)
  This property provides functions for setting ( \p setName() ) and getting ( \p name() ) the name of the dock.
  */
/** \fn AAbstractDock::XProperty(bool, hasVerticalTitlebar, setVerticalTitlebar)
  This property provides functions for setting ( \p setVerticalTitlebar() ) and getting ( \p hasVerticalTitlebar() ) whether the dock is drawn with a vertical titlebar.
  */
/** \fn AAbstractDock::XProperty(APlugin *, app, setApp)
  This property provides functions for setting ( \p setApp() ) and getting ( \p app() ) the application pointer.
  */

/** Default constructor for the dock.
  Most initialisation of a dock should be done in added(), becuase when the constructor is called the application pointer app() is not valid yet.
  \sa added()
  */
AAbstractDock::AAbstractDock( ) : XGetPropertyMember(hasVerticalTitlebar)( FALSE ), _app( 0 )
    {
    _base = new QWidget();
    }

/** Destroy the dock
  */
AAbstractDock::~AAbstractDock( )
    {
    delete _base;
    }

/** Called when the dock is added to the application, this is the point at which \c app() becomes a valid pointer
  */
void AAbstractDock::added()
    {
    }

/** Returns a pointer to the Gui surface the child must attach to.
  */
QWidget *AAbstractDock::base()
    {
    return _base;
    }

/** Returns whether the dock is visible or not, so the programmer can not complete expensive operations.
  */
bool AAbstractDock::isVisible()
    {
    return _base->isVisible();
    }

/** Set the titlebar position to pos
  \deprecated
  */
void AAbstractDock::setTitleBarPosition( TitleBarPosition pos )
    {
    if( pos == Vertical )
        {
        setVerticalTitlebar( TRUE );
        }
    else
        {
        setVerticalTitlebar( FALSE );
        }
    }

/** This functions returns a menu that is relative to this dock, used on titlebar clicks etc.
  */
void AAbstractDock::menu( QMenu * )
    {
    }
