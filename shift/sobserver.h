#ifndef SOBSERVER_H
#define SOBSERVER_H

#include "XVector"
#include "sglobal.h"
#include "sproperty.h"

class SChange;

class SObserver
  {
public:
  virtual ~SObserver() { }
  virtual void actOnChanges() { };
  };

class SHIFT_EXPORT STreeObserver : public virtual SObserver
  {
public:
  virtual void onTreeChange(const SChange *) = 0;
  };

class SHIFT_EXPORT SDirtyObserver : public virtual SObserver
  {
public:
  virtual void onPropertyDirtied(const SProperty*) = 0;
  };

class SHIFT_EXPORT SConnectionObserver : public virtual SObserver
  {
public:
  virtual void onConnectionChange(const SChange *) = 0;
  };


#endif // SOBSERVER_H
