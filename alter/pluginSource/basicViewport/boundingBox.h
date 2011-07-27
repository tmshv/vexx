#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "../SDK/AGlobal"
#include "viewportGlobal.h"

class VIEWPORT_EXPORT boundingBox : public jLib::graphics::primitives::lineCloud
    {
public:
    boundingBox( jLib::graphics::entity::entityG *, const jLib::math::triple &, const jLib::math::triple & );

    void rebuild( const jLib::math::triple &, const jLib::math::triple & );
    };

#endif
