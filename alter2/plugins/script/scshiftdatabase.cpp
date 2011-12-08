#include "scshiftdatabase.h"
#include "sdatabase.h"
#include "styperegistry.h"
#include "scembeddedtypes.h"
#include "Serialisation/sjsonio.h"

SPropertyInstanceInformation::DataKey g_computeKey(SPropertyInstanceInformation::newDataKey());

ScShiftDatabase::ScShiftDatabase(QScriptEngine *eng) : ScShiftPropertyContainerBase(eng)
  {
  addMemberFunction("addType", addType);
  addMemberFunction("load", load);
  addMemberFunction("save", save);
  initiateGlobalValue<ScShiftDatabase>("SDatabase", "SEntity");
  }

ScShiftDatabase::~ScShiftDatabase()
  {
  }

QScriptValue ScShiftDatabase::load(QScriptContext *ctx, QScriptEngine *)
  {
  enum
    {
    SaverType = 0,
    Device = 1,
    Parent = 2
    };

  ScProfileFunction
  SProperty **propPtr = getThis(ctx);
  SDatabase *db = 0;
  if(propPtr)
    {
    db = (*propPtr)->uncheckedCastTo<SDatabase>();
    }
  if(!db)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SDatabase.save(...);");
    return false;
    }

  QString saverType = ctx->argument(SaverType).toString();

  QObject *deviceObject = ctx->argument(Device).toQObject();
  if(!deviceObject)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect device argument to SDatabase.save(...);");
    return false;
    }

  QIODevice *device = qobject_cast<QIODevice*>(deviceObject);
  if(!device || !device->isReadable())
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect device argument to SDatabase.save(...);");
    return false;
    }

  SProperty *prop = *unpackValue(ctx->argument(Parent));

  if(!prop)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect entity argument to SDatabase.save(...);");
    return false;
    }

  SPropertyContainer *ent = prop->castTo<SPropertyContainer>();

  if(!ent)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect entity argument to SDatabase.save(...);");
    return false;
    }

  if(saverType == "json")
    {
    SJSONLoader s;

    SProperty *p = ent->firstChild();
    while(p && p->nextSibling())
      {
      p = p->nextSibling();
      }

    s.readFromDevice(device, ent);

    if(!p)
      {
      p = ent->firstChild();
      }

    if(p && p->nextSibling()) // array return
      {
      QScriptValue v;

      SProperty *c = p;
      int i = 0;
      while(c)
        {
        v.setProperty(i++, ScEmbeddedTypes::packValue(c));
        c = c->nextSibling();
        }

      return v;
      }
    else if(p) // one property
      {
      return ScEmbeddedTypes::packValue(p);
      }

    return QScriptValue();
    }


  ctx->throwError(QScriptContext::SyntaxError, "Incorrect saver type argument " + saverType + "SDatabase.save(...);");
  return false;
  }

QScriptValue ScShiftDatabase::save(QScriptContext *ctx, QScriptEngine *)
  {
  enum
    {
    SaverType = 0,
    Device = 1,
    Entity = 2,
    OptReadable = 3,
    OptIncludeRoot = 4
    };

  ScProfileFunction
  SProperty **propPtr = getThis(ctx);
  SDatabase *db = 0;
  if(propPtr)
    {
    db = (*propPtr)->uncheckedCastTo<SDatabase>();
    }
  if(!db)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SDatabase.save(...);");
    return false;
    }

  QString saverType = ctx->argument(SaverType).toString();

  QObject *deviceObject = ctx->argument(Device).toQObject();
  if(!deviceObject)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect device argument to SDatabase.save(...);");
    return false;
    }

  QIODevice *device = qobject_cast<QIODevice*>(deviceObject);
  if(!device || !device->isWritable())
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect device argument to SDatabase.save(...);");
    return false;
    }

  SProperty *prop = *unpackValue(ctx->argument(Entity));

  if(!prop)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect entity argument to SDatabase.save(...);");
    return false;
    }

  SPropertyContainer *ent = prop->castTo<SPropertyContainer>();

  if(!ent)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect entity argument to SDatabase.save(...);");
    return false;
    }

  bool readable = true;
  QScriptValue readableValue = ctx->argument(OptReadable);
  if(readableValue.isBool())
    {
    readable = readableValue.toBool();
    }

  bool includeRoot = true;
  QScriptValue includeRootValue = ctx->argument(OptIncludeRoot);
  if(includeRootValue.isBool())
    {
    includeRoot = includeRootValue.toBool();
    }

  if(saverType == "json")
    {
    SJSONSaver s;
    s.setAutoWhitespace(readable);

    s.writeToDevice(device, ent, includeRoot);
    return true;
    }


  ctx->throwError(QScriptContext::SyntaxError, "Incorrect saver type argument " + saverType + "SDatabase.save(...);");
  return false;
  }

