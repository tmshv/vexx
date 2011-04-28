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
  S_ENTITY(SDatabase, SEntity, 0);

public:
  UnsignedIntProperty majorVersion;
  UnsignedIntProperty minorVersion;
  UnsignedIntProperty revision;

  SDatabase();
  virtual ~SDatabase();

  const XHash <SPropertyType, const SPropertyInformation *> &types() const { return _types; }
  void addType(const SPropertyInformation *);

  template <typename T> void addType()
    {
    SPropertyType id = static_cast<T*>(0)->Type;
    if(!_types.contains(id))
      {
      const SPropertyInformation *info = T::staticTypeInformation();
      xAssert(info->assign());
      xAssert(info->save());
      xAssert(info->load());
      xAssert(info->create());
      xAssert(info->typeName().length());
      _types.insert(id, info);
      }
    else
      {
      qDebug() << "Registering a type twice? or duplicate IDs. Already got type '" << _types[id]->typeName() << "', trying to register '" << T::staticTypeInformation()->typeName() << "'";
      xAssertFail();
      }
    }

  void beginBlock();
  void endBlock();

  void write(const SProperty *, SPropertyData &, SPropertyData::Mode mode) const;
  SProperty *read(const SPropertyData &, SPropertyContainer *parent, SPropertyData::Mode mode);

  static QString pathSeparator();
  static QString propertySeparator();

  void *allocateChangeMemory(xsize);

  void submitChange(SChange *change);

  const SPropertyInformation *findType(const QString &) const;
  const SPropertyInformation *findType(xuint32) const;

protected:
  void initiateInheritedDatabaseType(const SPropertyInformation *info);

private:
  SProperty *createDynamicProperty(xuint32);
  void deleteDynamicProperty(SProperty *);

  void destoryChangeMemory(SChange *);

  template <typename T> static T *createHelper()
    {
    return new T();
    }
  XHash <SPropertyType, const SPropertyInformation *> _types;
  xuint32 _blockLevel;

  void initiate();

  void inform();
  SObservers _currentObservers;
  SObservers _blockObservers;
  bool _inSubmitChange;

  XList <SChange*> _done;
  InstanceInformation _instanceInfoData;

  void initiateProperty(SProperty *);
  void initiatePropertyFromMetaData(SPropertyContainer *prop, const SPropertyInformation *mD, bool includeParents=true);
  void uninitiateProperty(SProperty *thisProp);
  void uninitiatePropertyFromMetaData(SPropertyContainer *container, const SPropertyInformation *mD);

  virtual void resolveInputAfterLoad(SProperty *prop, QString inputPath);
  PostLoadInputHash _resolveAfterLoad;
  xsize _readLevel;
  XRandomAccessAllocator _memory;

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
