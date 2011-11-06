#include "sashaderparteditorinterface.h"
#include "3D/GCShader.h"
#include "saviewport.h"
#include "QVBoxLayout"
#include "QTextEdit"
#include "QLabel"
#include "spropertydefaultui.h"

class ShaderPreviewViewport : public SViewport
  {
public:
  ShaderPreviewViewport(SEntity *ent) : SViewport(ent)
    {
    }
  };

xsize SShaderPartEditorInterface::numberOfTypeParameters(SEntity *prop) const
  {
  return SDefaultPartEditorInterface::numberOfTypeParameters(prop) - 1;
  }

void SShaderPartEditorInterface::addProperty(SEntity *e) const
  {
  ColourProperty *newProp = e->addProperty<ColourProperty>();
  const ColourProperty::InstanceInformation *cI = newProp->instanceInformation();
  ColourProperty::InstanceInformation *i = const_cast<ColourProperty::InstanceInformation*>(cI);

  GCShader *shader = e->uncheckedCastTo<GCShader>();
  i->setAffects(shader->runtimeShader.instanceInformation());
  }

QStringList SShaderPartEditorInterface::possiblePropertyTypes() const
  {
  QStringList l;
  foreach(const SPropertyInformation *i, STypeRegistry::types())
    {
    if(i->interfaceFactory(GCShaderBindableData::InterfaceTypeId))
      {
      l << i->typeName();
      }
    }

  qSort(l);
  return l;
  }

QWidget *SShaderPartEditorInterface::buildCustomEditor(SEntity *e) const
  {
  GCShader *shader = e->uncheckedCastTo<GCShader>();

  QWidget *main(new QWidget);
  QVBoxLayout *layout(new QVBoxLayout(main));
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(new QLabel("<b>Vertex</b>"));

  SEntity *ent = e->children.findChild<SEntity>("ShaderContents");
  if(!ent)
    {
    ent = e->addChild<SEntity>("ShaderContents");
    }

  GCShaderComponent *frag = ent->addProperty<GCFragmentShaderComponent>();
  shader->components.addPointer(frag);
  QWidget *vertex(new SPropertyDefaultUI::LongString(&frag->source, false, 0));
  layout->addWidget(vertex);

  layout->addWidget(new QLabel("<b>Fragment</b>"));

  GCShaderComponent *vert = ent->addProperty<GCVertexShaderComponent>();
  shader->components.addPointer(vert);
  QWidget *fragment(new SPropertyDefaultUI::LongString(&vert->source, false, 0));
  layout->addWidget(fragment);

  return main;
  }

QWidget *SShaderPartEditorInterface::buildCustomPreview(SEntity *e) const
  {
  return new ShaderPreviewViewport(e);
  }
