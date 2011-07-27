#include "../SDK/include/AEvent.h"

/** \class AEvent
  \brief A base class for events in the application
  */
/** A Protected constructor so an event cannot be constructed without a superclass
  */
AEvent::AEvent()
    {
    }

/** \class ASelectionEvent
  \brief A class providing information about a selection event in the application
  */

/** ASelectionEvent::mode()
  The selection mode after the selection event (now).
  */


/** Construct a selection event with a specified mode.
  */
ASelectionEvent::ASelectionEvent( SelectionMode m ) : _mode( m )
    {
    }
