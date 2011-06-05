#ifndef ASSETTREE_H
#define ASSETTREE_H

#if 0

#include "UISurface.h"
#include "QAbstractItemModel"
#include "XEnvironment.h"
#include "QTimer"
#include "QIcon"

class QTreeView;
class Application;
class QAction;

class AssetTree : public QAbstractItemModel, public UISurface
  {
  Q_OBJECT
public:
  AssetTree(Application *env);

  virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const;
  virtual QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
  virtual QModelIndex parent( const QModelIndex &child ) const;
  virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const;
  virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

protected:
  virtual void timerEvent(QTimerEvent *event);

private slots:
  void contextMenu(QPoint);
  void updateFromRequest(const XEnvironment::Request &);
  void contextMentTriggered(QAction *);

private:
  XEnvironment::ItemID containerID(const QModelIndex &index) const;
  bool isContainer(const QModelIndex &index) const;
  bool isTexture(const QModelIndex &index) const;
  quint32 indexForID(xuint16 type, XEnvironment::ItemID) const;

  void requestContainer(XEnvironment::ItemID) const;

  QTreeView* _treeView;
  Application* _app;

  QIcon _containerIcon;
  QIcon _textureIcon;

  struct Index
    {
    xuint16 type;
    XEnvironment::ItemID id;
    bool operator==(const Index & i) { return type == i.type && id == i.id; }
    };
  mutable QVector<Index> _indexMap;
  mutable int _cleanupTimerID;
  };

#endif

#endif // ASSETTREE_H
