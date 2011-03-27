#ifndef UIPROFILERSURFACE_H
#define UIPROFILERSURFACE_H

#include "QObject"
#include "UISurface.h"
#include "XProfiler"

class QTreeWidget;
class QListWidget;
class QTreeWidgetItem;
class QCheckBox;
class QStackedLayout;
class QAction;

class UIProfilerSurface : public QObject, public UISurface
  {
  Q_OBJECT
public:
  UIProfilerSurface();

private slots:
  void clear();
  void update();
  void chooseFilter();
  void setFilter(QAction*);

private:
  void populateTreeFromContext(const XProfiler::ProfilingContext*, QTreeWidgetItem* parent = 0);
  void populateListFromContext(const XProfiler::ProfilingContext*);

  void populateHashFromContext(const XProfiler::ProfilingContext*, QHash<const char *, XTimeStatistics> &) const;

  void findAllTypes(const XProfiler::ProfilingContext*, QList <xuint32> &) const;

  QList <xuint32> _filteredTypes;
  QTreeWidget *_tree;
  QTreeWidget *_list;
  QCheckBox *_showTree;
  QStackedLayout *_stackedLayout;
  };

#endif // UIPROFILERSURFACE_H
