#ifndef APLUGIN_H
#define APLUGIN_H

#include "aabstractplugin.h"
#include "acore.h"


template <typename T> class APlugin
  {
public:
  APlugin(AAbstractPlugin *requester, const QString &name);
  APlugin(ACore &requester, const QString &name);

  T &operator *();
  T *operator->();
  T *plugin();
  bool isValid();

private:
  T *_plugin;
  };

template <typename T>inline T &APlugin<T>::operator *()
  {
  xAssert( _plugin );
  return _plugin;
  }

template <typename T>inline T *APlugin<T>::operator->()
  {
  xAssert( _plugin );
  return _plugin;
  }

template <typename T>inline T *APlugin<T>::plugin()
  {
  xAssert( _plugin );
  return _plugin;
  }

template <typename T> APlugin<T>::APlugin( AAbstractPlugin *requester, const QString &name )
  {
  xAssert( requester );
  xAssert( requester->core() );

  _plugin = qobject_cast<T*>(requester->core()->plugin(name));
  }

template <typename T> APlugin<T>::APlugin(ACore &requester, const QString &name)
  {
  _plugin = qobject_cast<T*>(requester.plugin(name));
  }

template <typename T> bool APlugin<T>::isValid()
  {
  return _plugin != 0;
  }

#endif // APLUGIN_H
