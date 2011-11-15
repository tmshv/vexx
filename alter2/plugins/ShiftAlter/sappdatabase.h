#ifndef SAPPDATABASE_H
#define SAPPDATABASE_H

#include "saglobal.h"
#include "sdatabase.h"
#include "sentity.h"

class SDocument;

class SHIFTALTER_EXPORT SAppDatabase : public SDatabase
  {
  S_ENTITY(SAppDatabase, SDatabase, 0);

public:
  SEntity documents;
  SEntity settings;

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

  SDocument *addDocument(const SPropertyInformation *info);
  };

#endif // SAPPDATABASE_H
