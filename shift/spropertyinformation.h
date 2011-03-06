#ifndef SPROPERTYINFORMATION_H
#define SPROPERTYINFORMATION_H

#include "QString"
#include "XGlobal"
#include "spropertydata.h"
#include "XHash"
#include "XVariant"

class SProperty;
class SLoader;
class SPropertyContainer;

class SHIFT_EXPORT SPropertyInformation
  {
public:
  typedef SProperty *(*CreateFunction)(void *data);
  typedef void (*SaveFunction)( const SProperty *, SPropertyData &, SPropertyData::Mode );
  typedef void (*LoadFunction)( SProperty *, const SPropertyData &, xuint32, SPropertyData::Mode, SLoader & );
  typedef void (*AssignFunction)( const SProperty *, SProperty * );
  typedef void (*ComputeFunction)( SPropertyContainer * );

  ~SPropertyInformation();

  CreateFunction create;
  SaveFunction save;
  LoadFunction load;
  AssignFunction assign;

  xuint32 version;

  QString typeName;
  xuint32 typeId;

  const SPropertyInformation *parentTypeInformation;


  // Child information
  struct Child
    {
    const SPropertyInformation *childInformation;
    QString name;
    SProperty SPropertyContainer::* location;
    ComputeFunction compute;
    SProperty SPropertyContainer::* *affects;

    typedef xuint16 DataKey;
    XHash<DataKey, XVariant> data;
    };

  xsize propertyOffset;
  xsize propertyCount;
  const Child *childMetaData;

  xsize size;
  xsize instances;
  bool dynamic;

  typedef xuint16 DataKey;
  XHash<DataKey, XVariant> data;

  static DataKey newDataKey();
  static Child::DataKey newChildDataKey();

  const Child *child(xsize index) const;
  };

#endif // SPROPERTYINFORMATION_H
