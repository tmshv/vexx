#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
#include "AFileDialog"
#include "nodeEntity.h"
#include "nodeDB.h"
#include "ALog"

PreferencesDialog::PreferencesDialog( QWidget *parent )
        : QDialog( parent ), ui( new Ui::PreferencesDialog )
    {
    ui->setupUi(this);

    connect( ui->projectDirectoryChooseButton, SIGNAL(clicked()), this, SLOT(chooseProjectDirectory()) );
    connect( ui->nodeDirectoryChooseButton, SIGNAL(clicked()), this, SLOT(chooseNodeDirectory()) );
    connect( ui->buttonBox, SIGNAL(accepted()), this, SLOT(setup()) );
    connect( ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()) );
    }

PreferencesDialog::~PreferencesDialog()
    {
    delete ui;
    }

void PreferencesDialog::showEvent( QShowEvent * )
    {
    ui->projectDirectory->setText( nodeEntity::projectDirectory() );
    ui->nodeDirectory->setText( nodeDB::nodeDirectory() );
    }

void PreferencesDialog::chooseProjectDirectory()
    {
    ui->projectDirectory->setText( AFileDialog::getExistingDirectory( "Choose project directory" ) );
    }

void PreferencesDialog::chooseNodeDirectory()
    {
    ui->nodeDirectory->setText( AFileDialog::getExistingDirectory( "Choose node directory" ) );
    }

void PreferencesDialog::setup()
    {
    nodeEntity::setProjectDirectory( ui->projectDirectory->text() );
    if( ui->nodeDirectory->text() != nodeDB::nodeDirectory() )
        {
        nodeDB::setNodeDirectory( ui->nodeDirectory->text() );
        AWarning << "You may need to restart synapse in order to reload the non-script nodes";
        }
    close();
    }
