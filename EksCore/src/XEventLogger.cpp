#include "XEventLogger"

XEventLoggerInternal *g_firstEvent = 0;
XEventLoggerInternal *XEventManager::firstEvent()
  {
  return g_firstEvent;
  }

void XEventManager::addEvent(XEventLoggerInternal *e)
  {
  xAssert(e->next == 0); // double add?
  e->next = g_firstEvent;
  g_firstEvent = e;
  }

XEventLoggerInternal::XEventLoggerInternal(const char *n, int t, xsize s, xsize c)
    : name(n), type(t), size(s), count(c), used(0), next(0), first(0)
  {
  XEventManager::addEvent(this);
  }

const void *XEventLoggerInternal::at(xsize index) const
  {
  xuint8 *d = first + (index *(sizeof(XTime) + size));

  const xsize s = (count * (sizeof(XTime) + size));
  if(d > (data + s))
    {
    d -= s;
    }

  return d;
  }

const void *XEventLoggerInternal::last() const
  {
  if(used == 0)
    {
    return 0;
    }

  if(used == count && first != data)
    {
    return first - (sizeof(XTime) + size);
    }

  return data + (used *(sizeof(XTime) + size));
  }

void *XEventLoggerInternal::last()
  {
  if(used == count && first != data)
    {
    return first - (sizeof(XTime) + size);
    }
  return data + (used *(sizeof(XTime) + size));
  }

void XEventLoggerInternal::add()
  {
  }
