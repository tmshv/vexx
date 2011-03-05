#ifndef SDATABASE_H
#define SDATABASE_H

#include "sglobal.h"
#include "XMap"
#include "sentity.h"
#include "sbaseproperties.h"
#include "spropertydata.h"
#include "XRandomAccessAllocator"
#include "sloader.h"

class SChange;

class SHIFT_EXPORT SDatabase : public SEntity, private SLoader
  {
  S_ENTITY(SDatabase, SEntity);

public:
  UnsignedIntProperty majorVersion;
  UnsignedIntProperty minorVersion;
  UnsignedIntProperty revision;

  SDatabase();
  virtual ~SDatabase();

  template <typename T> void addType()
    {
    SPropertyType id = static_cast<T*>(0)->Type;
    if(!_types.contains(id))
      {
      const SPropertyInformation *info = T::staticTypeInformation();
      xAssert(info->assign);
      xAssert(info->save);
      xAssert(info->load);
      xAssert(info->create);
      xAssert(info->typeName.length());
      _types.insert(id, info);
      }
    else
      {
      xAssertFailMessage("Registering a type twice? or duplicate IDs")
      }
    }

  void beginBlock();
  void endBlock();

  void write(const SProperty *, SPropertyData &, SPropertyData::Mode mode) const;
  SProperty *read(const SPropertyData &, SPropertyContainer *parent, SPropertyData::Mode mode);

  static QString pathSeparator();
  static QString propertySeparator();

  void *allocateChangeMemory(xsize);
  void destoryChangeMemory(xsize, void *);

  void submitChange(SChange *change);

  const SPropertyInformation *findType(const QString &) const;
  const SPropertyInformation *findType(xuint32) const;

protected:
  SDatabase(const SPropertyInformation *metaData);

private:
  SProperty *createProperty(xuint32);
  void deleteProperty(SProperty *);

  template <typename T> static T *createHelper()
    {
    return new T();
    }
  XMap <SPropertyType, const SPropertyInformation *> _types;
  xuint32 _blockLevel;

  void initiate();

  void inform();
  SObservers _currentObservers;
  SObservers _blockObservers;
  bool _inSubmitChange;

  XList <SChange*> _done;

  void initiateProperty(SProperty *);
  void initiatePropertyFromMetaData(SPropertyContainer *prop, const SPropertyInformation *mD);
  void uninitiateProperty(SProperty *thisProp);
  void uninitiatePropertyFromMetaData(SPropertyContainer *container, const SPropertyInformation *mD);

  virtual void resolveInputAfterLoad(SProperty *prop, QString inputPath);
  PostLoadInputHash _resolveAfterLoad;
  xsize _readLevel;
  XRandomAccessAllocator _properties;

  friend class SPropertyContainer;
  friend class SPropertyContainer::TreeChange;
  };

class SBlock
  {
public:
  SBlock(SDatabase *db);
  ~SBlock();

private:
  SDatabase *_db;
  };

#endif // SDATABASE_H
