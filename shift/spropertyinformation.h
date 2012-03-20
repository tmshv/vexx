#ifndef SPROPERTYINFORMATION_H
#define SPROPERTYINFORMATION_H

#include "sglobal.h"
#include "QString"
#include "sinterface.h"
#include "XProperty"
#include "XHash"
#include "QVariant"

class SProperty;
class SLoader;
class SSaver;
class SPropertyContainer;
class SPropertyInformation;

namespace ShiftTypePacker
{
inline QVariant pack(void)
  {
  return QVariant();
  }
inline QVariant pack(bool b)
  {
  return QVariant(b);
  }
inline QVariant pack(int i)
  {
  return QVariant(i);
  }
inline void unpack(const QVariant &i, bool *success, QString& out)
  {
  *success = true;
  out =i.toString();
  }
inline void unpack(const QVariant &i, bool *success, int &in)
  {
  in = i.toInt(success);
  }
}

namespace std
{
  template <typename T> class initializer_list;
}

class SPropertyInstanceInformationInitialiser
  {
public:
  virtual void initialise(SPropertyInstanceInformation *) = 0;
  };

class XInterfaceBase;
template <typename T> class XInterface;
template <typename T> XInterfaceBase* initiateAPIInterface(const SPropertyInformation *);

// Child information
class SHIFT_EXPORT SPropertyInstanceInformation
  {
public:
  typedef void (*ComputeFunction)( const SPropertyInstanceInformation *, SPropertyContainer * );
  typedef void (*QueueComputeFunction)( const SPropertyInstanceInformation *, const SPropertyContainer *, SProperty **, xsize &numJobs );

  typedef xuint16 DataKey;
  typedef XHash<DataKey, QVariant> DataHash;

  enum Mode
    {
    Internal,
    InputOutput,
    InternalInput,
    Input,
    Output,
    Computed,
    InternalComputed,
    UserSettable,

    NumberOfModes,

    Default = InputOutput
    };

XProperties:
  XProperty(const SPropertyInformation *, childInformation, setChildInformation);
  XProperty(SPropertyInformation *, holdingTypeInformation, setHoldingTypeInformation);
  XRefProperty(QString, name);
  XProperty(xsize, location, setLocation);
  XROProperty(ComputeFunction, compute);
  XProperty(bool, computeLockedToMainThread, setComputeLockedToMainThread);
  XProperty(QueueComputeFunction, queueCompute, setQueueCompute);
  XROProperty(xsize *, affects);
  // this index is internal to this instance information only
  XProperty(xsize, index, setIndex);
  XProperty(bool, extra, setExtra);

  XROProperty(Mode, mode);

  XProperty(bool, dynamic, setDynamic);
  XRORefProperty(DataHash, data);

  XROProperty(xptrdiff, defaultInput);

public:
  SPropertyInstanceInformation();
  static SPropertyInstanceInformation *allocate(xsize size);
  static void destroy(SPropertyInstanceInformation *);

  void setMode(Mode);
  void setModeString(const QString &);
  bool isDefaultMode() const;
  const QString &modeString() const;

  template <typename T> void setCompute(void (*fn)( const SPropertyInstanceInformation *, T * ))
    {
    SPropertyContainer *c = static_cast<T*>((SPropertyContainer*)0);
    xAssert(c == 0);

    ComputeFunction t = (ComputeFunction)fn;

    setCompute(t);
    }

  void setCompute(ComputeFunction fn);
  void setAffects(const SPropertyInstanceInformation *info);
  void setAffects(const SPropertyInstanceInformation **info, xsize size);
  void setAffects(xsize *affects);

  virtual void setDefaultValue(const QString &) { xAssertFail(); }
  void setDefaultInput(const SPropertyInstanceInformation *info);

  virtual void initiateProperty(SProperty *X_UNUSED(propertyToInitiate)) const;
  static DataKey newDataKey();

  bool isComputed() const { return _compute != 0; }
  bool affectsSiblings() const { return _affects != 0; }

  void setData(DataKey, const QVariant &);

  const SPropertyInstanceInformation *resolvePath(const QString &) const;

  SProperty *locateProperty(SPropertyContainer *parent) const;
  const SProperty *locateProperty(const SPropertyContainer *parent) const;

  X_ALIGNED_OPERATOR_NEW

private:
  void initiate(const SPropertyInformation *info,
                const QString &name,
                xsize index,
                xsize s);

  friend class SProperty;
  friend class SPropertyContainer;
  friend class SPropertyInformation;
  static void defaultQueue(const SPropertyInstanceInformation *, const SPropertyContainer *, SProperty **, xsize &numJobs);
  };

