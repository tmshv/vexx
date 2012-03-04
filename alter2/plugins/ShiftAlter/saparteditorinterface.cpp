#include "saparteditorinterface.h"
#include "sentity.h"
#include "sdatabase.h"
#include "QDebug"

PropertyNameEditor::PropertyNameEditor(SProperty *e, SPartEditorInterfaceFeedbacker *feedback, CallbackFn callback)
    : _property(e), _feedback(feedback), _callback(callback)
  {
  setText(e->name());
  xAssert(e->isDynamic());
  connect(this, SIGNAL(editingFinished()), this, SLOT(editName()));

  if(property()->entity())
    {
    property()->entity()->addTreeObserver(this);
    _observedEntity = property()->entity();
    }
  }

PropertyNameEditor::~PropertyNameEditor()
  {
  if(_observedEntity.isValid())
    {
    _observedEntity->removeTreeObserver(this);
    }
  }

void PropertyNameEditor::onTreeChange(const SChange *c)
  {
  const SProperty::NameChange *n = c->castTo<SProperty::NameChange>();
  if(n && n->property() == property())
    {
    setText(n->after());
    }
  }

void PropertyNameEditor::editName()
  {
  if(property() && property()->parent())
    {
    property()->setName(text());
    if(feedback() && callback())
      {
      (feedback()->*callback())();
      }
    }
  }


PropertyTypeEditor::PropertyTypeEditor(SProperty *e, const QStringList &items, SPartEditorInterfaceFeedbacker *f, CallbackFn callback)
   : _property(e), _feedback(f), _callback(callback)
  {
  addItems(items);

  int index = findText(e->typeInformation()->typeName());
  setCurrentIndex(index);


  xAssert(e->isDynamic());
  connect(this, SIGNAL(activated(QString)), this, SLOT(editType(QString)));
  }

PropertyTypeEditor::~PropertyTypeEditor()
  {
  }

void PropertyTypeEditor::editType(const QString &t)
  {
  xAssert(property());
  SBlock b(property()->handler());

  const SPropertyInformation *info = STypeRegistry::findType(t);
  SProperty *parent = property()->parent();
  if(parent && info)
    {
    class Initialiser : public SPropertyInstanceInformationInitialiser
      {
    public:
      void initialise(SPropertyInstanceInformation *i)
        {
        const SPropertyInstanceInformation *oldInfo = oldProp->instanceInformation();
        const xsize *affects = oldInfo->affects();
        if(affects && affects[0] != 0)
          {
          xAssert(affects[1] == 0);
          const SPropertyInstanceInformation *info = oldProp->parent()->typeInformation()->child(affects[0]);
          xAssert(info);
          i->setAffects(info);
          }
        }

      const SProperty *oldProp;
      };

    Initialiser i;
    i.oldProp = _property;

    SProperty *newProp = 0;
    if(parent->inheritsFromType<SEntity>())
      {
      SEntity *entParent = parent->uncheckedCastTo<SEntity>();

      newProp = entParent->addProperty(info, "", &i);
      }
    else if(parent->inheritsFromType<SPropertyArray>())
      {
      SPropertyArray *arrayParent = parent->uncheckedCastTo<SPropertyArray>();

      newProp = arrayParent->add(info, "", &i);
      }

    xAssert(newProp);
    if(newProp)
      {
      QString n = property()->name();
      property()->setName("");

      QVector<SProperty*> outputs;

      SEntity *newCont = newProp->castTo<SEntity>();
      SEntity *oldCont = property()->castTo<SEntity>();
      if(newCont && oldCont)
        {
        for(SProperty *child=oldCont->firstChild(); child; /* nothing */)
          {
          if(child->isDynamic())
            {
            SProperty *next = child->nextSibling();
            oldCont->moveProperty(newCont, child);
            child = next;
            }
          else
            {
            SProperty *prop = newCont->findChild(child->name());
            if(prop)
              {
              child->assign(prop);

              const SProperty *inp = child->input();
              if(inp)
                {
                inp->disconnect(child);
                inp->connect(prop);
                }

              outputs.clear();
              for(SProperty *out=child->output(); out; out=out->nextOutput())
                {
                outputs << out;
                }

              prop->connect(outputs);

              child->disconnect();
              }

            child=child->nextSibling();
            }
          }
        }
      else if(!newProp->inheritsFromType<SPropertyContainer>() && !property()->inheritsFromType<SPropertyContainer>())
        {
        // transfer the value
        property()->assign(newProp);
        }


      // remove the old property
      if(parent->inheritsFromType<SEntity>())
        {
        SEntity *entParent = parent->uncheckedCastTo<SEntity>();

        entParent->removeProperty(property());
        }
      else if(parent->inheritsFromType<SPropertyArray>())
        {
        SPropertyArray *arrayParent = parent->uncheckedCastTo<SPropertyArray>();

        arrayParent->remove(property());
        }

      newProp->setName(n);
      setProperty(newProp);

      if(feedback() && callback())
        {
        (feedback()->*callback())(newProp);
        }
      }
    }
  else
    {
    xAssertFail();
    }
  }

