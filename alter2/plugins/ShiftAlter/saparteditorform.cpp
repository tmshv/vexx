#include "saparteditorform.h"
#include "ui_saparteditorform.h"

SAPartEditorForm::SAPartEditorForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SAPartEditorForm)
  {
  ui->setupUi(this);
  }

SAPartEditorForm::~SAPartEditorForm()
  {
  delete ui;
  }
