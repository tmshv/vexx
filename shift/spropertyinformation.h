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

namespace std
{
  template <typename T> class initializer_list;
}

// Child information
class SHIFT_EXPORT SPropertyInstanceInformation
  {
public:
  typedef void (*ComputeFunction)( const SPropertyInstanceInformation *, SPropertyContainer * );
  typedef void (*QueueComputeFunction)( const SPropertyInstanceInformation *, const SPropertyContainer *, SProperty **, xsize &numJobs );

  typedef xuint16 DataKey;
  typedef XHash<DataKey, QVariant> DataHash;

XProperties:
  XProperty(const SPropertyInformation *, childInformation, setChildInformation);
  XRefProperty(QString, name);
  XProperty(xsize, location, setLocation);
  XProperty(ComputeFunction, compute, setCompute);
  XProperty(bool, computeLockedToMainThread, setComputeLockedToMainThread);
  XProperty(QueueComputeFunction, queueCompute, setQueueCompute);
  XROProperty(xsize *, affects);
  // this index is internal to this instance information only
  XProperty(xsize, index, setIndex);
  XProperty(bool, entityChild, setEntityChild);
  XProperty(bool, extra, setExtra);

  XProperty(bool, dynamic, setDynamic);
  XRORefProperty(DataHash, data);

public:
  // extra properties indicate that whilst they are contained within the type itself, the constuctor does not
  // initiate or destroy them, and that the Database should handle this.
  SPropertyInstanceInformation();
  virtual ~SPropertyInstanceInformation();

  void setAffects(const SPropertyInstanceInformation *info);
  void setAffects(xsize *affects);

  virtual void initiateProperty(SProperty *X_UNUSED(propertyToInitiate)) const { }
  static DataKey newDataKey();

  bool isComputed() const { return _compute != 0; }
  bool affectsSiblings() const { return _affects != 0; }

  void setData(DataKey, const QVariant &);

  SProperty *locateProperty(SPropertyContainer *parent) const
    {
    xuint8* parentOffset = reinterpret_cast<xuint8*>(parent);
    xuint8* childOffset = parentOffset + location();
    SProperty *child = reinterpret_cast<SProperty*>(childOffset);
    return child;
    }

  const SProperty *locateProperty(const SPropertyContainer *parent) const
    {
    const xuint8* parentOffset = reinterpret_cast<const xuint8*>(parent);
    const xuint8* childOffset = parentOffset + location();
    const SProperty *child = reinterpret_cast<const SProperty*>(childOffset);
    return child;
    }

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
  typedef void (*CreateFunction)(void *data, const SPropertyInformation *type, SPropertyInstanceInformation **info);
  typedef void (*PostCreateFunction)(void *data);
  typedef void (*PostSetFunction)(SPropertyContainer *cont, SProperty *data);
  typedef SPropertyInstanceInformation *(*CreateInstanceInformationFunction)(const SPropertyInformation *type,
                                                                            const QString &name,
                                                                            xsize index,
                                                                            SProperty SPropertyContainer::* location);
  typedef void (*SaveFunction)( const SProperty *, SSaver & );
  typedef SProperty *(*LoadFunction)( SPropertyContainer *, SLoader & );
  typedef void (*AssignFunction)( const SProperty *, SProperty * );
  typedef bool (*SaveQueryFunction)( const SProperty * );

  typedef xuint16 DataKey;
  typedef XHash<DataKey, QVariant> DataHash;
  typedef XHash<xuint32, SInterfaceBaseFactory *> InterfaceHash;

XProperties:
  XProperty(CreateFunction, create, setCreate);
  XProperty(CreateInstanceInformationFunction, createInstanceInformation, setCreateInstanceInformation);
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
  XProperty(bool, dynamic, seyDynamic);

  XRORefProperty(DataHash, data);

  XROProperty(xsize, instances);

public:
  template <typename PropType> static SPropertyInformation *create(const QString &typeName, void postCreate(PropType *) = 0);
  template <typename PropType> static SPropertyInformation *createNoParent(const QString &typeName, void postCreate(PropType *) = 0);

  SPropertyInformation();
  SPropertyInformation(const SPropertyInformation &info);

  ~SPropertyInformation();

  static DataKey newDataKey();

  template <typename T> bool inheritsFromType() const
    {
    return inheritsFromType(T::Type);
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

  // access child instance information
  SPropertyInstanceInformation *childFromIndex(xsize index);
  const SPropertyInstanceInformation *childFromIndex(xsize index) const;

  // size of the property type, and its instance information
  xsize dynamicSize() const { return size() + instanceInformationSize() + X_ALIGN_BYTE_COUNT; }

  template <typename T, typename U> typename U::InstanceInformation *add(U T::* ptr, const QString &name, typename U::InstanceInformation *def=0);

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

  template <typename T> void addInterfaceFactory(T *factory) const;
  template <typename PROPTYPE, typename T> void addInheritedInterface() const;
  template <typename T> void addAddonInterface() const;
  template <typename T> void addStaticInterface(T *) const;


  X_ALIGNED_OPERATOR_NEW

private:
  template <typename T>
    static SPropertyInstanceInformation *createInstanceInformation(const SPropertyInformation *type,
      const QString &name,
      xsize index,
      SProperty SPropertyContainer::* ptr)
    {
    typename T::InstanceInformation* def = new typename T::InstanceInformation;

    SPropertyContainer *u = 0;
    SProperty *offset = &(u->*ptr);
    xptrdiff location = reinterpret_cast<xsize>(offset);
    xAssert(location > 0);

    def->initiate(type, name, index, location);
    return def;
    }

  void reference() const;
  void dereference() const;

  mutable InterfaceHash _interfaceFactories;

  friend class SDatabase;
};

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
                  const SPropertyInformation *,
                  SPropertyInstanceInformation **instanceInfo)
    {
    T *prop = new(ptr) T();

    if(instanceInfo)
      {
      xuint8 *alignedPtr = (xuint8*)(prop+1);
      alignedPtr = X_ROUND_TO_ALIGNMENT(xuint8 *, alignedPtr);
      xAssertIsAligned(alignedPtr);
      *instanceInfo = (SPropertyInstanceInformation *)(alignedPtr);
      new(*instanceInfo) T::InstanceInformation();
      (*instanceInfo)->setDynamic(true);
      }
    }
  };
}

