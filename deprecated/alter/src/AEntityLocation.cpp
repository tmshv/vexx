#include "../SDK/AEntityLocation"
#include "QString"
#include "QStringList"
#include "../SDK/ALog"

/** \class AEntityLocation
  \brief A location in a \c AEntityArray. Used to query and manipulate the location.
  */

/** \fn AEntityLocation::XROProperty( QString, path )
  This property allows read only access to the path. to change the path use the functions detailed below.
  */

/** Create a location from a string eg "/directory/path/name"
  \sa path(QString)
  */
AEntityLocation::AEntityLocation( const char *in )
    {
    path( in );
    }

/** Create a location from a variant containing a string path
  \sa operator AVariant()
  */
AEntityLocation::AEntityLocation( AVariant in )
    {
    path( in.toString() );
    }

/** Create a location from a string eg "/directory/path/name"
  \sa path(QString)
  */
AEntityLocation::AEntityLocation( QString in )
    {
    path( in );
    }

/** Create a location from the path \p p and the name \p n. The name should not contain '/'
  */
AEntityLocation::AEntityLocation( AEntityLocation p, QString n )
    {
    path( p, n );
    }

/** Set the path to \p in.
  */
void AEntityLocation::path( QString in )
    {
    // split up path by '/'
    QStringList pathSplit = in.split( "/", QString::SkipEmptyParts );
    QString ret;
    // if it isnt blank
    if( pathSplit.size() )
        {
        // set the path to the concatenated parts ( this splitting removes any // and a final /
        for( int x=0; x<pathSplit.size(); x++ )
            {
            ret += "/" + pathSplit[x];
            }
        }
    else
        {
        // otherwise a default path
        ret = "/";
        }
    _path = ret;
    }

/** \overload
  This sets the path to \p p / \p n.
  */
void AEntityLocation::path( AEntityLocation p, QString n )
    {
    path( p.path() + "/" + n );
    }

/** Set the directory of the path, retains the name as \c name().
  */
void AEntityLocation::directory( QString in )
    {
    path( in + "/" + name() );
    }

/** Return the parent location of this entity location. eg. if path() is /directory/to/place, parent() is /directory/to
  */
AEntityLocation AEntityLocation::parent( ) const
    {
    return directory();
    }
/** Find the directory of the path. eg. if path() is /directory/thing/name, directory() is /directory/thing ( note the missing final slash )
  */
QString AEntityLocation::directory() const
    {
    QStringList pathSplit = _path.split( "/", QString::SkipEmptyParts );
    QString ret;
    for( int x=0; x<(pathSplit.size()-1); x++ )
        {
        ret += "/" + pathSplit[x];
        }

    return ret;
    }

/** Find all the different levels in the path as their individual components.
  \note these strings have no /'s at all.
  */
QStringList AEntityLocation::levels( ) const
    {
    return _path.split( "/", QString::SkipEmptyParts );
    }

/** Find the resulting path if we remove the path \p in from the beginning of this path. eg. if path() is /dir/a/b/c/d and root.path() is /dir/a/b then the returned value's path is /c/d
  */
AEntityLocation AEntityLocation::subtractBasePath( const AEntityLocation &root ) const
    {
    if( isChild( root ) )
        {
        QStringList pathSplit = _path.split( "/", QString::SkipEmptyParts );
        QString ret;

        for( int x=root.level()+1; x<pathSplit.size(); x++ )
            {
            ret += "/" + pathSplit[x];
            }

        return ret;
        }
    else if( *this == root )
        {
        return "/";
        }
    return *this;
    }

/** Return a number of levels of the path. eg. if path() is /directory/path/to/somewhere then upToLevel(2) is /directory/path/to
  */
AEntityLocation AEntityLocation::upToLevel( int top ) const
    {
    QStringList pathSplit = _path.split( "/", QString::SkipEmptyParts );
    QString ret;
    if( ( top + 1 ) > pathSplit.size() )
        {
        return _path;
        }
    for( int x=0; x<top+1; x++ )
        {
        ret += "/" + pathSplit[x];
        }

    return ret;
    }

/** Set the name of the path, the directory is retained as directory()
  */
void AEntityLocation::name( QString in )
    {
    path( directory() + "/" + in );
    }

/** Return the name of the path
  */
