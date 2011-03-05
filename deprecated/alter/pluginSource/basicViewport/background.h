#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../SDK/AGlobal"
#include "viewportGlobal.h"

class background : public jLib::graphics::primitives::triangleCloud
    {
    jProperty( jLib::graphics::shader::sColour, topColour )
    jProperty( jLib::graphics::shader::sColour, bottomColour )
public:
    background( jLib::graphics::entity::entityG * );
    ~background();

    virtual void draw();

private:
    void refreshShader();
    jLib::graphics::shader::surfaceShader *_shader;
    };

#endif
