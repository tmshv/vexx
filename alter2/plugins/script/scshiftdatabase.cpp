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

bool ScShiftDatabase::parseChildProperties(QScriptContext *ctx, SPropertyInformation *newType, QScriptValue propertiesArray)
  {
  if(propertiesArray.isArray())
    {
    // First pass, add basic instance information
    xuint32 i = 0;
    for(QScriptValue propertyObject = propertiesArray.property(0);
        propertyObject.isObject();
        propertyObject = propertiesArray.property(++i))
      {
      // Name
      QScriptValue nameObject = propertyObject.property("name");
      if(!nameObject.isString())
        {
        ctx->throwError(QScriptContext::SyntaxError, "String expected as 'properties' array 'name' entry");
        return false;
        }
      QString propName = nameObject.toString();

      // Extending a current child?
      QScriptValue extendingObject = propertyObject.property("extend");
      if(extendingObject.isBoolean() && extendingObject.toBoolean())
        {
        // Type
        QScriptValue typeObject = propertyObject.property("type");
        const SPropertyInformation *propType = 0;
        if(typeObject.isString())
          {
          propType = STypeRegistry::findType(typeObject.toString());
          }
        else if(typeObject.isObject())
          {
          QScriptValue typeName = typeObject.property("typeName");
          if(typeName.isString())
            {
            propType = STypeRegistry::findType(typeName.toString());
            }
          }

        if(!propType)
          {
          ctx->throwError(QScriptContext::SyntaxError, "String expected as 'properties' array 'type' entry");
          return false;
          }

        // Compute function
        SPropertyInstanceInformation::ComputeFunction computeFn = 0;
        QScriptValue computeObject = propertyObject.property("compute");
        if(computeObject.isFunction())
          {
          computeFn = computeNode;
          }

        // Default value
        QScriptValue valueObject = propertyObject.property("defaultValue");
        QString valueStr;
        if(valueObject.isString())
          {
          valueStr = valueObject.toString();
          }

        // add the described child
        SPropertyInstanceInformation *info = newType->add(propType, propName);
        info->setDefaultValue(valueStr);
        info->setCompute(computeFn);
        info->setExtra(true);

        if(computeFn)
          {
          info->setData(g_computeKey, qVariantFromValue(typeObject));
          }
        }
      else
        {
        SPropertyInstanceInformation *inst = newType->childFromName(propName);
        if(!inst)
          {
          ctx->throwError(QScriptContext::SyntaxError, "Attempting to extend non-defined child '" + propName + "'.");
          return false;
          }

        SPropertyInformation *extended = newType->extendContainedProperty(inst);
        // Extending known type:
        QScriptValue properties = propertyObject.property("properties");
        parseChildProperties(ctx, extended, properties);
        }
      }

    // Second pass, affects information, default inputs
    i = 0;
    for(QScriptValue propertyObject = propertiesArray.property(0);
        propertyObject.isObject();
        propertyObject = propertiesArray.property(++i))
      {
      // Affects
      QScriptValue affectsObject = propertyObject.property("affects");
      if(affectsObject.isArray())
        {
        xsize *affects = 0;
        bool affectsIsValid = true;
        xuint32 length = affectsObject.property("length").toUInt32();
        affects = new xsize [length+1];
        affects[length] = 0;
        for(xuint32 affectIdx=0; affectIdx<length; ++affectIdx)
          {
          affects[affectIdx] = 0;
          QString name = affectsObject.property(affectIdx).toString();

          SPropertyInstanceInformation *inst = newType->childFromName(name);
          if(inst)
            {
            affects[affectIdx] = inst->location();
            }

          if(!affects[affectIdx])
            {
            affectsIsValid = false;
            ctx->throwError(QScriptContext::SyntaxError, "Defined sibling property name expected for affectedBy members.");
            return false;
            }
          }

        if(!affectsIsValid)
          {
          delete [] affects;
          affects = 0;
          }

        SPropertyInstanceInformation *info =  newType->childFromIndex(i);
        info->setAffects(affects);
        }

      // Default input
      QScriptValue inputObject = propertyObject.property("defaultInput");
      if(inputObject.isString())
        {
        xAssertFail();
        }
      }
    }
  return true;
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

  QScriptValue nameObject = typeObject.property("name");
  QString name = nameObject.toString();
  if(name == "" || STypeRegistry::findType(name) != 0)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Unique, non zero-lengthed string expected as 'name' property");
    return QScriptValue();
    }

  QScriptValue parentObject = typeObject.property("parent");
  const SPropertyInformation *parent = 0;
  if(parentObject.isString())
    {
    parent = STypeRegistry::findType(parentObject.toString());
    }
  else if(parentObject.isObject())
    {
    QScriptValue typeName = parentObject.property("typeName");
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

  QScriptValue versionObject = typeObject.property("version");
  xuint32 version = versionObject.toUInt32();

  SPropertyInformation *newType = SPropertyInformation::create(parent);

  QScriptValue propertiesObject = typeObject.property("properties");
  parseChildProperties(ctx, newType, propertiesObject);

  newType->setVersion(version);
  newType->typeName() = name;
  newType->setParentTypeInformation(parent);

  QScriptValue prototype = typeObject.property("prototype");
  if(prototype.isObject())
    {
    QScriptValue g = engine->globalObject();
    QScriptValue parentObj = g.property(parent->typeName());
    xAssert(parentObj.isObject());
    if(parentObj.isObject())
      {
      prototype.setPrototype(parentObj);
      prototype.setProperty("typeName", name);
      g.setProperty(name, prototype);
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
