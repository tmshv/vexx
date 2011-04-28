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

class SHIFT_EXPORT STreeObserver : public SObserver
  {
public:
  virtual void onTreeChange( int m, const SChange *) = 0;
  };

class SHIFT_EXPORT SDataObserver : public SObserver
  {
public:
  virtual void onDataChange( int m, const SProperty::DataChange *) = 0;
  };

class SHIFT_EXPORT SConnectionObserver : public SObserver
  {
public:
  virtual void onConnectionChange( int m, const SChange *) = 0;
  };


#endif // SOBSERVER_H
