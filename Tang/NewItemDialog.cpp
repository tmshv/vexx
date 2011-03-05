#include "NewItemDialog.h"
#include "ui_NewItemDialog.h"
#include "QFileDialog"
#include "XColladaFile.h"
#include "XEnvironment.h"
#include "XGeometry.h"

NewItemDialog::NewItemDialog(XEnvironment *env, QWidget *parent) : QDialog(parent), ui(new Ui::NewItemDialog), _environment(env)
  {
  ui->setupUi(this);

  connect(ui->chooseFilename, SIGNAL(clicked()), this, SLOT(chooseFilename()));

  connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(acceptNewItem()));
  connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(this, SIGNAL(finished(int)), this, SLOT(deleteLater()));
  }

NewItemDialog::~NewItemDialog()
  {
  delete ui;
  }

void NewItemDialog::chooseFilename()
  {
  ui->filename->setText(QFileDialog::getOpenFileName(this, "Choose a collada file"));
  }

void NewItemDialog::acceptNewItem()
  {
  /*if(QFile::exists(ui->filename->text()))
    {
    XColladaFile col(ui->filename->text());
    XGeometry geo( col.geometry( col.geometryNames().front() ) );
    XEnvironmentItemID id = _environment->createItem();

    // set the name
      {
      QByteArray nameArr;
      QDataStream nameStr(&nameArr, QIODevice::WriteOnly);
      nameStr << ui->filename->text();
      _environment->setItemData(Xid, XEnvironmentItem::Name, nameArr);
      _environment->syncItemData(id, XEnvironmentItem::Name);
      }

    XEnvironmentItem item;

    item.shapes() << XShape(geo);
    item.bounds() = geo.computeBounds();

    QByteArray arr;
    QDataStream str(&arr, QIODevice::WriteOnly);
    item.save(str, XEnvironmentItem::Basic);
    _environment->setItemData(id, XEnvironmentItem::Basic, arr);
    _environment->syncItem(id, XEnvironmentItem::Basic);

    arr.clear();
    item.save(str, XEnvironmentItem::Renderable);
    _environment->setItemData(id, XEnvironmentItem::Renderable, arr);
    _environment->syncItem(id, XEnvironmentItem::Renderable);

    accept();
    }
  reject();*/
  }
