#ifndef AREADOCUMENT_H
#define AREADOCUMENT_H

#include "sadocument.h"
#include "saparteditorinterface.h"

class AreaDocument : public SDocument
  {
  S_ENTITY(AreaDocument, SDocument, 0)

public:
  AreaDocument();

  void newFile();

  QWidget *createEditor();
  };

S_PROPERTY_INTERFACE(AreaDocument)

#endif // AREADOCUMENT_H
