#ifndef APPDOCUMENT_H
#define APPDOCUMENT_H

#include "APrivateEntitySaver.h"
#include "APrivateCommandStack.h"
#include "APrivateEntityCreators.h"

/** \cond INTERNAL
  */

class APrivateCommandStack;

class ALTER_EXPORT APrivateDocument : public QObject, public APrivateEntitySaver
    {
    Q_OBJECT
    XROProperty( AEntitySearch, selection );
    XROProperty( bool, inserting );

public:
    APrivateDocument( QString appName, QString orgName, QString domainName, QString version );
    ~APrivateDocument();

    void addCreator( AEntityType, AEntityCreatorFn, bool=TRUE );
    void removeCreator( AEntityType t );

    propertyEntityCreators &entityCreators();
    const propertyEntityCreators &entityCreators() const;

    AEntityLocation addEntity( AEntity * );
    void removeEntity( AEntity * );
    void removeEntities( const AEntitySearch & );

    AEntityArray &propertyEntities();

    void selectAdd( AEntity * );
    void select( AEntity * );
    void select( AEntitySearch );
    void deselect( AEntity * );
    void deselect( );

    APrivateCommandStack &undoStack();
    const APrivateCommandStack &undoStack() const;

    QString organisationName( ) const;
    QString organisationDomain( ) const;
    QString programName( ) const;
    QString programVersion( ) const;

    void saveFile( QString, QString );
    void openFile( QString, QString &ext );
    void newFile( );

    QByteArray cut( const AEntitySearch & );
    QByteArray copy( const AEntitySearch & );
    void paste( QByteArray );

private Q_SLOTS:
    void arrayChange( AEntity * );
    void onCommand( APrivateCommand ** );

Q_SIGNALS:
    void modified( bool );
    void insertState();
    void openState();
    void cleanState();
    void selectionChanged();

private:
    void setModified( bool );
    bool _modified;
    AEntityArray _propertyEntities;
    APrivateCommandStack *_undo;
    propertyEntityCreators _propertyEntityCreators;
    };

/** \endcond
  */
#endif // APPCORE_H
