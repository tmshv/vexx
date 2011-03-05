#ifndef SPLUGIN_H
#define SPLUGIN_H

#include "saglobal.h"
#include "aabstractplugin.h"
#include "sdatabase.h"

class SHIFTALTER_EXPORT SPlugin : public AAbstractPlugin
  {
  Q_OBJECT

public:
  SPlugin();

  SDatabase &db();
  const SDatabase &db() const;

private:
  virtual void load();
  virtual void unload();

  SDatabase _db;
  };

#endif // SPLUGIN_H
