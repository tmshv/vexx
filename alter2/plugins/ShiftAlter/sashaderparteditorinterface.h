#ifndef SASHADERPARTEDITORINTERFACE_H
#define SASHADERPARTEDITORINTERFACE_H

#include "saparteditorinterface.h"

class SShaderPartEditorInterface : public SDefaultPartEditorInterface
  {
public:

  virtual xsize numberOfTypeParameters(SEntity *prop) const;

  virtual void addProperty(SEntity *) const;
  virtual QStringList possiblePropertyTypes() const;

  virtual QWidget *buildCustomEditor(SEntity *) const;
  virtual QWidget *buildCustomPreview(SEntity *) const;
  };

#endif // SASHADERPARTEDITORINTERFACE_H
