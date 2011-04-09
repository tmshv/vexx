#ifndef ASSETTREE_H
#define ASSETTREE_H

#include "UISurface.h"
#include "QAbstractItemModel"
#include "XEnvironment.h"
#include "QTimer"

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

private slots:
  void contextMenu(QPoint);
  void updateFromRequest(const XEnvironment::Request &);
  void contextMentTriggered(QAction *);

private:
  void requestContainer(XEnvironment::ItemID) const;

  QTreeView* _treeView;
  Application* _app;
  };

#endif // ASSETTREE_H
