#ifndef SCSURFACE_H
#define SCSURFACE_H

#include "UISurface.h"
#include "QObject"

class ScPlugin;
class QTextEdit;

class ScSurface : public QObject, public UISurface
  {
  Q_OBJECT
properties:
  XROProperty(ScPlugin *, plugin);

public:
  ScSurface(ScPlugin *plugin);
  ~ScSurface();

  void threadSafeLog(const QString &);

public slots:
  void appendToLog(const QString &);

private slots:
  void execute();
  void executeDebugged();

signals:
  void threadSafeLogSignal(const QString &);

private:
  QTextEdit *_editor;
  QTextEdit *_log;
  friend void logHandler(QtMsgType msgType, const char *message);
  };

#endif // SCSURFACE_H
