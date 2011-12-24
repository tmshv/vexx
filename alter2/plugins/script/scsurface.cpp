#include "QWidget"
#include "QVBoxLayout"
#include "QTextEdit"
#include "QToolBar"
#include "QAction"
#include "QDir"
#include "QSplitter"

#include "scplugin.h"
#include "scsurface.h"
#include "QScrollBar"
#include "acore.h"
#include <iostream>

ScSurface *g_surface = 0;
QtMsgHandler g_oldHandler = 0;

void logHandler(QtMsgType msgType, const char *message)
  {
  if(g_oldHandler)
    {
    g_oldHandler(msgType, message);
    }

#ifdef X_DEBUG
  if(QString(message).startsWith("QMutex::lock: Deadlock detected"))
    {
    xAssertFail();
    }
#endif

  static bool recursion = false;
  if(g_surface && !recursion)
    {
    recursion = true;
    if(msgType == QtFatalMsg)
      {
      xAssertFail();
      }

    g_surface->threadSafeLog(msgType, message);

    if(msgType > QtWarningMsg)
      {
      std::cerr << message;
      }
    recursion = false;
    }
  else
    {
    if(msgType != QtDebugMsg)
      {
      std::cerr << message;
      }
    else
      {
      std::cout << message;
      }
    }
  }

ScSurface::ScSurface(ScPlugin *plugin) : UISurface("Script", new QWidget(), UISurface::Dock), _plugin(plugin)
  {
  g_surface = this;

  QVBoxLayout *sLayout(new QVBoxLayout(widget()));
  sLayout->setContentsMargins(0, 0, 0, 0);
  sLayout->setSpacing(0);

  QSplitter *splitter = new QSplitter(widget());
  splitter->setOrientation(Qt::Vertical);
  sLayout->addWidget(splitter);

  _log = new QTextEdit(widget());
  g_oldHandler = qInstallMsgHandler(logHandler);
  connect(this, SIGNAL(threadSafeLogSignal(QtMsgType, QString)), this, SLOT(appendToLog(QtMsgType, QString)));
  splitter->addWidget(_log);
  _log->setReadOnly(true);


  QWidget *editor = new QWidget(widget());
  splitter->addWidget(editor);

  QVBoxLayout *layout(new QVBoxLayout(editor));
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  QToolBar *toolbar(new QToolBar(editor));
  toolbar->setIconSize(QSize(16,16));
  layout->addWidget(toolbar);

  toolbar->addAction(QIcon(":/Sc/run.svg"), "Execute", this, SLOT(execute()));

  toolbar->addAction(QIcon(":/Sc/runDebugged.svg"), "Execute with Debugging", this, SLOT(executeDebugged()));

  QAction *enableDebug = toolbar->addAction(QIcon(":/Sc/debug.svg"), "Enable Script Debugging");
  enableDebug->setCheckable(true);
  connect(_plugin, SIGNAL(debuggingStateChanged(bool)), enableDebug, SLOT(setChecked(bool)));
  connect(enableDebug, SIGNAL(toggled(bool)), _plugin, SLOT(enableDebugging(bool)));

  toolbar->addAction(QIcon(":/Sc/debugWindow.svg"), "Show Debugger", plugin, SLOT(showDebugger()));

  _editor = new QTextEdit(editor);
  layout->addWidget(_editor);

  QDir localData = plugin->core()->localDataDirectory();
  _logFile.setFileName(localData.absolutePath() + "/log.txt");
  bool open = _logFile.open(QIODevice::WriteOnly);
  xAssert(open);
  }

ScSurface::~ScSurface()
  {
  g_surface = 0;
  }

void ScSurface::execute()
  {
  _plugin->execute(_editor->toPlainText());
  }

void ScSurface::executeDebugged()
  {
  bool debugged = _plugin->isDebuggingEnabled();

  _plugin->enableDebugging(true);
  _plugin->execute(_editor->toPlainText());
  _plugin->enableDebugging(debugged);
  }

void ScSurface::threadSafeLog(QtMsgType t, const QString &message)
  {
  emit threadSafeLogSignal(t, message);
  }

void ScSurface::appendToLog(QtMsgType t, const QString &message)
  {
  QString finalMessage = message + "\n";

  QString htmlMessage = finalMessage;
  htmlMessage.replace('\n', "<br>\n");

  if(t == QtWarningMsg)
    {
    htmlMessage = QString("<font color=\"#FFA824\">%1</font>").arg(htmlMessage);
    }
  else if(t > QtWarningMsg)
    {
    htmlMessage = QString("<b><font color=\"red\">%1</font></b>").arg(htmlMessage);
    }

  _log->moveCursor(QTextCursor::End);
  _log->insertHtml(htmlMessage);

  // scroll to bottom
  QScrollBar *sb = _log->verticalScrollBar();
  sb->setValue(sb->maximum());

  _logFile.write(finalMessage.toUtf8());
  _logFile.flush();
  }
