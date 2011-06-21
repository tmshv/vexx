#ifndef SPROPERTY_H
#define SPROPERTY_H

#include "sglobal.h"
#include "XObject"
#include "schange.h"
#include "spropertyinformation.h"
#include "XFlags"

class SEntity;
class SProperty;
class SPropertyContainer;
class SPropertyMetaData;
class SDatabase;

#define S_REGISTER_TYPE_FUNCTION() \
  private: static const SPropertyInformation *createTypeInformation(); \
  public: static const SPropertyInformation *staticTypeInformation();

#define S_ADD_INSTANCE_INFORMATION(name) const InstanceInformation *instanceInformation() const { return static_cast<const InstanceInformation *>(baseInstanceInformation()); }


#define S_ADD_STATIC_INFO(name, version) \
  static void createProperty(void *ptr, const SPropertyInformation *, SPropertyInstanceInformation **instanceInfo) { \
    name *prop = new(ptr) name(); \
    if(instanceInfo) { \
    *instanceInfo = (SPropertyInstanceInformation *)(prop + 1); \
    new(*instanceInfo) InstanceInformation(); \
    (*instanceInfo)->setDynamic(true); \
    } } \
  public: enum { Type = Types::name, Version = version };

#define S_PROPERTY_ROOT(myName, version) \
  public: \
  S_ADD_STATIC_INFO(myName, version); \
  S_ADD_INSTANCE_INFORMATION(myName) \
  typedef void ParentType; \
  S_REGISTER_TYPE_FUNCTION()

#define S_IMPLEMENT_PROPERTY(myName) \
  const SPropertyInformation *myName::staticTypeInformation() { \
  static const SPropertyInformation *info = 0; \
  if(!info) { info = STypeRegistry::findType(#myName); \
  if(!info) { info = createTypeInformation(); xAssert(info); } } \
  return info;}

#define S_PROPERTY(myName, superName, version) \
  public: \
  S_ADD_STATIC_INFO(myName, version) \
  S_ADD_INSTANCE_INFORMATION(myName) \
  typedef superName ParentType; \
  S_REGISTER_TYPE_FUNCTION()

class SHIFT_EXPORT SProperty
  {
public:
  typedef SPropertyInstanceInformation InstanceInformation;
  S_PROPERTY_ROOT(SProperty, 0)

public:
  SProperty();
  virtual ~SProperty();

  void assign(const SProperty *propToAssign);

  // get the parent entity for this attribute
  // or if this attribute is an entity, get it.
  SEntity *entity() const;

  SPropertyContainer *parent() const {return _parent;}

  SProperty *input() const {return _input;}
  SProperty *output() const {return _output;}
  SProperty *nextOutput() const {return _nextOutput;}

  // connect this property (driver) to the passed property (driven)
  void connect(SProperty *) const;
  void disconnect(SProperty *) const;
  void disconnect() const;

  bool isComputed() const { return baseInstanceInformation()->compute() != 0; }
  bool hasInput() const { return _input; }
  bool hasOutputs() const { preGet(); return _output; }

  template <typename T> T *nextSibling()
    {
    SProperty *prop = nextSibling();
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

  SProperty *nextSibling() const;

  SDatabase *database() { return _database; }
  const SDatabase *database() const { return _database; }

  bool inheritsFromType(const QString &type) const;
  bool inheritsFromType(SPropertyType type) const;
  template <typename T> bool inheritsFromType() const { return inheritsFromType(T::Type); }

  const SPropertyInformation *typeInformation() const { xAssert(_info); return _info; }
  const SPropertyInstanceInformation *baseInstanceInformation() const { xAssert(_instanceInfo); return _instanceInfo; }
  SPropertyType type() const;

  void postSet();
  void preGet() const;

  bool isDynamic() const;
  xsize index() const;

  QString path() const;
  QString path(const SProperty *from) const;

  bool isDescendedFrom(const SProperty *ent) const;
  SProperty *resolvePath(const QString &path);
  const SProperty *resolvePath(const QString &path) const;

  // set only works for dynamic properties
  void setName(const QString &);
  const QString &name() const;

  template <typename T>T *uncheckedCastTo()
    {
    xAssert(castTo<T>());
    return static_cast<T *>(this);
    }
  template <typename T>const T *uncheckedCastTo() const
    {
    xAssert(castTo<T>());
    return static_cast<const T *>(this);
    }

  template <typename T>T *castTo()
    {
    if(inheritsFromType(T::Type))
      {
      return static_cast<T *>(this);
      }
    return 0;
    }
  template <typename T>const T *castTo() const
    {
    if(inheritsFromType(T::Type))
      {
      return static_cast<const T *>(this);
      }
    return 0;
    }

  class DataChange : public SChange
    {
    S_CHANGE(DataChange, SChange, 53)
  public:
    DataChange(SProperty *p) : _property(p) { }
    SProperty *property() {return _property;}
    const SProperty *property() const {return _property;}
  private:
    SProperty *_property;
    };

  class NameChange : public SChange
    {
    S_CHANGE(NameChange, SChange, 50)
  public:
    NameChange(const QString &b, const QString &a, SProperty *ent)
      : _before(b), _after(a), _property(ent)
      { }
    const QString &before() const {return _before;}
    const QString &after() const {return _after;}
    SProperty *property() {return _property;}
    const SProperty *property() const {return _property;}
  private:
    QString _before;
    QString _after;
    SProperty *_property;
    bool apply(int mode);
    };

  class ConnectionChange : public SChange
    {
    S_CHANGE(ConnectionChange, SChange, 51)
  public:
    enum Mode
      {
      Connect,
      Disconnect
      };

    ConnectionChange(Mode m, SProperty *driver, SProperty *driven)
      : _driver(driver), _driven(driven), _mode(m)
      { }
    SProperty *driver() { return _driver; }
    SProperty *driven() { return _driven; }
    const SProperty *driver() const { return _driver; }
    const SProperty *driven() const { return _driven; }
    Mode mode() const { return _mode; }
  private:
    SProperty *_driver;
    SProperty *_driven;
    Mode _mode;
    bool apply(int mode);
    void setParentHasInputConnection(SProperty *);
    void setParentHasOutputConnection(SProperty *);
    void clearParentHasInputConnection(SProperty *);
    void clearParentHasOutputConnection(SProperty *);
    };

  static void assignProperty(const SProperty *, SProperty *);
  static void saveProperty(const SProperty *, SSaver & );
  static SProperty *loadProperty(SPropertyContainer *, SLoader &);

protected:
  template <typename T> T *getChange() const
    {
    return reinterpret_cast<T*>(getChangeMemory(sizeof(T)));
    }

private:
  void setDirty(bool force=false);
  friend void setDependantsDirty(SProperty* prop, bool force=false);
  void internalSetName(const QString &name);

  void connectInternal(SProperty *) const;
  void disconnectInternal(SProperty *) const;
  void *getChangeMemory(size_t size) const;
  SProperty *_nextSibling;
  SProperty *_input;
  SProperty *_output;
  SProperty *_nextOutput;
  SDatabase *_database;
  SPropertyContainer *_parent;
  const SPropertyInformation *_info;
  const InstanceInformation *_instanceInfo;
  mutable SEntity *_entity;

  enum Flags
    {
    Dirty = 1,
    ParentHasInput = 2,
    ParentHasOutput = 4,
    PreGetting = 8
    };
  XFlags<Flags, xuint8> _flags;

  friend class SEntity;
  friend class SDatabase;
  friend class SPropertyContainer;
  friend class SProcessManager;
  };

#endif // SPROPERTY_H
