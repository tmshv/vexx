#include "XNodeObserver.h"

XNodeOutputObserver::~XNodeOutputObserver()
  {
  foreach(XNetwork *net, _networks)
    {
    net->removeOutputObserver(this);
    }
  }

XNodeDirtyObserver::~XNodeDirtyObserver()
  {
  foreach(XNetwork *net, _networks)
    {
    net->removeDirtyObserver(this);
    }
  }
