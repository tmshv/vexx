#ifndef SPLUGIN_H
#define SPLUGIN_H

#include "saglobal.h"
#include "aabstractplugin.h"
#include "sappdatabase.h"

class XMathsEngine;

class SHIFTALTER_EXPORT SPlugin : public AAbstractPlugin
  {
  Q_OBJECT

public:
  SPlugin();

  SAppDatabase &db();
  const SAppDatabase &db() const;

  template <typename T> const T &setting(const QString &ns, const QString &name)
    {
    typedef typename SPODInterface<T>::Type PropertyType;

    PropertyType* prop = settingProperty<PropertyType>(ns, name);
    xAssert(prop);

    xAssert(prop);
    return SPODInterface<T>::value(prop);
    }

  template <typename T> const T &setting(const QString &ns, const QString &name) const
    {
    typedef typename SPODInterface<T>::Type PropertyType;

    PropertyType* prop = settingProperty<PropertyType>(ns, name);
    xAssert(prop);

    if(prop)
      {
      SPODInterface<T>::value(prop);
      }
    return 0;
    }

  template <typename T> void setSetting(const QString &ns, const QString &name, const T &val)
    {
    typedef typename SPODInterface<T>::Type PropertyType;

    PropertyType* prop = settingProperty<PropertyType>(ns, name);
    xAssert(prop);

    if(prop)
      {
      SPODInterface<T>::assign(prop, val);
      }
    }

  template <typename T> T *settingProperty(const QString &ns, const QString &name)
    {
    xAssert(_db);
    SEntity *nsEnt = _db->settings.findChildEntity(ns);
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

    nsEnt = _db->settings.addChild<SEntity>(ns);
    xAssert(nsEnt);
    return nsEnt->addProperty<T>(name);
    }

  template <typename T> const T *settingProperty(const QString &ns, const QString &name) const
    {
    xAssert(_db);
    SEntity *nsEnt = _db->settings().findChildEntity(ns);
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

  SAppDatabase *_db;
  XMathsEngine *_mathsEngine;
  };

#endif // SPLUGIN_H
