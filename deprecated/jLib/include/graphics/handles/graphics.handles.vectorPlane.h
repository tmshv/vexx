#ifndef JLIB_GRAPHICS_HANDLES_VECTOR1D_H
#define JLIB_GRAPHICS_HANDLES_VECTOR1D_H

#include <graphics/graphics.types.h>
#include <graphics/graphics.colour4.h>
#include <graphics/graphics.entity.h>
#include <graphics/implicit/graphics.implicit.vector.h>

namespace jLib
    {
    namespace graphics
        {
        namespace handles
            {
            class vector1D : entityG
                {
            public:
                vector1D( entityG *p, jLib::graphics::colour4 c=jLib::graphics::colour4() );

            private:
                jLib::graphics::implicit::vector arrow;
                };
            }
        }
    }

#endif
