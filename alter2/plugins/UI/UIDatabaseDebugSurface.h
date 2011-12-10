#ifndef UIDATABASEDEBUGSURFACE_H
#define UIDATABASEDEBUGSURFACE_H

#include "UISurface.h"
#include "sdatabasemodel.h"

class QTreeView;

class UIDatabaseDebugSurface : public QObject, public UISurface
  {
  Q_OBJECT
public:
  UIDatabaseDebugSurface(SDatabase *db=0);
  ~UIDatabaseDebugSurface();

  void setDatabase(SDatabase *db);

private slots:
  void contextMenu(QPoint);
  void disconnectInput();
  void disconnectOutput();
  void copyPath();

private:
  QTreeView *_treeView;
  SDatabase *_db;
  SDatabaseModel _model;
  SDatabaseDelegate *_delegate;
  SProperty *_clickedItem;
  };

#endif // UIDATABASEDEBUGSURFACE_H