QScriptValue ScShiftDatabase::addType(QScriptContext *ctx, QScriptEngine *engine)
  {
  ScProfileFunction
  SProperty **propPtr = getThis(ctx);
  SDatabase *db = 0;
  if(propPtr)
    {
    db = (*propPtr)->uncheckedCastTo<SDatabase>();
    }
  if(!db)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SDatabase.addType(...);");
    }

  if(ctx->argumentCount() != 1)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect number of arguments to SDatabase.addType(...);");
    return QScriptValue();
    }

  QScriptValue typeObject = ctx->argument(0);
  if(!typeObject.isObject())
    {
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect argument type to SDatabase.addType(...); expected object");
    return QScriptValue();
    }

  QScriptValue tempObject;

  tempObject = typeObject.property("name");
  QString name = tempObject.toString();
  if(name == "" || STypeRegistry::findType(name) != 0)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Unique, non zero-lengthed string expected as 'name' property");
    return QScriptValue();
    }

  tempObject = typeObject.property("parent");
  const SPropertyInformation *parent = 0;
  if(tempObject.isString())
    {
    parent = STypeRegistry::findType(tempObject.toString());
    }
  else if(tempObject.isObject())
    {
    QScriptValue typeName = tempObject.property("typeName");
    if(typeName.isString())
      {
      parent = STypeRegistry::findType(typeName.toString());
      }
    }
  if(parent == 0)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Defined property type expected as 'parent' property");
    return QScriptValue();
    }

  tempObject = typeObject.property("version");
  xuint32 version = tempObject.toUInt32();

  xsize endOfUsedMemory = parent->dynamicSize();
  QList<SPropertyInstanceInformation*> properties;

  tempObject = typeObject.property("properties");
  if(tempObject.isArray())
    {
    xuint32 i = 0;
    QScriptValue val = tempObject.property(i);
    QScriptValue tempArrayObject;
    while(val.isObject())
      {
      tempArrayObject = val.property("name");
      if(!tempArrayObject.isString())
        {
        ctx->throwError(QScriptContext::SyntaxError, "String expected as 'properties' array 'name' entry");
        break;
        }
      QString propName = tempArrayObject.toString();

      tempArrayObject = val.property("type");
      const SPropertyInformation *propType = 0;
      if(tempArrayObject.isString())
        {
        propType = STypeRegistry::findType(tempArrayObject.toString());
        }
      else if(tempArrayObject.isObject())
        {
        QScriptValue typeName = tempArrayObject.property("typeName");
        if(typeName.isString())
          {
          propType = STypeRegistry::findType(typeName.toString());
          }
        }

      if(!propType)
        {
        ctx->throwError(QScriptContext::SyntaxError, "String expected as 'properties' array 'type' entry");
        break;
        }

      SPropertyInstanceInformation::ComputeFunction computeFn = 0;
      tempArrayObject = val.property("compute");
      if(tempArrayObject.isFunction())
        {
        computeFn = computeNode;
        }

      SPropertyInstanceInformation *info = propType->createInstanceInformation()(propType, propName, i, *reinterpret_cast<SProperty SPropertyContainer::**>(&endOfUsedMemory));
      info->setCompute(computeFn);
      info->setExtra(true);

      endOfUsedMemory += propType->size();
      properties << info;

      if(computeFn)
        {
        info->setData(g_computeKey, qVariantFromValue(tempArrayObject));
        }

      val = tempObject.property(++i);
      }

    i = 0;
    val = tempObject.property(i);
    while(val.isObject())
      {
      xsize *affects = 0;
      tempArrayObject = val.property("affects");
      bool affectsIsValid = false;
      if(tempArrayObject.isArray())
        {
        affectsIsValid = true;
        xuint32 length = tempArrayObject.property("length").toUInt32();
        affects = new xsize [length+1];
        affects[length] = 0;
        for(xuint32 i=0; i<length; ++i)
          {
          affects[i] = 0;
          QString name = tempArrayObject.property(i).toString();

          for(int propIdx=0, s=properties.size(); propIdx<s; ++propIdx)
            {
            if(name == properties[propIdx]->name())
              {
              affects[i] = properties[propIdx]->location();
              break;
              }
            }

          if(!affects[i])
            {
            affectsIsValid = false;
            ctx->throwError(QScriptContext::SyntaxError, "Defined sibling property name expected for affectedBy members.");
            break;
            }
          }
        }

      if(!affectsIsValid)
        {
        delete [] affects;
        affects = 0;
        }


      SPropertyInstanceInformation *info = properties[i];
      info->setAffects(affects);


      val = tempObject.property(++i);
      }
    }

  SPropertyInformation *newType = new SPropertyInformation(*parent);

  newType->setVersion(version);
  newType->typeName() = name;
  newType->setParentTypeInformation(parent);
  newType->setSize(endOfUsedMemory);

  newType->children() << parent->children() << properties;

  tempObject = typeObject.property("prototype");
  if(tempObject.isObject())
    {
    QScriptValue g = engine->globalObject();
    QScriptValue parentObj = g.property(parent->typeName());
    xAssert(parentObj.isObject());
    if(parentObj.isObject())
      {
      tempObject.setPrototype(parentObj);
      tempObject.setProperty("typeName", name);
      g.setProperty(name, tempObject);
      }
    }

  STypeRegistry::addType(newType);

  return QScriptValue();
  }

void ScShiftDatabase::computeNode(const SPropertyInstanceInformation *instanceInfo, SPropertyContainer *node)
  {
  ScProfileFunction
  const QVariant &val = instanceInfo->data()[g_computeKey];
  QScriptValue compute = qvariant_cast<QScriptValue>(val);
  xAssert(compute.isFunction());

  // call with this as the node being computed, and no arguments.
  compute.call(ScEmbeddedTypes::packValue(node));
  }