class SHIFT_EXPORT SPropertyInformation
  {
public:
  typedef void (*CreatePropertyFunction)(void *data, const SPropertyInformation *type, SPropertyInstanceInformation **info);
  typedef void (*PostCreateFunction)(void *data);
  typedef void (*PostSetFunction)(SPropertyContainer *cont, SProperty *data);
  typedef void (*SaveFunction)( const SProperty *, SSaver & );
  typedef SProperty *(*LoadFunction)( SPropertyContainer *, SLoader & );
  typedef void (*AssignFunction)( const SProperty *, SProperty * );
  typedef bool (*SaveQueryFunction)( const SProperty * );

  typedef xuint16 DataKey;
  typedef XHash<DataKey, QVariant> DataHash;
  typedef XHash<xuint32, SInterfaceBaseFactory *> InterfaceHash;

XProperties:
  XProperty(CreatePropertyFunction, createProperty, setCreateProperty);
  XProperty(SaveFunction, save, setSave);
  XProperty(LoadFunction, load, setLoad);
  XProperty(SaveQueryFunction, shouldSave, setShouldSave);
  XProperty(SaveQueryFunction, shouldSaveValue, setShouldSaveValue);
  XProperty(AssignFunction, assign, setAssign);
  XProperty(PostCreateFunction, postCreate, setPostCreate);
  XProperty(PostSetFunction, postChildSet, setPostChildSet);

  XProperty(xuint32, version, setVersion);

  XRefProperty(QString, typeName);

  XProperty(const SPropertyInformation *, parentTypeInformation, setParentTypeInformation);

  XRefProperty(XList<SPropertyInstanceInformation*>, children);
  XProperty(xsize, size, setSize);
  XProperty(xsize, instanceInformationSize, setInstanceInformationSize);

  XRORefProperty(DataHash, data);

  XROProperty(xsize, instances);

  XProperty(SPropertyInstanceInformation *, extendedParent, setExtendedParent);

  XROProperty(XInterfaceBase*, apiInterface);

public:
  SPropertyInformation() { }
  static SPropertyInformation *allocate();
  static void destroy(SPropertyInformation *);

  template <typename PropType> static SPropertyInformation *create(const QString &typeName, void postCreate(PropType *) = 0);
  static SPropertyInformation *create(const SPropertyInformation *);

  static DataKey newDataKey();

  template <typename T> bool inheritsFromType() const
    {
    return inheritsFromType(T::staticTypeInformation());
    }

  bool inheritsFromType(const SPropertyInformation *type) const;

  // this classes children count
  xsize childCount() const { return children().size(); }

  template <typename T, typename U>
  typename U::InstanceInformation *child(U T::* location)
    {
    union Convertor
    {
      U T::* in;
      SProperty SPropertyContainer::* out;
    };
    Convertor c;
    c.in = location;

    return static_cast<typename U::InstanceInformation*>(child(c.out));
  }

  template <typename T, typename U>
  const typename U::InstanceInformation *child(U T::* location) const
    {
    union Convertor
    {
      U T::* in;
      SProperty SPropertyContainer::* out;
    };
    Convertor c;
    c.in = location;

    return static_cast<const typename U::InstanceInformation*>(child(c.out));
    }

  // access the properties from offset of member
  SPropertyInstanceInformation *child(SProperty SPropertyContainer::* location);
  const SPropertyInstanceInformation *child(SProperty SPropertyContainer::* location) const;

  SPropertyInstanceInformation *child(xsize location);
  const SPropertyInstanceInformation *child(xsize location) const;

  SPropertyInstanceInformation *childFromName(const QString &);
  const SPropertyInstanceInformation *childFromName(const QString &) const;

  // access child instance information
  SPropertyInstanceInformation *childFromIndex(xsize index);
  const SPropertyInstanceInformation *childFromIndex(xsize index) const;

  SPropertyInformation *findAllocatableBase(xsize &offset);
  const SPropertyInformation *findAllocatableBase(xsize &offset) const;

  void setData(DataKey, const QVariant &);

  // size of the property type, and its instance information
  xsize dynamicSize() const { return size() + instanceInformationSize() + X_ALIGN_BYTE_COUNT; }

  template <typename T, typename U> typename U::InstanceInformation *add(U T::* ptr, const QString &name);
  template <typename T> typename T::InstanceInformation *add(const QString &name);
  template <typename T> typename T::InstanceInformation *add(xsize location, const QString &name);
  SPropertyInstanceInformation *add(const SPropertyInformation *newChildType, xsize location, const QString &name, bool extra);
  SPropertyInstanceInformation *add(const SPropertyInformation *newChildType, const QString &name);

  const SInterfaceBaseFactory *interfaceFactory(xuint32 type) const
    {
    const SInterfaceBaseFactory *fac = 0;
    const SPropertyInformation *info = this;
    while(!fac && info)
      {
      fac = info->_interfaceFactories.value(type, 0);
      info = info->parentTypeInformation();
      }

    return fac;
    }

  SPropertyInformation *extendContainedProperty(SPropertyInstanceInformation *inst);

  template <typename T> void addInterfaceFactory(T *factory) const;
  template <typename PROPTYPE, typename T> void addInheritedInterface() const;
  template <typename T> void addAddonInterface() const;
  template <typename T> void addStaticInterface(T *) const;

  template <typename T> static const SPropertyInformation *findStaticTypeInformation(const char *);

  template <typename Class, void (Class::*Member)()>
  static QVariant wrapAPIMemberFunctionHelper(Class *cls, QVariant *, xuint32 argCount, bool *success, QString *error)
    {
    xAssert(argCount == 0);

    (cls->*Member)();
    return QVariant();
    }

  template <typename Ret, typename Class, Ret (Class::*Member)()>
  static QVariant wrapAPIMemberFunctionHelper(Class *cls, QVariant *, xuint32 argCount, bool *success, QString *error)
    {
    xAssert(argCount == 0);

    return ShiftTypePacker::pack((cls->*Member)());
    }

  template <typename Class, typename T1, void (Class::*Member)(T1)>
  static QVariant wrapAPIMemberFunctionHelper(Class *cls, QVariant *args, xuint32 argCount, bool *success, QString *error)
    {
    xAssert(argCount == 1);

    T1 t1;
    ShiftTypePacker::unpack(args[0], success, t1);
    if(!success)
      {
      return QVariant();
      }

    (cls->*Member)(t1);
    return QVariant();
    }

  template <typename Ret, typename Class, typename T1, Ret (Class::*Member)(T1)>
  static QVariant wrapAPIMemberFunctionHelper(Class *cls, QVariant *args, xuint32 argCount, bool *success, QString *error)
    {
    xAssert(argCount == 1);
    T1 t1;
    ShiftTypePacker::unpack(args[0], success, t1);
    if(!success)
      {
      *error = "Could not extract argument 0, incorrect input type.";
      return QVariant();
      }


    return ShiftTypePacker::pack((cls->*Member)(t1));
    }

  template <typename Ret, typename Class, Ret (Class::*Member)()>
  void addAPIMemberFunction(const QString &str)
    {
    typedef QVariant (*WrappedTypedFunction)(Class *cls, QVariant *v, xuint32 argCount, bool *success, QString *error);

    WrappedTypedFunction tFn = wrapAPIMemberFunctionHelper<Ret, Class, Member>;
    WrappedFunction wFn = (WrappedFunction)tFn;
    addAPIMemberFunction(str, wFn, 0);
    }

  template <typename Class, void (Class::*Member)()>
  void addAPIMemberFunction(const QString &str)
    {
    typedef QVariant (*WrappedTypedFunction)(Class *cls, QVariant *v, xuint32 argCount, bool *success, QString *error);

    WrappedTypedFunction tFn = wrapAPIMemberFunctionHelper<Class, Member>;
    WrappedFunction wFn = (WrappedFunction)tFn;
    addAPIMemberFunction(str, wFn, 0);
    }

  template <typename Ret, typename Class, typename T1, Ret (Class::*Member)(T1)>
  void addAPIMemberFunction(const QString &str)
    {
    typedef QVariant (*WrappedTypedFunction)(Class *cls, QVariant *v, xuint32 argCount, bool *success, QString *error);

    WrappedTypedFunction tFn = wrapAPIMemberFunctionHelper<Ret, Class, T1, Member>;
    WrappedFunction wFn = (WrappedFunction)tFn;
    addAPIMemberFunction(str, wFn, 1);
    }

  template <typename Class, typename T1, void (Class::*Member)(T1)>
  void addAPIMemberFunction(const QString &str)
    {
    typedef QVariant (*WrappedTypedFunction)(Class *cls, QVariant *v, xuint32 argCount, bool *success, QString *error);

    WrappedTypedFunction tFn = wrapAPIMemberFunctionHelper<Class, T1, Member>;
    WrappedFunction wFn = (WrappedFunction)tFn;
    addAPIMemberFunction(str, wFn, 1);
    }

  template <typename T> XInterface<T> *apiInterface();

  X_ALIGNED_OPERATOR_NEW

private:
  X_DISABLE_COPY(SPropertyInformation);

  template <typename T> static SPropertyInformation *initiate(SPropertyInformation *info, const QString &typeName, void postCreate(T *));
  template <typename T> static SPropertyInstanceInformation *createInstanceInformationHelper(const QString &name, xsize index, xsize ptr);

  template <typename PropType> static SPropertyInformation *createNoParent(const QString &typeName, void postCreate(PropType *) = 0);

  void reference() const;
  void dereference() const;

  typedef SPropertyInstanceInformation *(*CreateInstanceInformationFunction)(const QString &name, xsize index, xsize location);
  SPropertyInformation *(*_copy)();
  CreateInstanceInformationFunction _createInstanceInformation;

  mutable InterfaceHash _interfaceFactories;


  friend class SProperty;
  friend class SDatabase;
};

