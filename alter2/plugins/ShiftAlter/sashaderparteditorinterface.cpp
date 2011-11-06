#include "sashaderparteditorinterface.h"
#include "3D/GCShader.h"
#include "saviewport.h"
#include "QVBoxLayout"
#include "QTextEdit"
#include "QLabel"

xsize SShaderPartEditorInterface::numberOfTypeParameters(SEntity *prop) const
  {
  return SDefaultPartEditorInterface::numberOfTypeParameters(prop) - 1;
  }

void SShaderPartEditorInterface::addProperty(SEntity *c) const
  {
  c->addProperty<ColourProperty>();
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

QWidget *SShaderPartEditorInterface::buildCustomEditor(SEntity *) const
  {
  QWidget *main(new QWidget);
  QVBoxLayout *layout(new QVBoxLayout(main));
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(new QLabel("<b>Vertex</b>"));

  QTextEdit *vertex(new QTextEdit);
  layout->addWidget(vertex);

  layout->addWidget(new QLabel("<b>Fragment</b>"));

  QTextEdit *fragment(new QTextEdit);
  layout->addWidget(fragment);

  return main;
  }

QWidget *SShaderPartEditorInterface::buildCustomPreview(SEntity *e) const
  {
  return new SViewport(e);
  }
