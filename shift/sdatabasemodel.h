#ifndef SDATABASEMODEL_H
#define SDATABASEMODEL_H

#include "QAbstractItemModel"
#include "QItemDelegate"
#include "sobserver.h"
#include "sentityui.h"
#include "sentityweakpointer.h"
#include "XFlags"

class SDatabase;

class SHIFT_EXPORT SDatabaseDelegate : public QItemDelegate
  {
  Q_OBJECT

public:
  SDatabaseDelegate();

  virtual QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
  virtual void setEditorData( QWidget *editor, const QModelIndex &index ) const;
  virtual void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
  QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

private slots:
  void currentItemDestroyed();

private:
  SEntityUI _ui;
  mutable QWidget *_currentWidget;
  mutable QModelIndex _currentIndex;
  };

class SHIFT_EXPORT SDatabaseModel : public QAbstractItemModel, STreeObserver
  {
public:
  enum OptionsFlags
    {
    EntitiesOnly = 1,
    //DisableUpdates = 2,
    ShowValues = 4
    };
  typedef XFlags<OptionsFlags> Options;

  SDatabaseModel(SDatabase *db, SEntity *ent, Options options);

  virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const;
  virtual QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
  virtual QModelIndex parent( const QModelIndex &child ) const;
  virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const;
  virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
  bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

  virtual void onTreeChange(int m, const SChange *);
  virtual void actOnChanges();

  void setOptions(Options options);
  Options options() const;

  virtual Qt::ItemFlags flags(const QModelIndex &index) const;

  void setRoot(SEntity *ent);
  void setDatabase(SDatabase *db, SEntity *ent);

private:
  SDatabase *_db;
  SEntityWeakPointer _root;
  Options _options;
  };

#endif // SDATABASEMODEL_H
