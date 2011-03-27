#include "scshiftdatabase.h"
#include "sdatabase.h"
#include "scembeddedtypes.h"

SPropertyInstanceInformation::DataKey g_computeKey(SPropertyInstanceInformation::newDataKey());

ScShiftDatabase::ScShiftDatabase(QScriptEngine *eng) : ScShiftEntity(eng, "SEntity")
  {
  addMemberFunction("addType", addType);
  }

ScShiftDatabase::~ScShiftDatabase()
  {
  }

void ScShiftDatabase::initiate()
  {
  setBlankConstructor<ScShiftDatabase>("SDatabase");
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
    ctx->throwError(QScriptContext::SyntaxError, "Incorrect this argument to SPropertyContainer.size(...);");
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
  if(name == "" || db->findType(name) != 0)
    {
    ctx->throwError(QScriptContext::SyntaxError, "Unique, non zero-lengthed string expected as 'name' property");
    return QScriptValue();
    }

  tempObject = typeObject.property("parent");
  const SPropertyInformation *parent = 0;
  if(tempObject.isString())
    {
    parent = db->findType(tempObject.toString());
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
        propType = db->findType(tempArrayObject.toString());
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

      SPropertyInstanceInformation *info = propType->createInstanceInformation()(propType, propName, i, *reinterpret_cast<SProperty SPropertyContainer::**>(&endOfUsedMemory), computeFn, 0, false, true);
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
      SProperty SPropertyContainer::* *affects = 0;
      tempArrayObject = val.property("affects");
      bool affectsIsValid = false;
      if(tempArrayObject.isArray())
        {
        affectsIsValid = true;
        xuint32 length = tempArrayObject.property("length").toUInt32();
        affects = new SProperty SPropertyContainer::* [length+1];
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

#warning check what happens if you embed a script type in a script type... etc, i think it will crash and burn?
  SPropertyInformation *newType = new SPropertyInformation(parent->create(), parent->createInstanceInformation(),
                                                           parent->save(), parent->load(), parent->assign(),
                                                           version, name, parent,
                                                           properties, endOfUsedMemory, parent->instanceInformationSize());

  db->addType(newType);

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
