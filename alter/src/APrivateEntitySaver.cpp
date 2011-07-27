#include "APrivateEntitySaver.h"
#include "../SDK/ALog"
#include "APrivateWindow.h"
#include "APrivateSystemEntity.h"
#include "QFile"

/** \cond INTERNAL
  */

/** \class APrivateEntitySaver
  \brief A APrivateEntitySaver allows a programmer to save entities to a byte array and then load them back again.
  */

/** Delete the saver.
  */
APrivateEntitySaver::~APrivateEntitySaver()
    {
    // delete our remaining file types.
    foreach( AAbstractFileType *val, _fileTypes.values() )
        {
        delete val;
        }
    }

/** Return the default file suffix for the saver
  */
QString APrivateEntitySaver::defaultFileSuffix() const
    {
    // for each file type
    foreach( AAbstractFileType *f, _fileTypes.values() )
        {
        // and each of its extensions
        foreach( QString e, f->extensions() )
            {
            // and its a valid extension type
            if( e != "" && e != "*" )
                {
                return e;
                }
            }
        }
    return "*";
    }

/** Find if the saver can save and load
  */
bool APrivateEntitySaver::isValid() const
    {
    return _fileTypes.size();
    }

/** Insert the entities in \p in into the array \p array, using the types in \p types and the file type in \p fileType
  \note if \p fileType == 0 then the saver will try to use the default
  */
void APrivateEntitySaver::insert( QByteArray in, AEntityArray &array, const propertyEntityCreators &types, AAbstractFileType *fileType )
    {
    QString ret;
    // if we can load
    if( isValid() )
        {
        // ensure we have a valid loader
        if( fileType == 0 )
            {
            fileType = _fileTypes.values()[0];
            }

        // we shoul dnot have a non 0 pointer, but just in case
        if( fileType != 0 )
            {
            // attempt to parse the string
            fileType->clear();
            fileType->parseFileContents( in );

            // add it to the array
            fillArrayFromFileEngine( array, types, fileType, TRUE );
            }
        }
    }

/** Extract the entities in \p search to a string using the file type \p fileType and return them.
  \note if \p fileType is 0, a default will be used.
  */
QByteArray APrivateEntitySaver::extract( const AEntitySearch &search, AAbstractFileType *fileType ) const
    {
    QByteArray ret;
    // ensure we have savers
    if( isValid() )
        {
        // find a default if one wasnt specified
        if( fileType == 0 )
            {
            fileType = _fileTypes.values()[0];
            }

        // check we now have a saver
        if( fileType != 0 )
            {
            // fill the engine from the search
            fillFileEngineFromSearch( search, fileType, true );

            // find the file contents
            ret = fileType->createFileContents( );
            }
        }

    // return the string
    return ret;
    }

/** Create a list of file filters usable with AFileDialog
  \sa AAbstractFileType::getFilterString()
  */
QString APrivateEntitySaver::fileFilterList() const
    {
    QString extensions;
    int x=0;
    // for each file type
    foreach( AAbstractFileType *f, _fileTypes.values() )
        {
        // add on the string
        extensions += f->getFilterString();

        // and if we are before the end, add on a ;; for Qt to parse
        if( x < ( _fileTypes.size() - 1 ) && extensions != "" )
            {
            extensions += ";;";
            }
        x++;
        }
    return extensions;
    }

/** Install the file type \p in with the name \p name for use.
  */
void APrivateEntitySaver::addFileType( QString name, AAbstractFileType *in )
    {
    _fileTypes.insert( name, in );
    }

/** Remove the file type \p in from the saver.
  \note This doesn't delete the file type, the caller should do this.
  \todo This should probably delete the file type.
  */
void APrivateEntitySaver::removeFileType( QString in )
    {
    _fileTypes.remove( in );
    }

/** Fill the array \p array from the file \p filename, using the file type \p ext ( this should be a filter list from a valid file engine in this saver ), and the creator types \p types.
  */
bool APrivateEntitySaver::fillArrayFromFile( AEntityArray &array, const propertyEntityCreators &types, QString filename, QString ext )
    {
    // extract teh file contents to string.
    QFile inFile( filename );
    if( !inFile.open( QIODevice::ReadOnly ) )
        {
        AWarning << "File Error|Couldn't open file " << filename << " for reading.";
        return FALSE;
        }
    QByteArray content( inFile.readAll() );

    // find an extension pointer from the filter list string
    AAbstractFileType *fileEngine = 0;
    foreach( AAbstractFileType *f, _fileTypes )
        {
        if( ext == f->getFilterString() )
            {
            fileEngine = f;
            break;
            }
        }

    // check we now have a file engine
    if( fileEngine != 0 )
        {
        // empty the file type
        fileEngine->clear();
        // ask it to parse the file content
        fileEngine->parseFileContents( content );

        // and fill the array given from the contents, and return its result
        return fillArrayFromFileEngine( array, types, fileEngine, FALSE );
        }
    return FALSE;
    }

