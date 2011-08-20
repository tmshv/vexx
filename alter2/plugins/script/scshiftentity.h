#ifndef SCSHIFTENTITY_H
#define SCSHIFTENTITY_H

#include "scshiftpropertycontainer.h"

class ScShiftEntity : public ScShiftPropertyContainer
  {
public:
  ScShiftEntity(QScriptEngine *engine);
  ~ScShiftEntity();

  void initiate();

  static QScriptValue addChild(QScriptContext *ctx, QScriptEngine *);
  };

Q_DECLARE_METATYPE(ScShiftEntity*)

#endif // SCSHIFTENTITY_H
