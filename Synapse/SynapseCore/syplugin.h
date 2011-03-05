#ifndef SYNAPSECORE_PLUGIN_H
#define SYNAPSECORE_PLUGIN_H

#include "syglobal.h"
#include "aabstractplugin.h"

class SYNAPSECORE_EXPORT SynapseCorePlugin : public AAbstractPlugin
  {
  Q_OBJECT

public:
  SynapseCorePlugin();

  virtual void load();
  };

#endif // SYNAPSECORE_PLUGIN_H
