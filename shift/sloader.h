#ifndef SLOADER_H
#define SLOADER_H

#include "QHash"

class QString;
class SProperty;

class SLoader
  {
public:
  typedef QHash <SProperty*, QString> PostLoadInputHash;
  virtual void resolveInputAfterLoad(SProperty *prop, QString inputPath ) = 0;
  };

#endif // SLOADER_H
