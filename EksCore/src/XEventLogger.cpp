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

XEventLoggerInternal::XEventLoggerInternal(const char *n, int t, xsize s, xsize c, const char *units)
    : name(n), units(units), type(t), size(s), count(c), next(0), firstData(0), nextData(data)
  {
  XEventManager::addEvent(this);
  }


xsize XEventLoggerInternal::used() const
  {
  if(nextData == data)
    {
    return 0;
    }

  if(nextData == firstData)
    {
    return count;
    }

  return (nextData - firstData) / size;
  }

const void *XEventLoggerInternal::at(xsize index) const
  {
  xAssert(firstData);
  xuint8 *d = firstData + (index * size);

  const xsize s = (count * size);
  if(d > (data + s))
    {
    d -= s;
    }

  return d;
  }

const void *XEventLoggerInternal::last() const
  {
  if(nextData == data)
    {
    return 0;
    }

  xuint8 *p = nextData - size;
  if(p < data)
    {
    p += (used() * size);
    }

  return p;
  }

void *XEventLoggerInternal::last()
  {
  if(nextData == data)
    {
    return 0;
    }

  xuint8 *p = nextData - size;
  if(p < data)
    {
    p += (used() * size);
    }

  return p;
  }

void XEventLoggerInternal::add()
  {
  bool syncFirst = nextData == firstData;

  if(!firstData)
    {
    firstData = data;
    }

  nextData += size;
  if(nextData > (data + count * size))
    {
    nextData = data;
    }

  if(syncFirst)
    {
    firstData = nextData;
    }
  }
