#ifndef JLIB_FILE_DIR_H
#define JLIB_FILE_DIR_H

#include "file/file.types.h"

BEGIN_JLIB_FILE_NAMESPACE

typedef struct
    {
    string name;
    enum
        {
        UNKNOWN,
        FILE,
        DIRECTORY,
        FIFO,
        SOCKET,
        CHARACTER,
        BLOCK
        } type;
    } fileInfo;

class JLIB_EXPORT dir
    {
private:
    vector <fileInfo> entries;
    bool exist;

public:
    dir( string _d );
    void sort();
    bool exists( );
    unsigned int size( );
    fileInfo& operator[] ( const unsigned int _i );
    };

END_JLIB_FILE_NAMESPACE

#endif
