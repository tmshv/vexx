#ifndef SDATABASE_H
#define SDATABASE_H

#include "sglobal.h"
#include "XMap"
#include "sentity.h"
#include "sbaseproperties.h"
#include "XRandomAccessAllocator"
#include "sloader.h"

class SChange;

class SHIFT_EXPORT SDatabase : public SEntity
  {
  S_ENTITY(SDatabase, SEntity, 0);

public:
  UnsignedIntProperty majorVersion;
  UnsignedIntProperty minorVersion;
  UnsignedIntProperty revision;

  SDatabase();
  virtual ~SDatabase();

  void beginBlock();
  void endBlock();

  //void write(const SProperty *, SPropertyData &, SPropertyData::Mode mode) const;
  //SProperty *read(const SPropertyData &, SPropertyContainer *parent, SPropertyData::Mode mode);

  static QString pathSeparator();
  static QString propertySeparator();

  void *allocateChangeMemory(xsize);

  void submitChange(SChange *change);

  SObservers &currentBlockObserverList() { return _blockObservers; }

  bool informingEnabled() const { return _informingEnabled; }
  void setInformingEnabled(bool enable) { _informingEnabled = enable; }

  bool stateStorageEnabled() const { return _stateStorageEnabled; }
  void setStateStorageEnabled(bool enable) { _stateStorageEnabled = enable; }

protected:
  void initiateInheritedDatabaseType(const SPropertyInformation *info);

private:
  SProperty *createDynamicProperty(const SPropertyInformation *info);
  void deleteDynamicProperty(SProperty *);

  void destoryChangeMemory(SChange *);

  template <typename T> static T *createHelper()
    {
    return new T();
    }
  xuint32 _blockLevel;

  void inform();
  SObservers _blockObservers;
  bool _inSubmitChange;
  bool _informingEnabled;
  bool _stateStorageEnabled;

  XList <SChange*> _done;
  InstanceInformation _instanceInfoData;

  void initiateProperty(SProperty *);
  void initiatePropertyFromMetaData(SPropertyContainer *prop, const SPropertyInformation *mD, bool includeParents=true);
  void uninitiateProperty(SProperty *thisProp);
  void uninitiatePropertyFromMetaData(SPropertyContainer *container, const SPropertyInformation *mD);

  XRandomAccessAllocator _memory;

  friend class SProperty;
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
