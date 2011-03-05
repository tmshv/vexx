#ifndef SCCONNECTION_H
#define SCCONNECTION_H

#include "scglobal.h"
#include "XProperty"

class ScNode;

class SC_EXPORT ScConnection
  {
properties:
  XROProperty( ScNode*, nodeA );
  XROProperty( ScNode*, nodeB );
  XROProperty( int, variableA );
  XROProperty( int, variableB );

public:
  ScConnection( ScNode *nA, int a, ScNode *nB, int b );
  bool operator==(const ScConnection &) const;
  };

uint qHash(const ScConnection &);

#endif // SCCONNECTION_H
