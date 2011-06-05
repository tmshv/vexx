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

  template <typename T> T *setting(const QString &ns, const QString &name)
    {
    SEntity *nsEnt = _db.settings.findChildEntity(ns);
    if(nsEnt)
      {
      SProperty *prop = nsEnt->findChild(name);
      if(prop)
        {
        return prop->castTo<T>();
        }

      xAssert(!prop);
      return nsEnt->addProperty<T>(name);
      }

    nsEnt = _db.settings.addChild<SEntity>(ns);
    xAssert(nsEnt);
    return nsEnt->addProperty<T>(name);
    }

  template <typename T> const T *setting(const QString &ns, const QString &name) const
    {
    SEntity *nsEnt = _db.settings.findChildEntity(ns);
    if(nsEnt)
      {
      SProperty *prop = nsEnt->findChild(name);
      if(prop)
        {
        return prop->castTo<T>();
        }
      }
    return 0;
    }

private:
  virtual void load();
  virtual void unload();

  SAppDatabase _db;
  };

#endif // SPLUGIN_H
