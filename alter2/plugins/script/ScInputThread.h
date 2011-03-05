#ifndef SCINPUTTHREAD_H
#define SCINPUTTHREAD_H

#include "QThread"

class ScPlugin;

class ScInputThread : public QThread
    {
    Q_OBJECT
public:
    ScInputThread( ScPlugin *plugin );
    void run();

signals:
    void triggerExecute( QString );

public slots:
    void execute( QString );

private:
    ScPlugin *_plugin;
    };

#endif // SCINPUTTHREAD_H
