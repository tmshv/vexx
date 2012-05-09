#ifndef UIPLUGIN_H
#define UIPLUGIN_H

#include "UIGlobal.h"
#include "aabstractplugin.h"

class UISurface;
class UIPluginPrivate;

class UI_EXPORT UIPlugin : public AAbstractPlugin
  {
  Q_OBJECT

public:
  UIPlugin();

  Q_INVOKABLE QString getOpenFilename(const QString& fileType);
  Q_INVOKABLE QString getSaveFilename(const QString& fileType);

  Q_INVOKABLE void addDock(QString name, QWidget *w);
  void addSurface(UISurface *surface);
  void removeSurface(QString name);
  void removeSurface(UISurface *s);

  Q_INVOKABLE void show();

  Q_INVOKABLE void addQMLSurface(const QString &);

signals:
  void aboutToClose();

private:
  virtual void load();
  virtual void unload();

  void setLastUsedDirectoryFromFile(const QString &fName);
  QString lastUsedDirectory() const;

  UIPluginPrivate *_priv;
  friend class UIPluginPrivate;
  friend class UISurfacePrivate;
  };

#endif // UIPLUGIN_H