Q_DECLARE_METATYPE(const SPropertyInformation*);

#include "sproperty.h"

namespace
{
template<typename T, bool Abstract = T::IsAbstract> struct CreatePropertyHelper;

template<typename T> struct CreatePropertyHelper<T, true>
  {
  static void run(void *,
                  const SPropertyInformation *,
                  SPropertyInstanceInformation **)
    {
    xAssertFail();
    }
  };

template<typename T> struct CreatePropertyHelper<T, false>
  {
  static void run(void *ptr,
                  const SPropertyInformation *type,
                  SPropertyInstanceInformation **instanceInfo)
    {
    T *prop = new(ptr) T();

    if(instanceInfo)
      {
      xuint8 *alignedPtr = (xuint8*)(prop) + type->size();
      alignedPtr = X_ROUND_TO_ALIGNMENT(xuint8 *, alignedPtr);
      xAssertIsAligned(alignedPtr);
      *instanceInfo = (SPropertyInstanceInformation *)(alignedPtr);
      new(*instanceInfo) T::InstanceInformation();
      (*instanceInfo)->setDynamic(true);
      }
    }
  };
}

template <typename T>
  static SPropertyInstanceInformation *SPropertyInformation::createInstanceInformationHelper(
    const QString &name,
    xsize index,
    xsize location)
  {
  typename T::InstanceInformation* def = (T::InstanceInformation*)SPropertyInstanceInformation::allocate(sizeof(T::InstanceInformation));

  new(def) T::InstanceInformation;

  def->initiate(T::staticTypeInformation(), name, index, location);

  return def;
  }

