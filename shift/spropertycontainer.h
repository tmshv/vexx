#ifndef SPROPERTYCONTAINER_H
#define SPROPERTYCONTAINER_H

#include "sglobal.h"
#include "sproperty.h"
#include "schange.h"

class SPropertyContainer;

#define S_PROPERTY_CONTAINER(myName, superName, version) \
  public: \
  S_ADD_STATIC_INFO(myName, version); \
  static const SPropertyInformation * staticTypeInformation(); \
  typedef superName ParentType; \
  S_ADD_INSTANCE_INFORMATION(myName)

#define S_PROPERTY_CONTAINER_EMPTY_DEFINITION(name) \
  const SPropertyInformation * name::staticTypeInformation() { \
  static SPropertyInformation info(SPropertyInformation::create<name>(#name)); \
  return &info;}


#define S_PROPERTY_CONTAINER_DEFINITION(name) \
  const SPropertyInformation *name::staticTypeInformation() \
    { typedef name className; \
    static SPropertyInformation *infoPtr = 0; \
    if(!infoPtr) { \
    xsize index = 0; \
    static SPropertyInformation info(SPropertyInformation::create<name>(#name)); \

#define S_COMPUTE_GROUP(name) static SProperty SPropertyContainer::* name[] = {
#define S_AFFECTS(property) reinterpret_cast<SProperty SPropertyContainer::*>(&className :: property),
#define S_COMPUTE_GROUP_END() 0 };

#define S_PROPERTY_DEFINITION(type, name, ...) \
    static type::InstanceInformation name##InstanceData(type :: staticTypeInformation(), \
                                                        #name, index++, \
                                                        reinterpret_cast<SProperty SPropertyContainer::*>(&className :: name), \
                                                        0, 0, false, false); \
    name##InstanceData.initiateFromDefinition(__VA_ARGS__); \
    info.children() << &name##InstanceData;


#define S_PROPERTY_ENTITY_CHILD_DEFINITION(type, name, ...) \
    static type::InstanceInformation name##InstanceData(type :: staticTypeInformation(), \
                                                        #name, index++, \
                                                        reinterpret_cast<SProperty SPropertyContainer::*>(&className :: name), \
                                                        0, 0, true, false); \
    name##InstanceData.initiateFromDefinition(__VA_ARGS__); \
    info.children() << &name##InstanceData;

#define S_COMPUTABLE_PROPERTY_DEFINITION(type, name, computeFn, computeVars, ...) \
    static type::InstanceInformation name##InstanceData(type :: staticTypeInformation(), \
                                                        #name, index++, \
                                                        reinterpret_cast<SProperty SPropertyContainer::*>(&className :: name), \
                                                        computeFn, computeVars, false, false); \
    name##InstanceData.initiateFromDefinition(__VA_ARGS__); \
    info.children() << &name##InstanceData;

#define S_PROPERTY_CONTAINER_END_DEFINITION(name) \
    infoPtr = &info; } \
    return infoPtr; }

class SHIFT_EXPORT SPropertyContainer : public SProperty
  {
  S_PROPERTY_CONTAINER(SPropertyContainer, SProperty, 0);

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
    bool apply(int);
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

  SProperty *firstChild() const { preGet(); return _child; }

  const SProperty *findChild(const QString &name) const;
  SProperty *findChild(const QString &name);

  xsize size() const;
  xsize containedProperties() const { return _containedProperties; }

  bool contains(SProperty *) const;

  static void assignProperty(const SProperty *, SProperty *);
  static void saveProperty(const SProperty *, SSaver & );
  static SProperty *loadProperty(SPropertyContainer *, SLoader &);

protected:
  // contained implies the property is aggregated by the inheriting class and should not be deleted.
  // you cannot add another contained property once dynamic properties have been added, this bool
  // should really be left alone and not exposed in sub classes
  SProperty *addProperty(xuint32, xsize index=X_SIZE_SENTINEL);
  // move a property from this to newParent
  void moveProperty(SPropertyContainer *newParent, SProperty *property);
  void removeProperty(SProperty *);

  SProperty *at(xsize i);
  const SProperty *at(xsize i) const;

private:
  SProperty *_child;
  xsize _containedProperties;

  void internalInsertProperty(bool contained, SProperty *, xsize index);
  void internalRemoveProperty(SProperty *);

  friend class TreeChange;
  friend class SEntity;
  friend class SProperty;
  friend class SDatabase;
  };

#endif // SPROPERTYCONTAINER_H
