#include "../SDK/AEntitySearch"

/** \class AEntitySearch
  \brief An AEntitySearch allows a user to search through an array in useful different ways.
  \sa AEntitySearchConst
  */

/** Create a search from the array.
  */
AEntitySearch::AEntitySearch( AEntityArray &_ent )
    {
    for( unsigned int x=0; x<_ent.size(); x++ )
        {
        QList<AEntity *>::append( &(_ent[x]) );
        }
    }

/** Create an empty search, that can be added to with the + and = operators.
  */
AEntitySearch::AEntitySearch()
    {
    }

/** Find the entity at index \p x
  */
AEntity &AEntitySearch::operator[]( int x )
    {
    return *at( x );
    }

/** \overload
  */
const AEntity &AEntitySearch::operator[]( int x ) const
    {
    return *at(x);
    }

/** Find the first entity in the list.
  */
AEntity &AEntitySearch::front()
    {
    return *QList<AEntity*>::front();
    }

/** Find the last entity in the list.
  */
AEntity &AEntitySearch::back()
    {
    return *QList<AEntity*>::back();
    }

/** \overload
  */
const AEntity &AEntitySearch::front() const
    {
    return *QList<AEntity*>::front();
    }

/** \overload
  */
const AEntity &AEntitySearch::back() const
    {
    return *QList<AEntity*>::back();
    }

/** Remove an entity from the list.
  */
void AEntitySearch::remove( AEntity *in )
    {
    removeAll( in );
    }

/** Remove the entities in \p in from the list.
  */
void AEntitySearch::remove( const AEntitySearch &in )
    {
    foreach( AEntity *ent, in )
        {
        remove( ent );
        }
    }

/** Append an entity to the list.
  */
void AEntitySearch::append( AEntity *in )
    {
    if( QList<AEntity *>::contains( in ) )
        {
        return;
        }
    QList<AEntity *>::append( in );
    }

/** \overload
  */
void AEntitySearch::append( const AEntitySearch &in )
    {
    // ensure the list is a "set" ( no double values )
    QList<AEntity *>::append( in );
    QSet <AEntity *> set( toSet() );
    clear();
    QList<AEntity *>::append( set.toList() );
    }

/** Find all entities in this search with the property \p in and return as a new search.
  */
AEntitySearch AEntitySearch::findHasProperty( QString in ) const
    {
    AEntitySearch ret;
    for( int x=0; x<size(); x++ )
        {
        if( at(x)->propertyExists( in ) )
            {
            ret.QList<AEntity *>::append( at(x) );
            }
        }
    return ret;
    }

/** Find the entities at the locations \p locs.
  */
AEntitySearch AEntitySearch::findEntitys( QList <AEntityLocation> locs ) const
    {
    AEntitySearch ret;
    // for every location
    for( int z=0; z<locs.size(); z++ )
        {
        // and each entity in the search
        for( int x=0; x<size(); x++ )
            {
            // compare the locations
            if( at(x)->location() == locs[z] )
                {
                // append with uniqueness
                ret.append( at(x) );
                }
            }
        }
    // return the list
    return ret;
    }

/** Find the Entity \t location \p in.
  \todo Surely this can only return 1 entity, so should not return a search?
  */
AEntitySearch AEntitySearch::findEntity( AEntityLocation in ) const
    {
    AEntitySearch ret;
    // search and append if it can find the entity
    for( int x=0; x<size(); x++ )
        {
        if( at(x)->location() == in )
            {
            ret.QList<AEntity *>::append( at(x) );
            }
        }
    return ret;
    }

/** Return true if the search contains the entity
  \todo remove this function, use the QList one...
  */
bool AEntitySearch::contains( AEntity *in ) const
    {
    return QList<AEntity *>::contains( in );
    }

/** \overload
  */
bool AEntitySearch::contains( AEntity &in ) const
    {
    return QList<AEntity *>::contains( &in );
    }

/** Find the direct children of the location \p in.
  \sa AEntityLocation::isDirectChild()
  */
AEntitySearch AEntitySearch::findDirectChildren( AEntityLocation in ) const
    {
    AEntitySearch ret;
    // for each entity in search
    for( int x=0; x<size(); x++ )
        {
        // see if its a child
        if( at(x)->location().isDirectChild( in ) )
            {
            ret.QList<AEntity *>::append( at(x) );
            }
        }
    return ret;
    }


/** Find all the children of the location \p in.
  \sa AEntityLocation::isChild()
  */
