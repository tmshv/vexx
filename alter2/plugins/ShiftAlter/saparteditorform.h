#ifndef SAPARTEDITORFORM_H
#define SAPARTEDITORFORM_H

#include <QWidget>

namespace Ui {
class SAPartEditorForm;
}

class SAPartEditorForm : public QWidget
  {
  Q_OBJECT
  
public:
  explicit SAPartEditorForm(QWidget *parent = 0);
  ~SAPartEditorForm();
  
private:
  Ui::SAPartEditorForm *ui;
  };

#endif // SAPARTEDITORFORM_H
