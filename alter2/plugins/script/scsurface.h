#ifndef SCSURFACE_H
#define SCSURFACE_H

#include "UISurface.h"
#include "QFile"
#include "QObject"

class ScPlugin;
class QTextEdit;

class ScSurface : public QObject, public UISurface
  {
  Q_OBJECT

XProperties:
  XROProperty(ScPlugin *, plugin);

public:
  ScSurface(ScPlugin *plugin);
  ~ScSurface();

  void threadSafeLog(QtMsgType t, const QString &);

public slots:
  void appendToLog(QtMsgType t, const QString &);

private slots:
  void execute();
  void executeDebugged();

signals:
  void threadSafeLogSignal(QtMsgType t, const QString &);

private:
  QTextEdit *_editor;
  QTextEdit *_log;
  QFile _logFile;
  friend void logHandler(QtMsgType msgType, const char *message);
  };

#endif // SCSURFACE_H
