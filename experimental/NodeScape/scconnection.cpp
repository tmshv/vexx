#include "scconnection.h"
#include "QByteArray"
#include "QHash"

ScConnection::ScConnection( ScNode *nA, int a, ScNode *nB, int b )
    : _nodeA(nA), _nodeB(nB), _variableA(a), _variableB(b)
  {
  }

bool ScConnection::operator==(const ScConnection &conn) const
  {
  return conn.nodeA() == nodeA() &&
         conn.nodeB() == nodeB() &&
         conn.variableA() == variableA() &&
         conn.variableB() == variableB();
  }

uint qHash(const ScConnection &conn)
  {
  return qHash(QByteArray(reinterpret_cast<const char*>(&conn), sizeof(ScConnection)));
  }
