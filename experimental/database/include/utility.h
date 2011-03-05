#pragma once
#include "string"
#include "vector"
#include "sstream"
#include <typeinfo>

#define _DEBUG
#include "assert.h"


// DLL export stuff
#ifdef WIN32
#ifdef DATABASE_BUILD
# define DATABASE_EXPORT __declspec(dllexport)
#else
# define DATABASE_EXPORT __declspec(dllimport)
#endif
#else
# define DATABASE_EXPORT
#endif