QString AEntityLocation::name( ) const
    {
    // name is the last array entry if path is formatted properly
    QStringList pathSplit = _path.split( "/", QString::SkipEmptyParts );
    if( pathSplit.size() > 0 )
        {
        return pathSplit[ pathSplit.size() - 1 ];
        }
    return "";
    }

/** Find the number of the levels in the path. eg. if path() is /dir/a/b/c the level is 3.
  \todo this maths is a bit misleading isnt it...
  */
unsigned int AEntityLocation::level( ) const
    {
    // level shouldnt be - 1 should it? must check to see if i depend on this result anywhere first though.
    QStringList pathSplit = _path.split( "/", QString::SkipEmptyParts );
    return pathSplit.size() - 1;
    }

/** Returns true if this path is a direct child of the given path \p in. eg. if in.path() is /dir and path() is /dir/a, and not /dir/a/b.
  */
bool AEntityLocation::isDirectChild( AEntityLocation in ) const
    {
    // split both the possible child and parent path
    QStringList cPath = _path.split( "/", QString::SkipEmptyParts );
    QStringList pPath = in._path.split( "/", QString::SkipEmptyParts );

    // if the sizes dont match up then they cant be DIRECTLY linked
    if( ( pPath.size() + 1 ) != cPath.size() )
        {
        return FALSE;
        }

    // now compare each level and ensure they are equal
    for( int x=0; x<pPath.size(); x++ )
        {
        if( pPath[x] != cPath[x] )
            {
            return FALSE;
            }
        }

    // must be directly related
    return TRUE;
    }

/** Returns true if this path is a child of the given path \p in. eg. if in.path() is /dir and path() is /dir/a/b/c.
  */
bool AEntityLocation::isChild( AEntityLocation in ) const
    {
    // split both the possible child and parent path
    QStringList cPath = _path.split( "/", QString::SkipEmptyParts );
    QStringList pPath = in._path.split( "/", QString::SkipEmptyParts );

    // if the sizes dont match up then they cant linked
    if( pPath.size() >= cPath.size() )
        {
        return FALSE;
        }

    // now compare each level and ensure they are equal
    for( int x=0; x<pPath.size(); x++ )
        {
        if( pPath[x] != cPath[x] )
            {
            return FALSE;
            }
        }

    // must be directly related
    return TRUE;
    }

/** Returns true if the path is /
  */
bool AEntityLocation::isRoot( ) const
    {
    if( _path == "/" )
        {
        return TRUE;
        }
    return FALSE;
    }

/** Convert the location to a variant
  */
AEntityLocation::operator AVariant() const
    {
    return AVariant( path() );
    }

/** Test for equality between two locations.
  */
bool AEntityLocation::operator==( const AEntityLocation &in ) const
    {
    // split both paths
    QStringList pathSplitA = in._path.split( "/", QString::SkipEmptyParts );
    QStringList pathSplitB = _path.split( "/", QString::SkipEmptyParts );

    // test sizes are equal
    if( pathSplitA.size() != pathSplitB.size() )
        {
        return FALSE;
        }

    // test equality for each part
    for( int x=0; x<pathSplitA.size(); x++ )
        {
        if( pathSplitA[x] != pathSplitB[x] )
            {
            return FALSE;
            }
        }

    return TRUE;
    }

/** Find the two locations componded together. eg. if this is /dir/a and a is /c/d the return will be /dir/a/c/d.
  */
AEntityLocation AEntityLocation::appendLocation( const AEntityLocation &a ) const
    {
    return path() + "/" + a.path();
    }

/** \overload
  Test for equality with a string. eg. //thing////la/a/ is the same as a path containing /thing/la/a
  */
bool AEntityLocation::operator==( QString &in ) const
    {
    return AEntityLocation( in ) == _path;
    }

/** Find the two locations componded together. eg. if a is /dir/a and b is /c/d the return will be /dir/a/c/d.
  */
AEntityLocation operator+( const AEntityLocation &a, const AEntityLocation &b )
    {
    return a.appendLocation( b );
    }

/** Test for equality between a string and a location
  \relates AEntityLocation
  */
bool operator==( const QString &s, const AEntityLocation &e )
    {
    return e == AEntityLocation( s );
    }

/** Find the hash of this location, used when putting locations into a QHash
  \relates AEntityLocation
  */
uint qHash( AEntityLocation in )
    {
    return qHash( in.path() );
    }
