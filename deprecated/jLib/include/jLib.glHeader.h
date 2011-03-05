#ifndef JLIB_GLHEADER_H
#define JLIB_GLHEADER_H

#include "jLib.global.h"
#include "QDebug"
#include <GL/glew.h>

#if 1
# define GLE ; { int _GL = glGetError(); if( _GL ) { qDebug() << QString::fromStdString( jLib::handleGLError( _GL ) ); } }
# define GLE_QUIET ; glGetError()
#else
# define GLE ; { int _GL = glGetError(); if( _GL ) { jFail( jLib::handleGLError( _GL ) ); } }
# define GLE_QUIET ; glGetError()
#endif

BEGIN_JLIB_NAMESPACE

void JLIB_EXPORT glewInitiate();
string JLIB_EXPORT handleGLError( int err );

END_JLIB_NAMESPACE

#endif
