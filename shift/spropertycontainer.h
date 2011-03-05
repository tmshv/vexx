#ifndef SPROPERTYCONTAINER_H
#define SPROPERTYCONTAINER_H

#include "sglobal.h"
#include "sproperty.h"
#include "schange.h"

class SPropertyContainer;

#define S_PROPERTY_CONTAINER(myName, superName) \
  private: \
  static SProperty *create##myName(void *ptr) { return new(ptr) myName(); } \
  template <typename INTERNALMETADATA_CLASS> friend class myName ## InternalMetaData; \
  public: \
  enum { Type = Types::myName }; \
  static const SPropertyInformation * staticTypeInformation(); \
  virtual const SPropertyInformation * typeInformation() const { return staticTypeInformation(); }

#define S_PROPERTY_CONTAINER_EMPTY_DEFINITION(name, parent, saveFn, loadFn) \
  const SPropertyInformation * name::staticTypeInformation() { \
  const SPropertyInformation *parentMetaData = parent::staticTypeInformation(); \
  xAssert(parentMetaData && "Parent MetaData"); \
  static SPropertyInformation info = { name::create##name, \
    name::saveFn, \
    name::loadFn, \
    name::assignContainer, \
    0, \
    #name, \
    name::Type, \
    parentMetaData, \
    parentMetaData->propertyCount+parentMetaData->propertyOffset, \
    0, \
    0, \
    sizeof(name), \
    0, \
    false }; return &info;}


#define S_PROPERTY_CONTAINER_DEFINITION_WITH_COMPUTATION(name, parent) template <typename T> class name ## InternalMetaData { public: static SPropertyInformation *typeInformation() {
#define S_COMPUTE_INPUTS(name) static SProperty SPropertyContainer::* name[] = {
#define S_INPUT(property) reinterpret_cast<SProperty SPropertyContainer::*>(&T :: property),
#define S_COMPUTE_INPUTS_END() 0 };
#define S_PROPERTY_CONTAINER_DEFINITION_CHILDREN(name, parent) xsize count = 0; static SPropertyInformation::Child childMetaData[] = {
#define S_PROPERTY_CONTAINER_DEFINITION(name, parent) \
  S_PROPERTY_CONTAINER_DEFINITION_WITH_COMPUTATION(name, parent) \
  S_PROPERTY_CONTAINER_DEFINITION_CHILDREN(name, parent)
#define S_PROPERTY_DEFINITION(type, name) { type :: staticTypeInformation() + (0*++count), #name, reinterpret_cast<SProperty SPropertyContainer::*>(&T :: name), 0, 0 },
  #define S_COMPUTABLE_PROPERTY_DEFINITION(type, name, computeFn, computeVars) { type :: staticTypeInformation() + (0*++count), #name, reinterpret_cast<SProperty SPropertyContainer::*>(&T :: name), computeFn, computeVars },

#define S_PROPERTY_CONTAINER_END_DEFINITION_PART1(name, parent, saveFn, loadFn) \
  }; \
  const SPropertyInformation *parentMetaData = parent::staticTypeInformation(); \
  xAssert(parentMetaData && "Parent MetaData"); \
  static SPropertyInformation info = { name::create##name, \
    name::saveFn, \
    name::loadFn, \
    name::assignContainer, \
    0, \
    #name, \
    name::Type, \
    parentMetaData, \
    parentMetaData->propertyCount+parentMetaData->propertyOffset, \
    count, \
    childMetaData, \
    sizeof(name), \
    0, \
    false };

#define S_PROPERTY_CONTAINER_END_DEFINITION_PART2(name, parent, saveFn, loadFn) \
  return &info;} }; \
  const SPropertyInformation *name::staticTypeInformation() { return name ## InternalMetaData<name>::typeInformation(); }

#define S_PROPERTY_CONTAINER_END_DEFINITION(name, parent, saveFn, loadFn) \
  S_PROPERTY_CONTAINER_END_DEFINITION_PART1(name, parent, saveFn, loadFn) \
  S_PROPERTY_CONTAINER_END_DEFINITION_PART2(name, parent, saveFn, loadFn)

class SHIFT_EXPORT SPropertyContainer : public SProperty
  {
  S_PROPERTY_CONTAINER(SPropertyContainer, SProperty);

public:
  class TreeChange : public SChange
    {
    S_CHANGE(TreeChange, SChange, 52)
  public:
    TreeChange(SPropertyContainer *b, SPropertyContainer *a, SProperty *ent, xsize index);
    ~TreeChange();
    SPropertyContainer *before() {return _before;}
    const SPropertyContainer *before() const {return _before;}
    SPropertyContainer *after() {return _after;}
    const SPropertyContainer *after() const {return _after;}
    SProperty *property() {return _property;}
    const SProperty *property() const {return _property;}
    xsize index() const { return _index; }
  private:
    SPropertyContainer *_before;
    SPropertyContainer *_after;
    SProperty *_property;
    xsize _index;
    bool _owner;
    bool apply(int mode, SObservers &);
    };

  SPropertyContainer();
  virtual ~SPropertyContainer();

  template <typename T> T *firstChild()
    {
    SProperty *prop = firstChild();
    while(prop)
      {
      T *t = prop->castTo<T>();
      if(t)
        {
        return t;
        }
      prop = prop->nextSibling();
      }
    return 0;
    }

  SProperty *firstChild() const {preGet(); return _child;}

  const SProperty *findChild(const QString &name) const;
  SProperty *findChild(const QString &name);

  xsize size() const;
  bool isChildDynamic(const SProperty *child) const;
  xsize indexOfChild(const SProperty *child) const;

  bool contains(SProperty *) const;

protected:
  // contained implies the property is aggregated by the inheriting class and should not be deleted.
  // you cannot add another contained property once dynamic properties have been added, this bool
  // should really be left alone and not exposed in sub classes
  SProperty *addProperty(xuint32, xsize index=X_SIZE_SENTINEL);
  // move a property from this to newParent
  void moveProperty(SPropertyContainer *newParent, SProperty *property);
  void removeProperty(SProperty *);

  static void assignContainer(const SProperty *, SProperty *);
  static void saveContainer(const SProperty *, SPropertyData &, SPropertyData::Mode);
  static void loadContainer(SProperty *, const SPropertyData &, xuint32, SPropertyData::Mode, SLoader &);

private:
  SProperty *_child;
  xsize _containedProperties;

  void internalInsertProperty(bool contained, SProperty *, xsize index);
  void internalRemoveProperty(SProperty *);

  friend class TreeChange;
  friend class SEntity;
  friend class SDatabase;
  };

#endif // SPROPERTYCONTAINER_H