template <typename PropType> SPropertyInformation *SPropertyInformation::create(const QString &typeName, void postCreate(PropType *))
  {
  SPropertyInformation *info = PropType::ParentType::createTypeInformation();

  info->setCreate(CreatePropertyHelper<PropType>::run);
  info->setPostCreate(reinterpret_cast<PostCreateFunction>(postCreate));
  info->setCreateInstanceInformation(createInstanceInformation<PropType>);
  info->setSave(PropType::saveProperty);
  info->setLoad(PropType::loadProperty);
  info->setShouldSave(PropType::shouldSaveProperty);
  info->setShouldSaveValue(PropType::shouldSavePropertyValue);
  info->setAssign(PropType::assignProperty);
  info->setPostCreate(reinterpret_cast<PostCreateFunction>(postCreate));
  info->setPostChildSet(PropType::postChildSet);
  info->setVersion(PropType::Version);
  info->typeName() = typeName;
  info->setParentTypeInformation(PropType::ParentType::staticTypeInformation());
  info->setSize(sizeof(PropType));
  info->setInstanceInformationSize(sizeof(typename PropType::InstanceInformation));

  return info;
  }

template <typename PropType> SPropertyInformation *SPropertyInformation::createNoParent(const QString &typeName, void postCreate(PropType *))
  {
  SPropertyInformation *info = new SPropertyInformation();

  info->setCreate(CreatePropertyHelper<PropType>::run);
  info->setPostCreate(reinterpret_cast<PostCreateFunction>(postCreate));
  info->setCreateInstanceInformation(createInstanceInformation<PropType>);
  info->setSave(PropType::saveProperty);
  info->setLoad(PropType::loadProperty);
  info->setShouldSave(PropType::shouldSaveProperty);
  info->setShouldSaveValue(PropType::shouldSavePropertyValue);
  info->setAssign(PropType::assignProperty);
  info->setPostCreate(reinterpret_cast<PostCreateFunction>(postCreate));
  info->setPostCreate(0);
  info->setVersion(PropType::Version);
  info->typeName() = typeName;
  info->setSize(sizeof(PropType));
  info->setInstanceInformationSize(sizeof(typename PropType::InstanceInformation));

  return info;
  }

template <typename T, typename U>
typename U::InstanceInformation *SPropertyInformation::add(U T::* ptr,
                                                           const QString &name,
                                                           typename U::InstanceInformation *def)
  {
  if(!def)
    {
    def = new typename U::InstanceInformation;
    }

  T *u = 0;
  SPropertyContainer *container = static_cast<SPropertyContainer *>(u);
  U *offset = &(u->*ptr);
  xptrdiff location = reinterpret_cast<xsize>(offset) - reinterpret_cast<xsize>(container);
  xAssert(location > 0);

  def->initiate(U::staticTypeInformation(), name, _children.size(), location);

  _children << def;
  return def;
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

#endif // SPROPERTYINFORMATION_H
