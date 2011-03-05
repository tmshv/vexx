#include "IneFixtureEditDialog.h"
#include "IneFixture.h"
#include "QTabWidget"
#include "QLineEdit"
#include "QFormLayout"
#include "sentityui.h"
#include "sproperty.h"
#include "QPushButton"
#include "QComboBox"
#include "XColour"

IneFixtureEditDialog::IneFixtureEditDialog(QWidget *parent) : QDialog(parent), _fixture(0)
  {
  setModal(true);

  _ui = new SEntityUI();

  QVBoxLayout *layout(new QVBoxLayout(this));
  layout->setContentsMargins(5,5,5,5);

  _tabs = new QTabWidget(this);
  layout->addWidget(_tabs);

  QHBoxLayout *subLayout(new QHBoxLayout());
  layout->addLayout(subLayout);

  QPushButton *add(new QPushButton("&Add", this));
  connect(add, SIGNAL(clicked()), this, SLOT(add()));
  subLayout->addWidget(add);

  QPushButton *remove(new QPushButton("&Remove", this));
  connect(remove, SIGNAL(clicked()), this, SLOT(remove()));
  subLayout->addWidget(remove);

  subLayout->addStretch();


  QPushButton *close(new QPushButton("&Close", this));
  connect(close, SIGNAL(clicked()), this, SLOT(close()));
  subLayout->addWidget(close);
  }

IneFixtureEditDialog::~IneFixtureEditDialog()
  {
  delete _ui;
  }

void IneFixtureEditDialog::setFixture(IneFixture *in)
  {
  if(_fixture)
    {
    _fixture->removeDataObserver(this);
    }
  _fixture = in;
  if(_fixture)
    {
    _fixture->addDataObserver(this);
    }
  refreshUI();
  }

void IneFixtureEditDialog::onDataChange( const SDataEvent &data )
  {
  if(_fixture)
    {
    if(data.property() >= IneFixture::LastProperty && data.attribute() == SProperty::Name)
      {
      for(int i=1, s=_tabs->count(); i<s; ++i)
        {
        SProperty param(_fixture->property(i + IneFixture::LastProperty - 1));
        _tabs->setTabText(i, param.name());
        }
      }
    }
  }

void IneFixtureEditDialog::setType(int i)
  {
  if(_fixture)
    {
    xuint32 index = IneFixture::LastProperty + _tabs->currentIndex() - 1;
    SProperty param(_fixture->property(index));
    if(i == 0)
      {
      param = 0.0f;
      }
    else if(i == 1)
      {
      param = XVector2D();
      }
    else if(i == 2)
      {
      param = XColour();
      }
    foreach(QObject *obj, _fixtureValueHolder->children())
      {
      if(obj->isWidgetType())
        {
        obj->deleteLater();
        }
      }

    _fixtureValueHolder->layout()->addWidget(_ui->createControlWidget(param));
    }
  }

void IneFixtureEditDialog::refreshUI()
  {
  if(_fixture)
    {
    setWindowTitle(_fixture->name());
    for(int i=0, s=_tabs->count(); i<s; ++i)
      {
      _tabs->setCurrentIndex(i);
      QWidget *tab = _tabs->currentWidget();
      tab->deleteLater();
      }

    QWidget *general(new QWidget);
    QFormLayout *form(new QFormLayout(general));

    form->addRow("Manufacturer", _ui->createControlWidget(_fixture->property(IneFixture::Manufacturer)));
    form->addRow("Name", _ui->createControlWidget(_fixture->property(IneFixture::Name)));

    _tabs->addTab(general, "General");

    for(xuint32 i=IneFixture::LastProperty, s=_fixture->numberOfProperties(); i<s; ++i)
      {
      SProperty param(_fixture->property(i));
      QWidget *tab(new QWidget(this));
      QFormLayout *paramForm(new QFormLayout(tab));

      paramForm->addRow("Name", _ui->createControlWidget(param.attribute(SProperty::Name)));

      QComboBox *typeCombo(new QComboBox);
      typeCombo->addItem("Scalar");
      typeCombo->addItem("Pan Tilt");
      typeCombo->addItem("Colour");
      if(param.value().type() == XMetaType::typeOf<xReal>())
        {
        typeCombo->setCurrentIndex(0);
        }
      if(param.value().type() == XMetaType::typeOf<XVector2D>())
        {
        typeCombo->setCurrentIndex(1);
        }
      if(param.value().type() == XMetaType::typeOf<XColour>())
        {
        typeCombo->setCurrentIndex(2);
        }
      connect(typeCombo, SIGNAL(activated(int)), this, SLOT(setType(int)));
      paramForm->addRow("Type", typeCombo);

      _fixtureValueHolder = new QWidget();
      QVBoxLayout *layout(new QVBoxLayout(_fixtureValueHolder));
      layout->setContentsMargins(0,0,0,0);
      layout->addWidget(_ui->createControlWidget(param));

      paramForm->addRow("Default Value", _fixtureValueHolder);
      _tabs->addTab(tab, param.name());
      }
    }
  }

void IneFixtureEditDialog::add()
  {
  if(_fixture)
    {
    _fixture->addParameter("New Parameter");
    refreshUI();
    }
  }

void IneFixtureEditDialog::remove()
  {
  if(_fixture)
    {
    int index = _tabs->currentIndex();
    QString text = _tabs->tabText(index);
    _fixture->removeParameter(text);
    refreshUI();
    }
  }
