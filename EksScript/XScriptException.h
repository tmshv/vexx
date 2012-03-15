#ifndef XSCRIPTEXCEPTION_H
#define XSCRIPTEXCEPTION_H

#include "QString"
#include "XScriptGlobal.h"

class XScriptValue;

EKSSCRIPT_EXPORT XScriptValue TossAsError( const QString &err );
EKSSCRIPT_EXPORT XScriptValue Toss(const QString &msg);

struct MissingThisException
  {
protected:
  QString msg;
  template <typename T>
  void init()
  {
    msg = QString("CastFromJS<%1>() returned NULL. Throwing to avoid dereferencing a NULL pointer!").arg(XScriptTypeName<T>::Value);
  }
  MissingThisException() {}

public:
  QString const &Buffer() const { return msg; }
  };

#endif // XSCRIPTEXCEPTION_H
