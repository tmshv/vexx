#include "XGlobal"
#include "stdio.h"
#include "stdlib.h"

#ifdef Q_CC_MSVC
# define WIN32_LEAN_AND_MEAN
# include <Windows.h>
# define snprintf _snprintf_s
#endif

void xDoAssert(const char *assertion, const char *file, int line)
  {
  char buf[256];
  snprintf(buf, 256, "ASSERT: \"%s\" in file %s, line %d", assertion, file, line);

  printf(buf);

#ifdef Q_CC_MSVC
  OutputDebugStringA(buf);
  DebugBreak();
#else
# error define a break type macro for this platform
#endif
  }
