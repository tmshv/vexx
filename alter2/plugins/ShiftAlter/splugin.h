#ifndef SPLUGIN_H
#define SPLUGIN_H

#include "saglobal.h"
#include "aabstractplugin.h"
#include "sappdatabase.h"

class SHIFTALTER_EXPORT SPlugin : public AAbstractPlugin
  {
  Q_OBJECT

public:
  SPlugin();

  SAppDatabase &db();
  const SAppDatabase &db() const;



private:
  virtual void load();
  virtual void unload();

  SAppDatabase _db;
  };

#endif // SPLUGIN_H
