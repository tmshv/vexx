#ifndef AABSTRACTPLUGIN_H
#define AABSTRACTPLUGIN_H

#include "aglobal.h"
#include "XObject"
#include "XProperty"

#define ALTER_PLUGIN( name ) \
  extern "C" Q_DECL_EXPORT AAbstractPlugin *initAlterPlugin( ) \
  { \
  return new name; \
  }

class ACore;

class ALTER_EXPORT AAbstractPlugin : public QObject
  {
  Q_OBJECT

XProperties:
  XProperty( ACore *, core, setCore );

public:
  AAbstractPlugin();

protected:
  virtual void load();
  virtual void unload();
  virtual void pluginAdded(const QString &type);
  virtual void pluginRemoved(const QString &type);

  friend class ACore;
  };

#endif // AABSTRACTPLUGIN_H
