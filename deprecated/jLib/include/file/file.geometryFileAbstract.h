#ifndef JLIB_FILE_GEOMETRYFILEABSTRACTFILE_H
#define JLIB_FILE_GEOMETRYFILEABSTRACTFILE_H

#include "file/file.types.h"

BEGIN_JLIB_FILE_NAMESPACE

class geometryFileAbstract
    {
public:
    vector <vector <unsigned int> >polygons;
    vector <math::triple> positions;
    vector <math::triple> normals;
    vector <math::vector2> textures;
    vector <math::colour4> colours;
    };

END_JLIB_FILE_NAMESPACE

#endif
