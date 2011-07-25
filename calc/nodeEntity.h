#ifndef NODEENTITY_H
#define NODEENTITY_H

#include "APlugin"
#include "jLib.h"
#include "QPixmap"
#include "synapseSDK.h"

#define NODE_TYPE( cl, dom, nam, col ) A_ENTITY( cl, dom, nam ); static QColor colour() { return col; }

class node;
class nodeEntity;
class nodeRenderer;
class nodeEntityManipulator;
class QPainter;
class QMouseEvent;
typedef QList <nodeEntity *> nodeEntityList;
typedef QList <const nodeEntity *> nodeEntityListConst;
typedef QList <AProperty *> propertyList;
typedef QList <const AProperty *> propertyListConst;

#define INIT_NODE property( "nodeEntity/type" ) = type().name(); property( "nodeEntity/group" ) = type().group(); setColour( colour().redF(), colour().greenF(), colour().blueF() );

class QToolBar;

class SYNPASE_SDK_EXPORT nodeEntity : public AEntity
    {
    Q_OBJECT
    NODE_TYPE( nodeEntity, "synapse.placid-acid.com", "Base Node", QColor( Qt::gray ) )
    XProperty( node *, drawNode, setDrawNode );
    XProperty( nodeRenderer *, exportRenderer, setExportRenderer );
    XROProperty( QPixmap, thumbnail );

public:
    typedef nodeEntityManipulator Manipulator;
    XRORefProperty( QList<Manipulator>, manipulators );

public:
    nodeEntity();
    ~nodeEntity( );

    QString tidyName( ) const;

    void setThumbnail( QImage );

    void addInput( QString, QString="", bool isDefault=FALSE );
    void addAttribute( QString, AVariant, int, QString="" );

    QIcon icon();

    void connect( QString, nodeEntity *, QString );
    void disconnect( QString, AEntity *, QString );
    void disconnect( AEntity * );
    void disconnect( );

    int viewNode() const;
    void setViewNode( int );

    void requestSetViewNode( int );

    virtual void relocated();
    virtual void dislocate( QHash<AEntityLocation, AEntityLocation> & );

    virtual QRectF rect();

    void definitions( QHash <QString, QString> & ) const;

    QString call( QString ) const;

    virtual void initiate();
    virtual void paint( QPainter * );
    virtual void prepareToRender( );
    virtual void prepareToRebuild( );

    nodeEntityList renderDependantNodeNetwork();

    bool isOutputLooselyConnected( nodeEntity * );

    propertyList connectedInputs();
    propertyListConst connectedInputs() const;

    nodeEntityList connectedNodes( QString );
    nodeEntityListConst connectedNodes( QString ) const;
    nodeEntityList connectedNodes( const AProperty & );
    nodeEntityListConst connectedNodes( const AProperty & ) const;

    nodeEntity *connectedNode( const AProperty & );
    const nodeEntity *connectedNode( const AProperty & ) const;
    nodeEntity *connectedNode( QString );
    const nodeEntity *connectedNode( QString ) const;

    propertyList connectedProperties( QString );
    propertyListConst connectedProperties( QString ) const;
    propertyList connectedProperties( const AProperty & );
    propertyListConst connectedProperties( const AProperty & ) const;

    AProperty &connectedProperty( const AProperty & );
    const AProperty &connectedProperty( const AProperty & ) const;
    AProperty &connectedProperty( QString );
    const AProperty &connectedProperty( QString ) const;

    void setColour( float, float, float );

    virtual void addVariables( jLib::graphics::shader::variableSet &set );

    void clearManipulators( );
    const Manipulator &getManipulator( int ) const;
    void setManipulator( int, const Manipulator & );
    virtual void manipulatorChanged( int );
    virtual void initiateManipulators( );

    virtual void setTime( int );

    void resetProperties();

    virtual QToolBar *toolbar();

    virtual bool canUse( QMouseEvent * );
    virtual void use( QMouseEvent * );

    static QString projectDirectory();
    static void setProjectDirectory( QString );

    int getTextureUnit();
    void returnTextureUnit( int );

signals:
    void isViewNodeChanged( );
    void _requestSetViewNode( nodeEntity *, int );
    void onConnection( APropertyDevice *, AProperty * );
    void onDisconnection( APropertyDevice *, AProperty * );
    void needsRebuild();
    void needsUpdate();
    void requestNetworkRedraw();

protected:
    int addManipulator( const Manipulator & );
    void removeManipulator( int x );
    void prePropertyChanged( AProperty * );

private slots:
    void propertiesChanged();
    void entityChanged(APropertyDevice*,AProperty*);

private:
    bool canInline( QString *inlinedSource=0 ) const;
    QIcon *_icon;
    QString parseInput( QString, QString ) const;
    QString parseAttribute( QString ) const;
    QString parsedSource( ) const;
    bool _sourceUnparsed;
    void reparseSource() const;
    mutable QString _pSource;
    QString _oldName;
    typedef QMap <AProperty *, AProperty *> connectionMap;
    connectionMap _connections;
    struct shaderVar {
        shaderVar( ) { }
        shaderVar( jLib::graphics::shader::surfaceShader::variable *v, int t ) : var( v ), type( t ) { }
        jLib::graphics::shader::surfaceShader::variable *var;
        int type;
        };
    QHash <QString, shaderVar> _attributes;

    template <typename T>inline T attribute( QString in )
        {
        jAssert( _attributes[in].var );
        jAssert( dynamic_cast<T>(_attributes[in].var) );
        return static_cast <T> (_attributes[in].var);
        }
    };

#endif // NODEENTITY_H