AEntitySearch AEntitySearch::findChildren( AEntityLocation in ) const
    {
    AEntitySearch ret;
    // for each entity in search
    for( int x=0; x<size(); x++ )
        {
        // see if its a child
        if( at(x)->location().isChild( in ) )
            {
            ret.QList<AEntity *>::append( at(x) );
            }
        }
    return ret;
    }

/** Find entities at the directory level \p in.
  \sa AEntityLocation::level()
  */
AEntitySearch AEntitySearch::findLevel( unsigned int in ) const
    {
    AEntitySearch ret;
    for( int x=0; x<size(); x++ )
        {
        if( at(x)->location().level() == in )
            {
            ret.QList<AEntity *>::append( at(x) );
            }
        }
    return ret;
    }

/** Find entities of type \p ty.
  */
AEntitySearch AEntitySearch::findType( const AEntityType &ty ) const
    {
    AEntitySearch ret;
    // for each entity
    for( int x=0; x<size(); x++ )
        {
        // compare types
        if( at(x)->type() == ty )
            {
            ret.QList<AEntity *>::append( at(x) );
            }
        }
    return ret;
    }

//----------------------------------------------------------------------------------------------------------------------
//                                                      CONST VERSION
//----------------------------------------------------------------------------------------------------------------------

/** \class AEntitySearchConst
  \brief An AEntitySearchConst allows a user to search through an array in useful different ways.
  \sa AEntitySearch
  */

/** Create a search from the array.
  */
AEntitySearchConst::AEntitySearchConst( const AEntityArray &_ent )
    {
    for( unsigned int x=0; x<_ent.size(); x++ )
        {
        QList<const AEntity *>::append( &(_ent[x]) );
        }
    }

/** Create an empty search.
  */
AEntitySearchConst::AEntitySearchConst()
    {
    }

/** Find the entity at position \p x.
  */
const AEntity &AEntitySearchConst::operator[]( int x ) const
    {
    return *at(x);
    }

/** Find the first entity in the list.
  */
const AEntity &AEntitySearchConst::front() const
    {
    return *QList<const AEntity *>::front();
    }

/** Find the last entity in the list.
  */
const AEntity &AEntitySearchConst::back() const
    {
    return *QList<const AEntity *>::back();
    }

/** Remove the entity \p in from the list.
  */
void AEntitySearchConst::remove( const AEntity *in )
    {
    removeAll( in );
    }

/** Remove the entitys \p in from the list.
  */
void AEntitySearchConst::remove( const AEntitySearchConst &in )
    {
    foreach( const AEntity *ent, in )
        {
        remove( ent );
        }
    }

/** Append the entity \p in to the list.
  */
void AEntitySearchConst::append( const AEntity *in )
    {
    // check for uniqueness
    if( QList<const AEntity *>::contains( in ) )
        {
        return;
        }
    QList<const AEntity *>::append( in );
    }

/** \overload
  */
void AEntitySearchConst::append( const AEntitySearchConst &in )
    {
    // check the list doesnt contain duplicates
    QList<const AEntity *>::append( in );
    QSet <const AEntity *> set( toSet() );
    clear();
    QList<const AEntity *>::append( set.toList() );
    }

/** Find entries in this search with a property called \p in.
  */
AEntitySearchConst AEntitySearchConst::findHasProperty( QString in ) const
    {
    AEntitySearchConst ret;
    for( int x=0; x<size(); x++ )
        {
        // check property
        if( at(x)->propertyExists( in ) )
            {
            // append to list
            ret.QList<const AEntity *>::append( at(x) );
            }
        }
    return ret;
    }

/** Find the entities at the locations \p locs.
  */
AEntitySearchConst AEntitySearchConst::findEntitys( QList <AEntityLocation> locs ) const
    {
    AEntitySearchConst ret;
    // for each location
    for( int z=0; z<locs.size(); z++ )
        {
        // and each entity
        for( int x=0; x<size(); x++ )
            {
            if( at(x)->location() == locs[z] )
                {
                // call the unique checking append.
                ret.append( at(x) );
                }
            }
        }
    return ret;
    }

/** Find the entity at location \p in.
  \todo this shouldnt return a search
  */
AEntitySearchConst AEntitySearchConst::findEntity( AEntityLocation in ) const
    {
    AEntitySearchConst ret;
    for( int x=0; x<size(); x++ )
        {
        if( at(x)->location() == in )
            {
            // append to list
            ret.QList<const AEntity *>::append( at(x) );
            }
        }
    return ret;
    }

/** Return true id the search contains \p in
  */
bool AEntitySearchConst::contains( const AEntity *in ) const
    {
    return QList<const AEntity *>::contains( in );
    }

