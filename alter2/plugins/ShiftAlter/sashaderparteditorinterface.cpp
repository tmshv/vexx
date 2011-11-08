#include "sashaderparteditorinterface.h"
#include "3D/GCShader.h"
#include "3D/GCCamera.h"
#include "3D/GCViewport.h"
#include "3D/GCScene.h"
#include "3D/Shaders/GCStandardSurface.h"
#include "3D/GCShadingGroup.h"
#include "3D/GCScreenRenderTarget.h"
#include "3D/Renderable/GCCuboid.h"
#include "saviewport.h"
#include "QVBoxLayout"
#include "QTextEdit"
#include "QLabel"
#include "QFile"
#include "spropertydefaultui.h"

class ShaderPreviewViewport : public SViewport
  {
public:
  ShaderPreviewViewport(SEntity *ent) : SViewport(ent)
    {
    GCShader *shader = ent->uncheckedCastTo<GCShader>();

    SEntity *sc = scene();

    GCViewport* vp = sc->addChild<GCViewport>("Viewport");
    _viewport = vp;
    GCScreenRenderTarget* op = sc->addChild<GCScreenRenderTarget>("Output");

    GCPerspectiveCamera* cam = sc->addChild<GCPerspectiveCamera>("Camera");
    vp->x.connect(&cam->viewportX);
    vp->y.connect(&cam->viewportY);
    vp->width.connect(&cam->viewportWidth);
    vp->height.connect(&cam->viewportHeight);

    cam->setPosition(XVector3D(0.0f, 0.0f, 10.0f));
    cam->setFocalPoint(XVector3D(0.0f, 0.0f, 0.0f));

    GCManipulatableScene* msc = sc->addChild<GCManipulatableScene>("Scene");
    cam->projection.connect(&msc->cameraProjection);
    cam->viewTransform.connect(&msc->cameraTransform);
    cam->connect(&msc->activeCamera);
    setController(msc);

    GCShadingGroup *group = sc->addChild<GCShadingGroup>("Groups");
    msc->shadingGroups.addPointer(group);

    group->shader.setPointed(shader);

    GCGeometryTransform *transform = sc->addChild<GCGeometryTransform>("Transform");
    group->geometry.addPointer(transform);


    GCCuboid *cube = sc->addChild<GCCuboid>("Cube");
    transform->geometry.setPointed(&cube->geometry);

    op->source.setPointed(msc);
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
    {
    QFile shaderResource(":/GLResources/shaders/standardSurface.frag");
    if(shaderResource.open(QIODevice::ReadOnly))
      {
      frag->source = shaderResource.readAll();
      }
    }

  shader->components.addPointer(frag);
  QWidget *vertex(new SPropertyDefaultUI::LongString(&frag->source, false, 0));
  layout->addWidget(vertex);

  layout->addWidget(new QLabel("<b>Fragment</b>"));

  GCShaderComponent *vert = ent->addProperty<GCVertexShaderComponent>();
    {
    QFile shaderResource(":/GLResources/shaders/standardSurface.vert");
    if(shaderResource.open(QIODevice::ReadOnly))
      {
      vert->source = shaderResource.readAll();
      }
    }

  shader->components.addPointer(vert);
  QWidget *fragment(new SPropertyDefaultUI::LongString(&vert->source, false, 0));
  layout->addWidget(fragment);

  return main;
  }

QWidget *SShaderPartEditorInterface::buildCustomPreview(SEntity *e) const
  {
  return new ShaderPreviewViewport(e);
  }
