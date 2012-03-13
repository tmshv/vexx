#ifndef XSCRIPTEXCEPTION_H
#define XSCRIPTEXCEPTION_H

#include "QString"
#include "XScriptGlobal.h"

class XScriptObject;

EKSSCRIPT_EXPORT XScriptObject TossAsError( const QString &err );
EKSSCRIPT_EXPORT XScriptObject Toss(const QString &msg);

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
