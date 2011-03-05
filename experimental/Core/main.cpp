#include "cvector.h"
#include "ctest.h"

int main(int argc, char *argv[])
{
  size_t vecCheck = 0, vec2Check = 0;

  CVector <int,size_t> vec(10);
  vec.resize(15, 0);
  vec.reserve(20);

  for(size_t i=0; i<vec.size(); ++i, ++vecCheck) { }
  CORE_CHECK_EQUAL(15,vec.size());
  CORE_CHECK_EQUAL(15,vecCheck);
  CORE_CHECK_EQUAL(20,vec.capacity());

  vec.resize(2);
  CVector <int,size_t> vec2(vec);

  for(size_t i=0; i<vec.size(); ++i, ++vec2Check) { }
  CORE_CHECK_EQUAL(2,vec.size());
  CORE_CHECK_EQUAL(2,vec2Check);

  vec2.resize(40);
  vec2Check = 0;
  for(size_t i=0; i<vec2.size(); ++i, ++vec2Check) { }
  CORE_CHECK_EQUAL(40,vec2.size());
  CORE_CHECK_EQUAL(40,vec2.capacity());
  CORE_CHECK_EQUAL(40,vec2Check);

  vecCheck = 0;
  for(size_t i=0; i<vec.size(); ++i, ++vecCheck) { }
  CORE_CHECK_EQUAL(2,vec.size());
  CORE_CHECK_EQUAL(20,vec.capacity());
  CORE_CHECK_EQUAL(20,vecCheck);

  return EXIT_SUCCESS;
}
