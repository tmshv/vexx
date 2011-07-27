#ifndef FRAMELOGDOCK_H
#define FRAMELOGDOCK_H

#include "AAbstractDock"

class QPlainTextEdit;
class QVBoxLayout;

class frameLogDock : public AAbstractDock
    {
public:
    frameLogDock();

    void setText( QString );

private:
    QPlainTextEdit *_text;
    QVBoxLayout *_layout;
    };

#endif // FRAMELOGDOCK_H