/** Fill a file \p filename from an entity search \p search, using the file type \p ext ( this should be a filter list from a valid file engine in this saver ).
  */
bool APrivateEntitySaver::fillFileFromSearch( const AEntitySearch &search, QString filename, QString ext ) const
    {
    AAbstractFileType *fileEngine = 0;

    // find a file type pointer from the extension string
    foreach( AAbstractFileType *f, _fileTypes )
        {
        if( ext == f->getFilterString() )
            {
            fileEngine = f;
            break;
            }
        }

    // if the file type is non 0
    if( fileEngine != 0 )
        {
        // fill the engine from the search
        fillFileEngineFromSearch( search, fileEngine, false );

        // find the file contents
        QByteArray contents = fileEngine->createFileContents( );

        // open the file for writing
        QFile outFile( filename );
        if( !outFile.open(QIODevice::WriteOnly) )
            {
            AWarning << "File Error|Couldnt open the file " << filename << " for writing.";
            return FALSE;
            }

        // write the content
        outFile.write( contents );
        return TRUE;
        }
    return FALSE;
    }

/** Fills an array from a file engine pointer. \p insert holds whether this is a paste style operation ( add to array ), or an open style operation ( clear and insert ).
  */
bool APrivateEntitySaver::fillArrayFromFileEngine( AEntityArray &pEntities, const propertyEntityCreators &types, const AAbstractFileType *eng, bool insert )
    {
    // if we are opening
    if( !insert )
        {
        // find the system entity
        bool found = FALSE;
        APrivateSystemEntity *sys=0;
        for( unsigned int x=0; x<pEntities.size(); x++ )
            {
            if( pEntities[x].type() == APrivateSystemEntity::identifier() )
                {
                sys = pEntities[x].castTo<APrivateSystemEntity *>();
                }
            }

        // check it exists ( it should )
        if( sys == 0 )
            {
            ALog<<"Load Failed, couldn't find system entity"<<endl;
            return FALSE;
            }

        // test the attributes given for equality with the load attributes, otherwise we arent a compatible file type
        foreach( AAbstractFileType::entity e, eng->entities() )
            {
            if( e.type == APrivateSystemEntity::identifier().hash().toHex() )
                {
                found = TRUE;
                foreach( QString p, e.properties.keys() )
                    {
                    if( p.startsWith( "system" ) && e.properties.value(p).value.toString() != sys->property( p )->toString() )
                        {
                        ALog<<"Files "<<p<<" attribute is not compatible with this application"<<endl;
                        return FALSE;
                        }
                    }
                }
            }

        // if we didnt find the system entity in the file engine it also isnt valid
        if( !found )
            {
            return FALSE;
            }

        //Clear array, only necessary if not inserting...
        for( unsigned int x=0; x<pEntities.size(); x++ )
            {
            if( pEntities[x].type() != APrivateSystemEntity::identifier() )
                {
                pEntities.remove( pEntities[x--] );
                }
            }
        }

    QList <AEntity *> ents;
    QList <AEntity *> inserted;

    // insert the entities into in the array
    foreach( QString e, eng->entities().keys() )
        {
        // find the entity reference
        AAbstractFileType::entity value( eng->entities().value( e ) );
        // if it isnt the system entity ( we dont open this )
        if( value.type != APrivateSystemEntity::identifier().hash().toHex() )
            {
            bool found = FALSE;
            // for each type in our DB
            foreach( const AEntityType t, types.keys() )
                {
                APrivateEntityCreatorFn fn( types.value( t ) );
                // try to find the type of the entity in our types DB
                if( value.type == t.hash().toHex() )
                    {
                    // create an entity of correct type
                    AEntity *ent = (*fn.fn)( );

                    ents << ent;

                    // no emittin signals while we create it
                    ent->blockSignals( TRUE );

                    // add it to the array
                    pEntities.add( *ent );

                    // clear its properties
                    ent->clearProperties();
                    QHash <QString, AAbstractFileType::property> props( value.properties );
                    // add the properties requested
                    foreach( QString propName, props.keys() )
                        {
                        // find the value of the property ( attributes and all )
                        AAbstractFileType::property propValue( props.value( propName ) );

                        // add the proeprty ( dont worry about type, this is an attribute )
                        ent->addProperty( propName, propValue.value );
                        // get a property reference
                        AProperty &prop = ent->property( propName );
                        // no signals while its created
                        prop.blockSignals( TRUE );
                        // add the attributes
                        foreach( QString a, propValue.attributes.keys() )
                            {
                            prop.attribute( a ) = propValue.attributes.value( a );
                            }
                        // now it can emit
                        prop.blockSignals( FALSE );
                        }

                    // now the entity can emit
                    ent->blockSignals( FALSE );

                    inserted << ent;

                    found = TRUE;
                    break;
                    }
                }
            if( !found )
                {
                ALog<<"An entity could not be loaded from file '"<<e<<"' - could a plugin be missing?"<<endl;
                }
            }
        }

    // Make sure we dislocate it if inserting, we dont want old data hanging around
    if( insert )
        {
        QHash <AEntityLocation, AEntityLocation> nameMap;
        QList <AEntity *> doneEntities;

        // process "top level" entnties
        foreach( AEntity *ent, inserted )
            {
            AEntityLocation loc( ent->location() );
            if( loc.level() == 0 )
                {
                AEntityLocation newLocation( pEntities.getUniquePath( ( pEntities.workingPath() + loc ).directory(), loc.name() ) );

                ent->setParent( newLocation.directory() );
                ent->setName( newLocation.name() );

                nameMap.insert( loc, newLocation );
                doneEntities << ent;
                }
            }

        // process child entities
        foreach( AEntity *ent, inserted )
            {
            AEntityLocation loc( ent->location() );
            if( loc.level() > 0 && !doneEntities.contains( ent ) )
                {
                xAssert( nameMap.contains( ent->parent().upToLevel(0) ) );

                AEntityLocation newParent( nameMap[ent->parent().upToLevel(0)] + ent->parent().subtractBasePath(ent->parent().upToLevel(0)) );


                ent->setParent( newParent );

                nameMap.insert( loc, ent->location() );
                }
            }

        foreach( AEntity *ent, ents )
            {
            ent->dislocate( nameMap );
            }
        }

    // Tell the entities theyve been instanciated
        {
        foreach( AEntity *ent, inserted )
            {
            if( ent->type() != APrivateSystemEntity::identifier() )
                {
                // relocate each entity so it can cache local data etc.
                ent->relocated();
                }
            }
        }

    return TRUE;
    }

