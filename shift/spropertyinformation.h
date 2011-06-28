#ifndef SPROPERTYINFORMATION_H
#define SPROPERTYINFORMATION_H

#include "QString"
#include "sglobal.h"
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
  XProperty(SProperty SPropertyContainer::*, location, setLocation);
  XProperty(ComputeFunction, compute, setCompute);
  XProperty(QueueComputeFunction, queueCompute, setQueueCompute);
  XROProperty(SProperty SPropertyContainer::* *, affects);
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

  void setAffects(SPropertyInstanceInformation *info);
  void setAffects(SProperty SPropertyContainer::* *affects);

  virtual void initiateProperty(SProperty *X_UNUSED(propertyToInitiate)) const { }
  static DataKey newDataKey();

  void setData(DataKey, const QVariant &);

private:
  void initiate(const SPropertyInformation *info,
                const QString &name,
                xsize index,
                SProperty SPropertyContainer::* location);

  friend class SProperty;
  friend class SPropertyContainer;
  friend class SPropertyInformation;
  static void defaultQueue(const SPropertyInstanceInformation *, const SPropertyContainer *, SProperty **, xsize &numJobs);
  };

class SHIFT_EXPORT SPropertyInformation
  {
public:
  typedef void (*CreateFunction)(void *data, const SPropertyInformation *type, SPropertyInstanceInformation **info);
  typedef SPropertyInstanceInformation *(*CreateInstanceInformationFunction)(const SPropertyInformation *type,
                                                                            const QString &name,
                                                                            xsize index,
                                                                            SProperty SPropertyContainer::* location);
  typedef void (*SaveFunction)( const SProperty *, SSaver & );
  typedef SProperty *(*LoadFunction)( SPropertyContainer *, SLoader & );
  typedef void (*AssignFunction)( const SProperty *, SProperty * );

  typedef xuint16 DataKey;
  typedef XHash<DataKey, QVariant> DataHash;

XProperties:
  XROProperty(CreateFunction, create);
  XROProperty(CreateInstanceInformationFunction, createInstanceInformation);
  XROProperty(SaveFunction, save);
  XROProperty(LoadFunction, load);
  XROProperty(AssignFunction, assign);

  XROProperty(xuint32, version);

  XRORefProperty(QString, typeName);

  XROProperty(const SPropertyInformation *, parentTypeInformation);

  XRefProperty(XList<SPropertyInstanceInformation*>, children);
  XROProperty(xsize, propertyOffset);
  XROProperty(xsize, size);
  XROProperty(xsize, instanceInformationSize);
  XROProperty(xsize, instances);
  XROProperty(bool, dynamic);

  XRORefProperty(DataHash, data);

public:
  template <typename PropType> static SPropertyInformation *create(const QString &typeName)
    {
    return new SPropertyInformation(PropType::createProperty,
                                createInstanceInformation<PropType>,
                                PropType::saveProperty,
                                PropType::loadProperty,
                                PropType::assignProperty,
                                PropType::Version,
                                typeName,
                                PropType::ParentType::staticTypeInformation(),
                                sizeof(PropType),
                                sizeof(typename PropType::InstanceInformation));
    }

  template <typename PropType> static SPropertyInformation *createNoParent(const QString &typeName)
    {
    return new SPropertyInformation(PropType::createProperty,
                                createInstanceInformation<PropType>,
                                PropType::saveProperty,
                                PropType::loadProperty,
                                PropType::assignProperty,
                                PropType::Version,
                                typeName,
                                0,
                                sizeof(PropType),
                                sizeof(typename PropType::InstanceInformation));
    }

  SPropertyInformation(CreateFunction createFn,
                       CreateInstanceInformationFunction createInstanceInfoFn,
                       SaveFunction saveFn,
                       LoadFunction loadFn,
                       AssignFunction assignFn,
                       xuint32 version,
                       const QString &typeName,
                       const SPropertyInformation *parent,
                       xsize size,
                       xsize instanceInformationSize);

  ~SPropertyInformation();

  static DataKey newDataKey();

  template <typename T> bool inheritsFromType() const
    {
    return inheritsFromType(T::Type);
    }

  bool inheritsFromType(const SPropertyInformation *type) const;

  // this classes and all its inherited children count
  xsize completeChildCount() const;
  // this classes children count
  xsize childCount() const { return children().size(); }
  // access child instance information
  const SPropertyInstanceInformation *completeChild(xsize index) const;
  const SPropertyInstanceInformation *child(xsize index) const
    { return children()[index]; };

  // size of the property type, and its instance information
  xsize dynamicSize() const { return size() + instanceInformationSize(); }

  template <typename T, typename U> typename U::InstanceInformation *add(U T::* ptr, const QString &name, typename U::InstanceInformation *def=0)
    {
    if(!def)
      {
      def = new typename U::InstanceInformation;
      }

    def->initiate(U::staticTypeInformation(), name, _children.size(), reinterpret_cast<SProperty SPropertyContainer::*>(ptr));

    _children << def;
    return def;
    }

private:
  template <typename T>
    static SPropertyInstanceInformation *createInstanceInformation(const SPropertyInformation *type,
      const QString &name,
      xsize index,
      SProperty SPropertyContainer::* location)
    {
    typename T::InstanceInformation* def = new typename T::InstanceInformation;
    def->initiate(type, name, index, location);
    return def;
    }

private:
  void reference() const;
  void dereference() const;
  friend class SDatabase;
};


#endif // SPROPERTYINFORMATION_H
