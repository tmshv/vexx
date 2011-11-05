#ifndef SOBSERVER_H
#define SOBSERVER_H

#include "sglobal.h"
#include "XVector"
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
  virtual ~STreeObserver() { }
  virtual void onTreeChange(const SChange *) = 0;
  };

class SHIFT_EXPORT SDirtyObserver : public virtual SObserver
  {
public:
  virtual ~SDirtyObserver() { }
  virtual void onPropertyDirtied(const SProperty*) = 0;
  };

class SHIFT_EXPORT SConnectionObserver : public virtual SObserver
  {
public:
  virtual ~SConnectionObserver() { }
  virtual void onConnectionChange(const SChange *) = 0;
  };


#endif // SOBSERVER_H
