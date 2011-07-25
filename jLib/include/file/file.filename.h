#ifndef JLIB_FILE_FILENAME_H
#define JLIB_FILE_FILENAME_H

#include "file/file.types.h"

BEGIN_JLIB_FILE_NAMESPACE

class JLIB_EXPORT filename
    {
public:
    string fileFullName;
    string fileDirName;
    string fileBaseName;
    string fileExtName;
    string slashType;

    filename( );
    filename( string _f );
    void processName( string _f );
    };

END_JLIB_FILE_NAMESPACE

#endif
