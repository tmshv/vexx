#ifndef SAPPDATABASE_H
#define SAPPDATABASE_H

#include "saglobal.h"
#include "sdatabase.h"
#include "sentity.h"

class SDocument;
class SPlugin;

class SHIFTALTER_EXPORT SAppDatabase : public SDatabase
  {
  S_ENTITY(SAppDatabase, SDatabase, 0);

XProperties:
  XProperty(SPlugin *, plugin, setPlugin);

public:
  SAppDatabase();

  template <typename T> T *addDocument()
    {
    SDocument *doc = addDocument(T::staticTypeInformation());
    if(doc)
      {
      return doc->uncheckedCastTo<T>();
      }
    return 0;
    }

  SEntity settings;

  SDocument *addDocument(const SPropertyInformation *info);
  };

S_PROPERTY_INTERFACE(SAppDatabase)

#endif // SAPPDATABASE_H
