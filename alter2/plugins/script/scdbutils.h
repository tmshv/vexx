#ifndef SCDBUTILS_H
#define SCDBUTILS_H

#include "XConvert.h"
#include "spropertyinformation.h"
#include "styperegistry.h"

class XScriptArguments;
class XScriptValue;

XScriptValue registerTypeFn(XScriptArguments const &args);
XScriptValue registerExporterFn(XScriptArguments const &args);

namespace XScriptConvert
{
namespace internal
{
template <> struct JSToNative<SPropertyInformation>
  {
  typedef const SPropertyInformation *ResultType;

  //! Must be specialized to be useful.
  ResultType operator()(XScriptValue const &h) const
    {
    if(h.isObject())
      {
      XScriptObject obj(h);
      return STypeRegistry::findType(obj.get("typeName").toString());
      }
    else
      {
      return STypeRegistry::findType(h.toString());
      }
    }
  };

template <> struct NativeToJS<SPropertyInformation>
  {
  XScriptValue operator()(SPropertyInformation const &x) const
    {
    return x.typeName();
    }
  };
}
}


#endif // SCDBUTILS_H
