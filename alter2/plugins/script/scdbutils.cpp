#include "scdbutils.h"
#include "spropertyinformationhelpers.h"
#include "scglobal.h"
#include "XScriptFunction.h"
#include "XScriptValue.h"
#include "XScriptObject.h"
#include "XScriptException.h"
#include "styperegistry.h"
#include "sadocument.h"
#include "scplugin.h"

SPropertyInstanceInformation::DataKey g_computeKey(SPropertyInstanceInformation::newDataKey());

void computeNode(const SPropertyInstanceInformation *instanceInfo, SPropertyContainer *node)
  {
  ScProfileFunction
  const QVariant &val = instanceInfo->data()[g_computeKey];
  XScriptFunction compute = qvariant_cast<XScriptFunction>(val);
  xAssert(compute.isValid());

  // call with this as the node being computed, and no arguments.
  compute.call(XScriptConvert::to(node), 0, 0);
  }

bool parseChildProperties(SPropertyInformation *newType, XScriptValue propertiesArray, XScriptValue &error)
  {
  if(propertiesArray.isArray())
    {
    // First pass, add basic instance information
    for(xuint32 i = 0; i < propertiesArray.length(); ++i)
      {
      XScriptObject propertyObject = propertiesArray.at(i);

      // Name
      QString propName = propertyObject.get("name").toString();
      if(!propName.length())
        {
        error = Toss("String expected as 'properties' array 'name' entry");
        return false;
        }

      SPropertyInstanceInformation *inst = newType->childFromName(propName);
      if(!inst)
        {
        // Type
        const SPropertyInformation *propType = XScriptConvert::from<SPropertyInformation>(propertyObject.get("type"));

        if(!propType)
          {
          error = Toss("String expected as 'properties' array 'type' entry");
          return false;
          }

        // add the described child
        inst = newType->add(propType, propName);
        inst->setExtra(true);
        }

      // Default value
      XScriptValue valueObject = propertyObject.get("defaultValue");
      QString valueStr;
      if(valueObject.isValid())
        {
        valueStr = valueObject.toString();
        }

      if(!valueStr.isEmpty())
        {
        inst->setDefaultValue(valueStr);
        }

      // Compute function
      XScriptFunction computeObject(propertyObject.get("compute"));
      if(computeObject.isValid())
        {
        inst->setCompute(computeNode);
        inst->setData(g_computeKey, qVariantFromValue(computeObject));
        }

      // Extending a current child?
      XScriptValue extendingObject = propertyObject.get("extend");
      if(extendingObject.isValid() && extendingObject.toBoolean())
        {
        SPropertyInformation *extended = newType->extendContainedProperty(inst);
        // Extending known type:
        XScriptValue properties = propertyObject.get("properties");
        if(!parseChildProperties(extended, properties, error))
          {
          return false;
          }
        }
      }
    }
  return true;
  }

bool postParseChildProperties(SPropertyInformation *newType, XScriptValue propertiesArray, XScriptValue &error)
  {
  if(propertiesArray.isArray())
    {
    // Second pass, affects information, default inputs
    for(xuint32 i = 0; i < propertiesArray.length(); ++i)
    {
      XScriptObject propertyObject = propertiesArray.at(i);
      // Name
      QString propName = propertyObject.get("name").toString();
      if(!propName.length())
        {
        error = Toss("String expected as 'properties' array 'name' entry");
        return false;
        }

      SPropertyInstanceInformation *info =  newType->childFromName(propName);
      xAssert(info);

      // Affects
      XScriptValue affectsObject = propertyObject.get("affects");
      if(affectsObject.isArray())
        {
        xsize *affects = 0;
        bool affectsIsValid = true;
        xuint32 length = affectsObject.length();
        affects = new xsize [length+1];
        affects[length] = 0;
        for(xuint32 affectIdx=0; affectIdx<length; ++affectIdx)
          {
          affects[affectIdx] = 0;
          QString name = affectsObject.at(affectIdx).toString();

          SPropertyInstanceInformation *inst = newType->childFromName(name);
          if(inst)
            {
            affects[affectIdx] = inst->location();
            }

          if(!affects[affectIdx])
            {
            affectsIsValid = false;
            error = Toss("Defined sibling property name expected for affectedBy members.");
            return false;
            }
          }

        if(!affectsIsValid)
          {
          delete [] affects;
          affects = 0;
          }

        info->setAffects(affects);
        }

      // Default input
      XScriptValue inputPathVal = propertyObject.get("defaultInput");
      if(inputPathVal.isValid())
        {
        QString inputPath = inputPathVal.toString();
        if(inputPath.length())
          {
          const SPropertyInstanceInformation *input = info->resolvePath(inputPath);
          if(!input)
            {
            error = Toss("Unable to find default input '" + inputPath + "'");
            return false;
            }

          info->setDefaultInput(input);
          }
        }

      // Extending a current child?
      XScriptValue extendingObject = propertyObject.get("extend");
      if(extendingObject.isValid() && extendingObject.toBoolean())
        {
        SPropertyInstanceInformation *inst = newType->childFromName(propName);
        if(!inst)
          {
          error = Toss("Attempting to extend non-defined child '" + propName + "'.");
          return false;
          }

        SPropertyInformation *child = const_cast<SPropertyInformation*>(inst->childInformation());

        // Extending known type:
        XScriptValue properties = propertyObject.get("properties");
        if(!postParseChildProperties(child, properties, error))
          {
          return false;
          }
        }
      }
    }
  return true;
  }

