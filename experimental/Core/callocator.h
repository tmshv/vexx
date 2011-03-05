#ifndef CALLOCATOR_H
#define CALLOCATOR_H

#include "malloc.h"

template <typename Element, typename SizeType> class CAllocator
{
public:
  static Element *get(SizeType size)
    {
    return (Element *)malloc(sizeof(Element)*size);
    }
  static void release(Element *in)
    {
    free(in);
    }
};

#endif // CALLOCATOR_H
