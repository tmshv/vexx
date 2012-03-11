#include "componentdocument.h"

S_IMPLEMENT_PROPERTY(ComponentDocument)

SPropertyInformation *ComponentDocument::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<ComponentDocument>("ComponentDocument");

  StringProperty::InstanceInformation* type = info->child(&ComponentDocument::type);
  type->setDefault("Component");

  return info;
  }

ComponentDocument::ComponentDocument()
  {
  }

#define JS_EXPORTER_NAME "Javascript Entity Export"
void ComponentDocument::saveFile(const QString &filename)
  {
  SPartDocument::saveFile(filename);

  const SPropertyInformation *info = typeInformation();
  const SExportableInterface *ifc = 0;
  const SExportableInterface::Exporter* exp = 0;
  while(!exp && info)
    {
    const SExportableInterface *newIfc = static_cast<const SExportableInterface*>(info->interfaceFactory(SExportableInterface::InterfaceTypeId));
    if(newIfc != ifc)
      {
      ifc = newIfc;
      if(ifc)
        {
        foreach(const XConstSharedPointer<SExportableInterface::Exporter> &e, ifc->exporters())
          {
          QString name = e->exporterName();
          if(name == JS_EXPORTER_NAME)
            {
            exp = e.constPtr();
            break;
            }
          }
        }
      }

    info = info->parentTypeInformation();
    }

  if(exp)
    {
    exp->exportFile(filename + ".js", this);
    }
  }

QWidget *ComponentDocument::createEditor()
  {
  return new SPartEditor(plugin(), this);
  }


xsize ComponentEditorInterface::numberOfTypeParameters(SEntity *prop) const
  {
  return SDefaultPartEditorInterface::numberOfTypeParameters(prop) - 1;
  }

/*void ComponentEditorInterface::addProperty(SEntity *) const
  {
  xAssertFail();
  }

QStringList ComponentEditorInterface::possiblePropertyTypes() const
  {
  xAssertFail();
  return QStringList();
  }

xsize ComponentEditorInterface::numberOfTypeSubParameters(SEntity *, void *property) const
  {
  xAssertFail();
  return 0;
  }

void ComponentEditorInterface::typeSubParameter(SPartEditorInterfaceFeedbacker *,
                                                SEntity *,
                                                void *prop,
                                                xsize i,
                                                QString &name,
                                                QWidget *&widget) const
  {
  xAssertFail();
  }

QWidget *ComponentEditorInterface::buildCustomEditor(SEntity *) const
  {
  return 0;
  }

QWidget *ComponentEditorInterface::buildCustomPreview(const SEntity *, SPropertyArray *) const
  {
  return 0;
  }
*/
