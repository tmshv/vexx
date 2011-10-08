#ifndef SDATABASE_H
#define SDATABASE_H

#include "sglobal.h"
#include "sentity.h"
#include "sbaseproperties.h"
#include "XBucketAllocator"
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
  ~SDatabase();

  void beginBlock();
  void endBlock();

  static QString pathSeparator();

#ifdef X_CPPOX_SUPPORT
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
      void *mem = changeAllocator()->alloc(sizeof(CLS));
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
#else
#define DO_CHANGE_IMPL(...) { \
    bool oldStateStorageEnabled = _stateStorageEnabled; \
    setStateStorageEnabled(false); \
    int mode = SChange::Forward|SChange::Inform; \
    if(!oldStateStorageEnabled) { \
      CLS change(__VA_ARGS__); \
      ((SChange&)change).apply(mode); \
    }else { \
      QMutexLocker l(&_doChange); \
      void *mem = changeAllocator()->alloc(sizeof(CLS)); \
      SChange* change = new(mem) CLS(__VA_ARGS__); \
      bool result = change->apply(mode); \
      if(result) { \
        _done << change; \
      } else { \
        xAssertFailMessage("Change failed"); \
      } } \
    setStateStorageEnabled(oldStateStorageEnabled); \
    }

  template <typename CLS, typename T0> void doChange(const T0 &t0)
    DO_CHANGE_IMPL(t0)
  template <typename CLS, typename T0, typename T1> void doChange(const T0 &t0, const T1 &t1)
    DO_CHANGE_IMPL(t0, t1)
  template <typename CLS, typename T0, typename T1, typename T2> void doChange(const T0 &t0, const T1 &t1, const T2 &t2)
    DO_CHANGE_IMPL(t0, t1, t2)
  template <typename CLS, typename T0, typename T1, typename T2, typename T3> void doChange(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3)
    DO_CHANGE_IMPL(t0, t1, t2, t3)
#endif

  SObservers &currentBlockObserverList() { return _blockObservers; }

  bool stateStorageEnabled() const { return _stateStorageEnabled; }
  void setStateStorageEnabled(bool enable) { _stateStorageEnabled = enable; }

protected:
  void initiateInheritedDatabaseType(const SPropertyInformation *info);

private:
  SProperty *createDynamicProperty(const SPropertyInformation *info);
  void deleteDynamicProperty(SProperty *);
  void deleteProperty(SProperty *);

  XAllocatorBase *changeAllocator()
    {
    return &_memory;
    }

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

  XBucketAllocator _memory;

  friend class SProperty;
  friend class SPropertyContainer;
  friend class SPropertyContainer::TreeChange;
  };

class SHIFT_EXPORT SBlock
  {
public:
  SBlock(SDatabase *db);
  ~SBlock();

private:
  SDatabase *_db;
  };

#endif // SDATABASE_H
