#ifndef ALTERSDK_H
#define ALTERSDK_H

#include "AGlobal.h"
#include "ALog.h"
#include "AEntityArray.h"
#include "AEntitySearch.h"
#include "AEntity.h"
#include "QString"
#include "AAbstractFileType.h"



#define ALTER_SDK_INITIALISE( _EXPORT )                 \
extern "C" int _EXPORT alterInitialise( APlugin *sdk )

#define ALTER_SDK_UNINITIALISE( _EXPORT )                          \
extern "C" void _EXPORT alterDeinitialise( APlugin *sdk )

class APrivateWindow;
class AAbstractDock;
class AAbstractToolBar;
class AEntity;
class AProperty;
class AEntityType;
class QAction;

typedef AEntity *(*AEntityCreatorFunction)( );
typedef QWidget *(*guiPropertyTypeFunction)( AProperty * );
typedef bool (*guiPropertyValidatorFunction)( AVariant & );


class ALTER_EXPORT APlugin : public QObject
    {
    Q_OBJECT
public:
    enum pluginResponse { Success, Failure };
    ~APlugin( );
    void addCreator( AEntityType, AEntityCreatorFn, bool show=TRUE );
    void removeCreator( AEntityType );

    void addGuiControlType( int, guiPropertyTypeFunction, guiPropertyValidatorFunction=0 );
    void removeGuiControlType( int );

    void addCore( void * );

    void log( QString );

    AEntityLocation workingPath() const;
    void setWorkingPath( AEntityLocation );

    AEntitySearch search();
    AEntitySearchConst search() const;

    void addEntity( AEntity * );
    void removeEntity( AEntity * );

    bool request( QString );
    void *access( QString );
    template <typename T> T accessAs( QString in )
        {
        return reinterpret_cast <T> ( access( in ) );
        }

    void addDockedItem( QString, AAbstractDock * );
    void removeDockedItem( QString );

    void addToolBar( QString, AAbstractToolBar * );
    void removeToolBar( QString );

    void addFileType( QString, AAbstractFileType * );
    void removeFileType( QString );

    void statusMessage( QString, unsigned int msecs=1000 );

    void selectClear();
    void select( AEntity * );
    void selectAdd( AEntity * );
    AEntitySearch selection( ) const;

    void beginBlockChange();
    void endBlockChange();

    bool inserting() const;

    QByteArray copy( const AEntitySearch &search );
    void paste( QByteArray data );

    void addPreferencesAction( QAction *act );

Q_SIGNALS:
    void workingPathChanged();
    void insertState();
    void cleanState();
    void openState();
    void entityAdded( AEntity * );
    void entityRemoved( AEntity * );
    void selectionChanged();

private:
    /** \cond INTERNAL */
    Q_DISABLE_COPY( APlugin );

    APlugin( APrivateWindow * );
    friend class APrivatePluginDialog;
    friend class APrivateWindow;
    friend class APrivateCustomDockWidget;
    APrivateWindow *_controller;
    /** \endcond */
    };

#endif
