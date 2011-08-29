#ifndef GCBUTTONMANIPULATOR_H
#define GCBUTTONMANIPULATOR_H

#include "GCManipulator.h"

class GRAPHICSCORE_EXPORT GCButtonManipulator : public GCVisualClickManipulator
  {
  S_PROPERTY_CONTAINER(GCButtonManipulator, GCVisualClickManipulator, 0)

public:
  GCButtonManipulator();
  ~GCButtonManipulator();

  BoolProperty checked;
  BoolProperty checkable;

  virtual void onClick();
  };

#endif // GCBUTTONMANIPULATOR_H