/** \overload
  */
bool AEntitySearchConst::contains( const AEntity &in ) const
    {
    return QList<const AEntity *>::contains( &in );
    }

/** Find the direct children of location \p in.
  \sa AEntityLocation::isDirectChild()
  */
AEntitySearchConst AEntitySearchConst::findDirectChildren( AEntityLocation in ) const
    {
    AEntitySearchConst ret;
    // check each entity
    for( int x=0; x<size(); x++ )
        {
        // if its a direct child
        if( at(x)->location().isDirectChild( in ) )
            {
            // append to list
            ret.QList<const AEntity *>::append( at(x) );
            }
        }
    return ret;
    }


/** Find all the children of the location \p in.
  \sa AEntityLocation::isChild()
  */
AEntitySearchConst AEntitySearchConst::findChildren( AEntityLocation in ) const
    {
    AEntitySearchConst ret;
    // for each entity in search
    for( int x=0; x<size(); x++ )
        {
        // see if its a child
        if( at(x)->location().isChild( in ) )
            {
            ret.QList<const AEntity *>::append( at(x) );
            }
        }
    return ret;
    }

/** Find entities at the level \p in.
  \sa AEntityLocation::level()
  */
AEntitySearchConst AEntitySearchConst::findLevel( unsigned int in ) const
    {
    AEntitySearchConst ret;
    for( int x=0; x<size(); x++ )
        {
        if( at(x)->location().level() == in )
            {
            ret.QList<const AEntity *>::append( at(x) );
            }
        }
    return ret;
    }

/** Find entities of the type \p ty.
  */
AEntitySearchConst AEntitySearchConst::findType( const AEntityType &ty ) const
    {
    AEntitySearchConst ret;
    for( int x=0; x<size(); x++ )
        {
        // check type
        if( at(x)->type() == ty )
            {
            ret.QList<const AEntity *>::append( at(x) );
            }
        }
    return ret;
    }

/** Add two searches together.
  \relates AEntitySearchConst
  */
AEntitySearchConst operator+( const AEntitySearchConst &a, const AEntitySearchConst &b )
    {
    AEntitySearchConst ret( a );
    ret.append( b );
    return ret;
    }

/** Add two searches together.
  \relates AEntitySearch
  */
AEntitySearch operator+( const AEntitySearch &a, const AEntitySearch &b )
    {
    AEntitySearch ret( a );
    ret.append( b );
    return ret;
    }

/** Returns the entities that are in a and not b.
  \relates AEntitySearch
  */
AEntitySearch operator-( const AEntitySearch &a, const AEntitySearch &b )
    {
    AEntitySearch ret( a );
    ret.remove( b );
    return ret;
    }

/** Returns the entities that are in \p a and not \p b.
  \relates AEntitySearchConst
  */
AEntitySearchConst operator-( const AEntitySearchConst &a, const AEntitySearchConst &b )
    {
    AEntitySearchConst ret( a );
    ret.remove( b );
    return ret;
    }

/** Adds the entities in \p inB to \p inA
  \relates AEntitySearch
  */
AEntitySearchConst &operator+=( AEntitySearchConst &inA, const AEntitySearchConst &inB )
    {
    inA.append( inB );
    return inA;
    }

/** Adds the entities in \p inB to \p inA
  \relates AEntitySearch
  */
AEntitySearch &ALTER_EXPORT operator+=( AEntitySearch &inA, const AEntitySearch &inB )
    {
    inA.append( inB );
    return inA;
    }

/** Adds the entity \p ent to \p inA and returns it
  \relates AEntitySearchConst
  */
AEntitySearchConst ALTER_EXPORT operator+( const AEntitySearchConst &inA, const AEntity *ent )
    {
    AEntitySearchConst ret( inA );
    ret.append( ent );
    return ret;
    }

/** Adds the entity \p ent to \p inA and returns it
  \relates AEntitySearch
  */
AEntitySearch ALTER_EXPORT operator+( const AEntitySearch &inA, AEntity *ent )
    {
    AEntitySearch ret( inA );
    ret.append( ent );
    return ret;
    }

/** Adds the entity \p ent to \p ret
  \relates AEntitySearch
  */
ALTER_EXPORT AEntitySearchConst &operator+=( AEntitySearchConst &ret, const AEntity *ent )
    {
    ret.append( ent );
    return ret;
    }

/** Adds the entity \p ent to \p ret
  \relates AEntitySearch
  */
ALTER_EXPORT AEntitySearch &operator+=( AEntitySearch &ret, AEntity *ent )
    {
    ret.append( ent );
    return ret;
    }
