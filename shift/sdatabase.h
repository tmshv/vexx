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


  template <typename CLS, typename ...CLSARGS> void doChange(CLSARGS&&... params)
    {
    bool oldStateStorageEnabled = _stateStorageEnabled;
    setStateStorageEnabled(false);

    int mode = SChange::Forward|SChange::Inform;
    if(!oldStateStorageEnabled)
      {
      CLS change(std::forward<CLSARGS>(params)...);
      ((SChange&)change).apply(mode);
      }
    else
      {
#if X_ASSERTS_ENABLED
      if(_doChange.tryLock())
        {
        _doChange.unlock();
        }
      else
        {
        xAssertFail();
        }
#endif
      QMutexLocker l(&_doChange);
      void *mem = allocateChangeMemory(sizeof(CLS));
      SChange* change = new(mem) CLS(std::forward<CLSARGS>(params)...);

      bool result = change->apply(mode);

      if(result)
        {
        _done << change;
        }
      else
        {
        xAssertFailMessage("Change failed");
        }
      }
    setStateStorageEnabled(oldStateStorageEnabled);
    }

  SObservers &currentBlockObserverList() { return _blockObservers; }

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
  QMutex _doChange;
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
