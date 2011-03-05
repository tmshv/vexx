#ifndef SOURCENODE_H
#define SOURCENODE_H

#include "APlugin"
#include "QList"
#include "QObject"

class QString;
class nodeEntity;
class QFileInfo;
class QVariant;
class QIcon;

namespace nodeDB {

class nodeType
    {
public:
    nodeType( );
    nodeType( AEntityType, AEntityCreatorFn, QColor );
    nodeType( QString group, QString type, QColor );
    nodeType( QString file, QString group, QString name, QString icon, QColor );
    nodeType( QVariant );
    nodeType( const nodeType & );
    ~nodeType();

    QColor colour() const;
    QString group() const;
    QString name() const;
    QIcon icon() const;
    QString file() const;

    bool isValid();

    nodeEntity *createAndInsert( APlugin * );

    operator QVariant();
    bool operator<( const nodeType & ) const;
    bool operator==( const nodeType & ) const;
    nodeType &operator=( const nodeType & );

private:
    nodeEntity *create( APlugin * );
    QString _file;
    QString _group;
    QString _name;
    QString _icon;
    AEntityType *_type;
    AEntityCreatorFn _fn;
    QColor _colour;
    };

class nodeTypesWatcher : public QObject
    {
    Q_OBJECT
public:
    nodeTypesWatcher( QObject *parent=0 );
    ~nodeTypesWatcher();

    void triggerTypesChanged();
signals:
    void typesChanged();
    };

void initiateLibraryNodes( APlugin * );

void setNodeDirectory( QString );
QString nodeDirectory();

void saveNode( APlugin *app, nodeEntity * );
void deleteNode( nodeType );

nodeType findType( QString group, QString type, bool *found=0 );

QList <QFileInfo> getXmlNodeFiles();

QList <QFileInfo> getNodeFiles();

QStringList nodeGroups();

QList <nodeType> listNodeGroup( QString );

QFileInfo fileFromNodeType( QString );

#ifndef SYNPASE_SDK_EXPORT
# ifdef CALCV2_LIBRARY
#  define SYNPASE_SDK_EXPORT Q_DECL_EXPORT
# else
#  define SYNPASE_SDK_EXPORT Q_DECL_IMPORT
# endif
#endif

void SYNPASE_SDK_EXPORT registerNode( APlugin *, AEntityType, AEntityCreatorFn, QColor );
void SYNPASE_SDK_EXPORT unRegisterNode( APlugin *, AEntityType );

};

#endif // SOURCENODE_H
