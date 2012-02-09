#ifndef SYDOCUMENT_H
#define SYDOCUMENT_H

#include "sadocument.h"
#include "syimageinput.h"

class SyDocument : public SDocument
  {
  S_ENTITY(SyDocument, SDocument, 0)

public:
  SyDocument();

  SyImageInput input;

  virtual QWidget *createEditor();
  };

#endif // SYDOCUMENT_H