/** Load an searches data into a file engine.
  */
void APrivateEntitySaver::fillFileEngineFromSearch( const AEntitySearch &search, AAbstractFileType *eng, bool extract ) const
    {
    QHash <QString, AAbstractFileType::entity> &entities = eng->entities();

    // clear any previous data
    entities.clear();

    // for each entity in the search
    foreach( AEntity *e, search )
        {
        AAbstractFileType::entity entity;

        // find the type hash
        entity.type = e->type().hash().toHex();

        // for each property
        foreach( QString propName, e->properties().keys() )
            {
            // get a property reference
            AProperty *propValue( e->properties().value( propName ) );
            AAbstractFileType::property propData;

            // get the proeprty value
            if( extract && propName == "entity/directory" )
                {
                propData.value = AEntityLocation( propValue->value() ).subtractBasePath( e->array()->workingPath() );
                }
            else
                {
                propData.value = propValue->value();
                }

            // add all the attributes
            foreach( QString a, propValue->attributes().keys() )
                {
                propData.attributes[a] = propValue->attributes().value( a );
                }

            // add the property reference to the entity refrerence
            entity.properties.insert( propName, propData );
            }

        // insert the entity reference to the file types array
        entities.insert( e->name(), entity );
        }
    }

/** Find the file plugin from an extension suffix.
  \sa AAbstractFileType::getFilterString()
  */
AAbstractFileType *APrivateEntitySaver::findFileTypeFromExtension( QString suff ) const
    {
    // find all the file types installed
    foreach( AAbstractFileType *f, _fileTypes.values() )
        {
        foreach( QString e, f->extensions() )
            {
            // if the suffix is one of the extensions supported
            if( e == suff )
                {
                // return the filer string
                return f;
                }
            }
        }
    return 0;
    }

/** Find the file plugin from a filter string.
  \sa AAbstractFileType::getFilterString()
  */
AAbstractFileType *APrivateEntitySaver::findFileTypeFromFilterString( QString filter ) const
    {
    // find all the file types installed
    foreach( AAbstractFileType *f, _fileTypes.values() )
        {
        // filter given is this plugins filter string
        if( f->getFilterString() == filter )
            {
            // return the filer string
            return f;
            }
        }
    return 0;
    }
/** \endcond
  */