template <typename PropType> SPropertyInformation *SPropertyInformation::create(const QString &typeName, void postCreate(PropType *))
  {
  SPropertyInformation *info = PropType::ParentType::createTypeInformation();

  info->setParentTypeInformation(PropType::ParentType::staticTypeInformation());
  initiate<PropType>(info, typeName, postCreate);

  return info;
  }

template <typename PropType> SPropertyInformation *SPropertyInformation::createNoParent(const QString &typeName, void postCreate(PropType *))
  {
  SPropertyInformation *info = SPropertyInformation::allocate();

  info->setParentTypeInformation(0);
  initiate<PropType>(info, typeName, postCreate);

  return info;
  }

template <typename PropType> SPropertyInformation *SPropertyInformation::initiate(SPropertyInformation *info, const QString &typeName, void postCreate(PropType *))
  {
  info->setCreateProperty(CreatePropertyHelper<PropType>::run);
  info->setPostCreate(reinterpret_cast<PostCreateFunction>(postCreate));
  info->setSave(PropType::saveProperty);
  info->setLoad(PropType::loadProperty);
  info->setShouldSave(PropType::shouldSaveProperty);
  info->setShouldSaveValue(PropType::shouldSavePropertyValue);
  info->setAssign(PropType::assignProperty);
  info->setPostCreate(reinterpret_cast<PostCreateFunction>(postCreate));
  info->setPostChildSet(PropType::postChildSet);
  info->setVersion(PropType::Version);
  info->setSize(sizeof(PropType));
  info->setInstanceInformationSize(sizeof(typename PropType::InstanceInformation));

  info->_createInstanceInformation = createInstanceInformationHelper<PropType>;
  info->_copy = PropType::createTypeInformation;
  info->_instances = 0;
  info->_extendedParent = 0;

  info->_typeName = typeName;

  info->_apiInterface = initiateAPIInterface<PropType>(info);

  return info;
  }

