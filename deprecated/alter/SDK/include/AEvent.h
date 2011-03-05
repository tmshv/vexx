#ifndef AEVENT_H
#define AEVENT_H

#include "XProperty"
#include "QtGlobal"

class AEvent
    {
protected:
    AEvent();

private:
    Q_DISABLE_COPY( AEvent );
    };

class ASelectionEvent : public AEvent
    {
public:
    enum SelectionMode { Selected, Unselected };
    XROProperty( SelectionMode, mode );

    ASelectionEvent( SelectionMode );
    };

#endif // AEVENT_H
