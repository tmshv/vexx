#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QMainWindow"
#include "taskdatabase.h"
#include "QModelIndex"

class QTreeView;
class QListWidget;
class QLineEdit;
class QTextEdit;
class QDoubleSpinBox;

class MainWindow : public QMainWindow, SConnectionObserver, STreeObserver
  {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void itemChosen(QModelIndex);
  void contextMenu(QPoint);
  void save();
  void addItem();
  void removeItem();
  void addDependancy();
  void removeDependancy();

private:
  virtual void onTreeChange(const SChange *);
  virtual void onConnectionChange(const SChange *);
  virtual void actOnChanges();

  void updateUIFromDB();
  void updateLinks();

  TaskDatabase _db;
  SDatabaseModel *_dbModel;
  QTreeView *_itemTree;
  QListWidget *_links;
  Item *_item;

  QLineEdit *_title;
  QTextEdit *_description;
  QDoubleSpinBox *_priority;
  QDoubleSpinBox *_severity;
  QModelIndex _itemHit;

  bool _updateLinks;
  bool _nullifyItem;
  };

#endif // MAINWINDOW_H
