#ifndef CASSERT_H
#define CASSERT_H

#include "stdio.h"

#define C_ASSERT(exp) (exp) ? CAssertMessage(#exp,__FILE__,__LINE__) : CNoop();

inline void CAssertMessage(const char *expression, const char *file, size_t line)
  {
  printf("Assertion Failure in %s, on line %d: %s\n", file, line, expression);
  exit(EXIT_FAILURE);
  }

#endif // CASSERT_H
