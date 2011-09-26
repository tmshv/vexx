#include "XAllocatorBase"

X_IMPLEMENT_EVENT(xTotalGlobalAllocatorSize, xsize, XEventLogger<xsize>::Large);

XGlobalAllocator g_allocator;

XGlobalAllocator *XGlobalAllocator::instance()
  {
  return &g_allocator;
  }

void *XGlobalAllocator::alloc(xsize size, xsize alignment)
  {
  return qMallocAligned(size, alignment);
  }

void XGlobalAllocator::free(void *mem)
  {
  qFreeAligned(mem);
  }
