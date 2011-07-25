#ifndef FRAMELOG_H
#define FRAMELOG_H

#include "physiCalGlobal.h"

extern jLib::error::report systemReport;

#define FRAMELOG_ID 76

#if 1
#   define frameLog( a ) jReportCustom( FRAMELOG_ID, systemReport )<<a<<endl;
#else
#   define frameLog( a )
#endif

#endif