XScriptValue registerTypeFn(XScriptArguments const &args)
  {
  ScProfileFunction

  if(args.length() != 1)
    {
    return Toss("Incorrect number of arguments to SDatabase.addType(...);");
    }

  XScriptValue typeValue = args.at(0);
  if(!typeValue.isObject())
    {
    return Toss("Incorrect argument type to SDatabase.addType(...); expected object");
    }

  XScriptObject typeObject(typeValue);

  XScriptValue nameObject = typeObject.get("name");
  QString name = nameObject.toString();
  if(name == "" || STypeRegistry::findType(name) != 0)
    {
    return Toss("Unique, non zero-lengthed string expected as 'name' property");
    }

  XScriptValue parentObject = typeObject.get("parent");
  const SPropertyInformation *parent = 0;
  if(parentObject.isObject())
    {
    XScriptValue typeName = XScriptObject(parentObject).get("typeName");
    parent = STypeRegistry::findType(typeName.toString());
    }
  else
    {
    parent = STypeRegistry::findType(parentObject.toString());
    }
  if(parent == 0)
    {
    return Toss("Defined property type expected as 'parent' property");
    }

  xuint32 version = XScriptConvert::from<xuint32>(typeObject.get("version"));

  SPropertyInformation *newType = SPropertyInformation::derive(parent);
  newType->setVersion(version);
  newType->typeName() = name;
  newType->setParentTypeInformation(parent);

  XInterfaceBase* pAPI = parent->apiInterface();
  XInterfaceBase* api = new XInterfaceBase(pAPI->typeId(),
                                           pAPI->nonPointerTypeId(),
                                           name,
                                           pAPI);

  newType->setApiInterface(api);

  XScriptObject propertiesObject(typeObject.get("properties"));
  XScriptValue error;
  if(!parseChildProperties(newType, propertiesObject, error) ||
     !postParseChildProperties(newType, propertiesObject, error))
    {
    return error;
    }

  STypeRegistry::addType(newType);

  return XScriptValue();
  }

XScriptValue registerExporterFn(XScriptArguments const &args)
  {
  ScProfileFunction

  if(args.length() != 2)
    {
    return Toss("Incorrect number of arguments to SDatabase.registerExporterFn(...);");
    }

  const SPropertyInformation *info = XScriptConvert::from<SPropertyInformation>(args.at(0));
  if(!info)
    {
    return Toss("Cannot find type " + args.at(0).toString() + " to add exporter.");
    }

  XScriptValue exp = args.at(1);

  class ScriptExporter : public SExportableInterface::Exporter
    {
  public:
    ScriptExporter(XScriptObject v) : _v(v)
      {
      }

    ~ScriptExporter()
      {
      _v.dispose();
      }

    virtual bool exportFile(const QString &file, SProperty *prop) const
      {
      XScriptObject obj(_v.asValue());
      XScriptFunction exFn(obj.get("exportFile"));

      if(!exFn.isValid())
        {
        return false;
        }

      XScriptValue l[] = {
        file,
        XScriptConvert::to(prop)
      };

      XScriptValue r = ScPlugin::call(exFn, obj, l, X_SIZEOF_ARRAY(l));

      return r.toBoolean();
      }

    virtual QString exporterName() const
      {
      XScriptObject obj(_v.asValue());
      return obj.get("name").toString();
      }

    virtual QString exporterFileType() const
      {
      XScriptObject obj(_v.asValue());
      return obj.get("fileType").toString();
      }

  private:
    XPersistentScriptValue _v;
    };

  XScriptObject obj(exp);
  if(!obj.isValid())
    {
    return Toss("Invalid input to register exporter");
    }

  SExportableInterface::Exporter *exporter = new ScriptExporter(obj);
  SExportableInterface::addExporter(info, exporter);

  return XScriptValue();
  }

