#ifndef COMMANDTOOLS_H
#define COMMANDTOOLS_H

#include "../SDK/AGlobal"

class APrivateCommand;

#define emitCommand( com )                  \
    {                                       \
    APrivateCommand *comAlloc = new com;    \
    emit onCommand( &comAlloc );            \
    if( comAlloc )                          \
        {                                   \
        comAlloc->firstDo();                \
        delete comAlloc;                    \
        }                                   \
    }

#endif
