#ifndef SPROPERTYINFORMATION_H
#define SPROPERTYINFORMATION_H

#include "QString"
#include "XGlobal"
#include "XProperty"
#include "spropertydata.h"
#include "XHash"
#include "XVariant"

class SProperty;
class SLoader;
class SPropertyContainer;
class SPropertyInformation;


// Child information
class SHIFT_EXPORT SPropertyInstanceInformation
  {
public:
  typedef void (*ComputeFunction)( SPropertyContainer * );

  typedef xuint16 DataKey;
  typedef XHash<DataKey, XVariant> DataHash;

XProperties:
  XROProperty(const SPropertyInformation *, childInformation);
  XRORefProperty(QString, name);
  XROProperty(SProperty SPropertyContainer::*, location);
  XROProperty(ComputeFunction, compute);
  XROProperty(SProperty SPropertyContainer::* *, affects);
  XROProperty(xsize, index);
  XROProperty(bool, entityChild);
  XROProperty(bool, extra);
  XROProperty(bool, dynamic);
  XROProperty(DataHash, data);

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
  virtual void initiateProperty(SProperty *propertyToInitiate) const { }
  static DataKey newDataKey();

private:
  friend class SProperty;
  friend class SPropertyContainer;
  };

class SHIFT_EXPORT SPropertyInformation
  {
public:
  typedef void (*CreateFunction)(void *data, const SPropertyInformation *type, SPropertyInstanceInformation **info);
  typedef void (*SaveFunction)( const SProperty *, SPropertyData &, SPropertyData::Mode );
  typedef void (*LoadFunction)( SProperty *, const SPropertyData &, xuint32, SPropertyData::Mode, SLoader & );
  typedef void (*AssignFunction)( const SProperty *, SProperty * );

  typedef xuint16 DataKey;
  typedef XHash<DataKey, XVariant> DataHash;

XProperties:
  XROProperty(CreateFunction, create);
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
