#ifndef CTEST_H
#define CTEST_H

#include "cassert.h"

#define C_ASSERT(exp) (exp) ? CAssertFail(#exp,__FILE__,__LINE__);

#define CORE_CHECK(exp) C_ASSERT(exp)
#define CORE_CHECK_EQUAL(a,b) CORE_CHECK(a==b)

#endif // CTEST_H
