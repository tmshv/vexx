#ifndef JLIB_GRAPHICS_SHADER_TOOLS_H
#define JLIB_GRAPHICS_SHADER_TOOLS_H

#include "graphics/shader/graphics.shader.types.h"

BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

namespace tools
{
bool compile( int, string, string& );
bool link( int &, const int *, unsigned int, string & );
}

END_JLIB_GRAPHICS_SHADER_NAMESPACE

#endif
