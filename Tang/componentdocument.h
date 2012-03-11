#ifndef COMPONENTDOCUMENT_H
#define COMPONENTDOCUMENT_H

#include "saparteditor.h"
#include "saparteditorinterface.h"

class ComponentDocument : public SPartDocument
  {
  S_ENTITY(ComponentDocument, SPartDocument, 0)

public:
  ComponentDocument();

  virtual void saveFile(const QString &filename="");

  QWidget *createEditor();
  };

class ComponentEditorInterface : public SDefaultPartEditorInterface
  {
public:
  enum
    {
    SubValue = SDefaultPartEditorInterface::NumberOfSubTypeParameters,
    NumberOfSubTypeParameters
    };

  virtual xsize numberOfTypeParameters(SEntity *prop) const;

  /*virtual void addProperty(SEntity *) const;
  virtual QStringList possiblePropertyTypes() const;

  virtual xsize numberOfTypeSubParameters(SEntity *, void *property) const;
  virtual void typeSubParameter(SPartEditorInterfaceFeedbacker *, SEntity *, void *prop, xsize i, QString &name, QWidget *&widget) const;

  virtual QWidget *buildCustomEditor(SEntity *) const;
  virtual QWidget *buildCustomPreview(const SEntity *, SPropertyArray *) const;*/
  };

#endif // COMPONENTDOCUMENT_H
