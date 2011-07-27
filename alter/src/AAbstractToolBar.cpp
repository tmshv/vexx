#include "../SDK/AAbstractToolBar"

/** \class AAbstractToolBar
  \brief This class allows the programmer to add a toolbar to the application.
  \sa APlugin::addToolBar()
  \sa APlugin::removeToolBar()
  */

/** Default constructor for the toolbar
  */
AAbstractToolBar::AAbstractToolBar() : _app( 0 )
    {
    }

/** Destory the toolbar
  */
void AAbstractToolBar::added()
    {
    }