template <typename T, typename U>
typename U::InstanceInformation *SPropertyInformation::add(U T::* ptr, const QString &name)
  {
  T *u = 0;
  SPropertyContainer *container = static_cast<SPropertyContainer *>(u);
  U *offset = &(u->*ptr);
  xptrdiff location = reinterpret_cast<xsize>(offset) - reinterpret_cast<xsize>(container);
  xAssert(location > 0);

  if(extendedParent())
    {
    location -= extendedParent()->location();
    }

  return add<U>(location, name);
  }

template <typename T> typename T::InstanceInformation *SPropertyInformation::add(const QString &name)
  {
  const SPropertyInformation *newChildType = T::staticTypeInformation();

  SPropertyInstanceInformation *inst = add(newChildType, name);

  return static_cast<typename T::InstanceInformation*>(inst);
  }

template <typename T> typename T::InstanceInformation *SPropertyInformation::add(xsize location, const QString &name)
  {
  const SPropertyInformation *newChildType = T::staticTypeInformation();

  SPropertyInstanceInformation *inst = add(newChildType, location, name, false);

  return static_cast<typename T::InstanceInformation*>(inst);
  }

template <typename T> void SPropertyInformation::addInterfaceFactory(T *factory) const
  {
  xAssert(factory);
  xAssert(T::InterfaceType::InterfaceTypeId != SInterfaceTypes::Invalid);
  _interfaceFactories.insert(T::InterfaceType::InterfaceTypeId, factory);
  SInterfaceBaseFactory *facBase = factory;
  ++facBase->_referenceCount;
  xAssert(interfaceFactory(T::InterfaceType::InterfaceTypeId) == factory);
  }

template <typename PROPTYPE, typename T> void SPropertyInformation::addInheritedInterface() const
  {
  class InheritedInterface : public SInterfaceBaseFactory
    {
    S_INTERFACE_FACTORY_TYPE(T)
  public:
    InheritedInterface() : SInterfaceBaseFactory(true) { }
    virtual SInterfaceBase *classInterface(SProperty *prop)
      {
      return prop->castTo<PROPTYPE>();
      }
    };

  addInterfaceFactory(new InheritedInterface);
  }

template <typename T> void SPropertyInformation::addAddonInterface() const
  {
  class AddonInterface : public SInterfaceBaseFactory
    {
    S_INTERFACE_FACTORY_TYPE(T)
    AddonInterface() : SInterfaceBaseFactory(true) { }
    virtual SInterfaceBase *classInterface(SProperty *prop)
      {
      SProperty::UserData *userData = prop->firstUserData();
      while(userData)
        {
        if(userData->userDataTypeId() == SUserDataTypes::InterfaceUserDataType)
          {
          SInterfaceBase *interfaceBase = static_cast<SInterfaceBase*>(userData);
          if(interfaceBase->interfaceTypeId() == T::InterfaceType::InterfaceTypeId)
            {
            return interfaceBase;
            }
          }
        userData = userData->next();
        }

      // none found, create one and add it.
      T* newInterface = new T(prop);
      prop->addUserData(newInterface);
      return newInterface;
      }
    };

  addInterfaceFactory(new AddonInterface);
  }

template <typename T> void SPropertyInformation::addStaticInterface(T *factory) const
  {
  addInterfaceFactory(factory);
  }

template <typename T> static const SPropertyInformation *SPropertyInformation::findStaticTypeInformation(const char *name)
  {
  static const SPropertyInformation *info = 0;
  if(!info)
    {
    info = STypeRegistry::findType(name);
    xAssert(!info);

    if(!info)
      {
      info = T::createTypeInformation();

      xAssert(info);
      }
    }
  else
    {
    static bool found = STypeRegistry::findType(name);

    XAssert ass(X_CURRENT_CODE_LOCATION, "", "");

    ass X_EXPAND_ARGS(X_ASSERT_VARIABLE, 0, name);

    xAssertMessage(found, "Types should be registered before they are used.", name);
    }
  return info;
  }

#include "spropertyinformationapiutilities.h"

#endif // SPROPERTYINFORMATION_H