SDefaultPartEditorInterface::SDefaultPartEditorInterface() : SPartEditorInterface(true)
  {
  }

QStringList SDefaultPartEditorInterface::possibleTypes() const
  {
  const SPropertyInformation *info = SEntity::staticTypeInformation();

  QStringList list;
  foreach(const SPropertyInformation *i, STypeRegistry::types())
    {
    if(i->inheritsFromType(info))
      {
      list << i->typeName();
      }
    }

  qSort(list);
  return list;
  }

xsize SDefaultPartEditorInterface::numberOfTypeParameters(SEntity *) const
  {
  return NumberOfTypeParameters;
  }

void SDefaultPartEditorInterface::typeParameter(SPartEditorInterfaceFeedbacker *f, SEntity *e, xsize index, QString& name, QWidget *&widget) const
  {
  if(index == Name)
    {
    name = "Name";
    widget = new PropertyNameEditor(e);
    }
  else if(index == Type)
    {
    name = "Type";

    QStringList types = possibleTypes();
    widget = new PropertyTypeEditor(e, types, f, &SPartEditorInterfaceFeedbacker::refreshAll);
    }
  else
    {
    xAssertFail();
    }
  }

bool SDefaultPartEditorInterface::hasPropertiesSection() const
  {
  return true;
  }

void SDefaultPartEditorInterface::propertiesSectionTitle(QString &n) const
  {
  n = "Properties";
  }

void SDefaultPartEditorInterface::properties(SEntity *p, QStringList &l) const
  {
  SProperty *c = p->firstChild();
  while(c)
    {
    if(c->isDynamic())
      {
      l << c->name();
      }

    c = c->nextSibling();
    }
  }

void *SDefaultPartEditorInterface::findProperty(SEntity *c, const QString &n) const
  {
  return c->findChild(n);
  }

QStringList SDefaultPartEditorInterface::possiblePropertyTypes() const
  {
  QStringList list;
  foreach(const SPropertyInformation *i, STypeRegistry::types())
    {
    list << i->typeName();
    }

  qSort(list);
  return list;
  }

void SDefaultPartEditorInterface::addProperty(SEntity *c) const
  {
  c->addProperty<SProperty>();
  }

void SDefaultPartEditorInterface::removeProperty(SEntity *c, const QString &n) const
  {
  SProperty *p = c->findChild(n);
  xAssert(p);
  if(p)
    {
    c->removeProperty(p);
    }
  }

xsize SDefaultPartEditorInterface::numberOfTypeSubParameters(SEntity *, void *) const
  {
  return NumberOfSubTypeParameters;
  }

void SDefaultPartEditorInterface::typeSubParameter(SPartEditorInterfaceFeedbacker *f, SEntity *, void *prop, xsize i, QString& name, QWidget *&widget) const
  {
  if(i == SubName)
    {
    name = "Name";
    widget = new PropertyNameEditor((SProperty*)prop, f, &SPartEditorInterfaceFeedbacker::propertyNameChanged);
    }
  else if(i == SubType)
    {
    name = "Type";

    QStringList types = possiblePropertyTypes();
    widget = new PropertyTypeEditor((SProperty*)prop, types, f, &SPartEditorInterfaceFeedbacker::propertySubParametersChanged);
    }
  else
    {
    xAssertFail();
    }
  }
