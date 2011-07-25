#ifndef JLIB_FILE_IOFILE_H
#define JLIB_FILE_IOFILE_H

#include "file/file.types.h"

BEGIN_JLIB_FILE_NAMESPACE

class JLIB_EXPORT ioFile
    {
public:
    //! Initiate from a string filename
    ioFile( string );
    //! Initiate from an ioFile
    ioFile( const ioFile & );
    //! Destory the file object, flushing if changes have been made
    ~ioFile( );
    //! Change the current file name, internal string remains unchanged until get called
    void setFilename( string );
    //! Retrieve the filename
    string getFilename( ) const;
    //! Get the contents of the file
    string &get( );
    //! Stream data to or from the file
    std::fstream &operator()( );
    //! Dump the cached version of the file and reload it from disk
    void reload( );
    //! Set the internal string holding the files contents
    void set( string );
    //! Write all changes to the file - if the file string has been changed
    void write( );
    //! For use with streaming operations, seek to an absolute position in the stream
    void seek( std::streampos );
    //! For use with streaming operations seek to a relative position in the file
    void seek( std::streamoff, std::ios_base::seekdir );
    //! For use with streaming operations, find where we are in the stream
    unsigned int tell( );
    //! Returns the buffer length ( file length if buffer hasnt been changed ), 0 for empty / non-existant
    unsigned int length();
    //! Does the file exist?
    bool exists( );
    //! Delete the file on disk, internakl string remains unchanged, get will return empty string
    bool remove( );
    //! Remove the specified file
    static bool remove( string );
    //! Does the specified file exist?
    static bool exists( string );
    //! Get the contents of a file as a string
    static string get( string );
protected:
    void open();
    void close();
    string name;
    string buffer;
    std::fstream reference;
    };

END_JLIB_FILE_NAMESPACE

#endif
