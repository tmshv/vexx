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
  XProperty(SProperty SPropertyContainer::* *, affects, setAffects);
  // this index is internal to this instance information only
  XProperty(xsize, index, setIndex);
  XProperty(bool, entityChild, setEntityChild);
  XProperty(bool, extra, setExtra);
  XProperty(bool, dynamic, setDynamic);
  XRORefProperty(DataHash, data);

public:
  // extra properties indicate that whilst they are contained within the type itself, the constuctor does not
  // initiate or destroy them, and that the Database should handle this.
  SPropertyInstanceInformation(const SPropertyInformation *info,
                   const QString &name,
                   xsize index,
                   SProperty SPropertyContainer::* location,
                   ComputeFunction computeFn,
                   SProperty SPropertyContainer::* *affects,
                   bool entityChild,
                   bool extra);

  SPropertyInstanceInformation(bool dynamic=true);

  void initiateFromDefinition() { }
  virtual void initiateProperty(SProperty *X_UNUSED(propertyToInitiate)) const { }
  static DataKey newDataKey();

  void setData(DataKey, const QVariant &);

private:
  friend class SProperty;
  friend class SPropertyContainer;

  static void defaultQueue(const SPropertyInstanceInformation *, const SPropertyContainer *, SProperty **, xsize &numJobs);
  };

class SHIFT_EXPORT SPropertyInformation
  {
public:
  typedef void (*CreateFunction)(void *data, const SPropertyInformation *type, SPropertyInstanceInformation **info);
  typedef SPropertyInstanceInformation *(*CreateInstanceInformationFunction)(const SPropertyInformation *type,
                                                                            const QString &name,
                                                                            xsize index,
                                                                            SProperty SPropertyContainer::* location,
                                                                            SPropertyInstanceInformation::ComputeFunction computeFn,
                                                                            SProperty SPropertyContainer::* *affects,
                                                                            bool entityChild,
                                                                            bool extra);
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
  XROProperty(xuint32, typeId);

  XROProperty(const SPropertyInformation *, parentTypeInformation);

  XRORefProperty(XList<SPropertyInstanceInformation*>, children);
  XROProperty(xsize, propertyOffset);
  XROProperty(xsize, size);
  XROProperty(xsize, instanceInformationSize);
  XROProperty(xsize, instances);
  XROProperty(bool, dynamic);

  XRORefProperty(DataHash, data);

public:
  SPropertyInformation(CreateFunction createFn,
                       CreateInstanceInformationFunction createInstanceInfoFn,
                       SaveFunction saveFn,
                       LoadFunction loadFn,
                       AssignFunction assignFn,
                       xuint32 version,
                       const QString &typeName,
                       xuint32 typeId,
                       const SPropertyInformation *parent,
                       const XList<SPropertyInstanceInformation*> children,
                       xsize size,
                       xsize instanceInformationSize);

  SPropertyInformation(CreateFunction createFn,
                       CreateInstanceInformationFunction createInstanceInfoFn,
                       SaveFunction saveFn,
                       LoadFunction loadFn,
                       AssignFunction assignFn,
                       xuint32 version,
                       const QString &typeName,
                       const SPropertyInformation *parent,
                       const XList<SPropertyInstanceInformation*> children,
                       xsize size,
                       xsize instanceInformationSize);

  ~SPropertyInformation();

  static DataKey newDataKey();

  template <typename T> bool inheritsFromType() const
    {
    return inheritsFromType(T::Type);
    }

  bool inheritsFromType(SPropertyType type) const;

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

private:
  void reference() const;
  void dereference() const;
  friend class SDatabase;
  };

#endif // SPROPERTYINFORMATION_H
