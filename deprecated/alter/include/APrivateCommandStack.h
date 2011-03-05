#ifndef COMMANDSTACK_H
#define COMMANDSTACK_H

#include "QObject"
#include "QVector"
#include "QTime"
#include "APrivateCommand.h"

/** \cond INTERNAL
   */

class QAction;

class APrivateCommandStack : public QObject
    {
    Q_OBJECT
public:
    APrivateCommandStack(QObject *parent = 0);
    ~APrivateCommandStack();

    QAction *createUndoAction( QObject *, QString );
    QAction *createRedoAction( QObject *, QString );

    void push( APrivateCommand * );
    void clear();

    void beginBlockChange();
    void endBlockChange();

public slots:
    void undo();
    void redo();

private:
    void updateStates();
    bool _canUndo;
    bool _canRedo;
    QVector <APrivateCommand *> _commands;
    int _index;
    QAction *_undo;
    QAction *_redo;
    bool _changingState;
    APrivateCommand *_currentParent;
    QTime _lastPush;
    int _undoBlock;
    APrivateCommand *_blockParent;
    };

/** \endcond
  */

#endif // COMMANDSTACK_H
