#include "nodeEditDialog.h"
#include "ui_nodeEditDialog.h"
#include "scriptEdit.h"
#include "XColourWidget"
#include "nodeEntity.h"
#include "APropertyManager"
#include "XColour"
#include "QInputDialog"
#include "QComboBox"
#include "QLabel"
#include "QMessageBox"
#include "QDataStream"
#include "ASettings"
#include "nodeDB.h"
#include "colourPickerWidget.h"

// a node edit dialog edits an XML based node and allows saving and deletion of them
nodeEditDialog::nodeEditDialog( APlugin *app, nodeEntity *node, QWidget *parent ) :
    QDialog(parent), ui(new Ui::nodeEditDialog), _controller( node ), _app( app )
    {
    // this shouldnt be possible to cause, but in case... ou cant edit a C++ node
    Q_ASSERT( node->property("editable")->toBool() );
    // setup the ui
    ui->setupUi(this);

    // add our basic properties from property aware widgets
    ui->dataLayout->addRow( "Name", APropertyManager::createWidget( _controller->property("nodeEntity/type") ) );
    ui->dataLayout->addRow( "Group", APropertyManager::createWidget( _controller->property("nodeEntity/group") ) );

    // add a script editor
    _script = new scriptEdit( this );
    _script->setPlainText( _controller->property("nodeEntity/source")->toString() );
    ui->sourceLayout->addWidget( _script );
    connect( _script, SIGNAL(textChanged()), this, SLOT(setSource()) );

    // add a colour widget
    QWidget *col = APropertyManager::createWidget( _controller->property("nodeEntity/colour") );
    col->setParent( this );
    ui->colourLayout->addWidget( col );

    // add the short help
    QWidget *shortHelp( APropertyManager::createWidget( _controller->property("shortHelp") ) );
    shortHelp->setParent( this );
    ui->shortHelpLayout->addWidget( shortHelp );

    // add the long help
    QWidget *longHelp( APropertyManager::createWidget( _controller->property("longHelp") ) );
    longHelp->setParent( this );
    ui->longHelpLayout->addWidget( longHelp );

    // connect buttons to this
    connect( ui->addInput, SIGNAL(clicked()), this, SLOT(addInput()) );
    connect( ui->removeInput, SIGNAL(clicked()), this, SLOT(removeInput()) );
    connect( ui->addAttribute, SIGNAL(clicked()), this, SLOT(addAttribute()) );
    connect( ui->removeAttribute, SIGNAL(clicked()), this, SLOT(removeAttribute()) );

    // save and delete type
    connect( ui->saveType, SIGNAL(clicked()), this, SLOT(saveNode()) );
    connect( ui->deleteType, SIGNAL(clicked()), this, SLOT(deleteNode()) );

    refreshItems();

    // used to retrieve and store settings
    ASettings settings( _controller->array(), "floatingPropertiesSettings" );
    restoreState( settings.value( "nodeEditState").toByteArray() );
    }

void nodeEditDialog::setSource( )
    {
    // triggered when the source has been changed, we set the property
    _controller->property("nodeEntity/source") = _script->toPlainText();
    }

nodeEditDialog::~nodeEditDialog()
    {
    // save state for node recall later
    ASettings settings( _controller->array(), "floatingPropertiesSettings" );
    settings.setValue("nodeEditState", saveState() );

    // delete ui
    delete ui;
    }

void nodeEditDialog::refreshItems()
    {
    // refresh the inputs and attributes lists
    ui->inputs->clear();
    ui->attributes->clear();
    // for each property
    foreach( AProperty *prop, _controller->properties() )
        {
        if( prop->attributes().contains("Linkable") && prop->name() != "output" )
            {
            // add input
            ui->inputs->addItem( prop->name() );
            }
        else if( prop->attributes().contains("Attribute") )
            {
            // add attribute
            ui->attributes->addItem( prop->name() );
            }
        }
    }

void nodeEditDialog::addInput( )
    {
    // triggered when a user wants to add an input
    bool ok( false );
    QString name( QInputDialog::getText( 0, "Add Input", "Name for input", QLineEdit::Normal, "", &ok ) );

    // if the user clicked on
    if( ok && name != "" )
        {
        // add the input
        _controller->addInput( name, FALSE );
        emit entityChanged();
        refreshItems();
        }
    }

void nodeEditDialog::removeInput( )
    {
    // for each selected item
    foreach( QListWidgetItem *item, ui->inputs->selectedItems() )
        {
        // try to find the property and delete it
        foreach( AProperty *prop, _controller->properties() )
            {
            if( prop->attributes().contains("Linkable") && item->text() == prop->displayName() )
                {
                // remove the property
                _controller->removeProperty( prop->name() );
                break;
                }
            }
        // emit the changed signal
        emit entityChanged();
        refreshItems();
        }
    }

void nodeEditDialog::addAttribute( )
    {
    QStringList list;

    // add the possible types to the list ( could be less hard coded )
    list << "Double" << "Colour" << "2D Vector" << "3D Vector" << "4D Vector";

    // make a dialog
    QDialog *dia( new QDialog( 0 ) );
    dia->setModal( true );

    // layout for the system
    QVBoxLayout *masterLayout( new QVBoxLayout( dia ) );

    // add a label
    masterLayout->addWidget( new QLabel( "Specify the attribute to add", dia ) );

    // add name and type inputs
    QFormLayout *layout( new QFormLayout( ) );
    masterLayout->addLayout( layout );

    _attributeName = new QLineEdit( dia );
    layout->addRow( "Name", _attributeName );
    _attributeTypeCombo = new QComboBox( dia );
    _attributeTypeCombo->addItems( list );
    layout->addRow("Type", _attributeTypeCombo );

    // add buttons for ok and cancel
    QHBoxLayout *buttonLayout( new QHBoxLayout( ) );
    masterLayout->addLayout( buttonLayout );

    buttonLayout->addStretch();

    QPushButton *ok( new QPushButton( "&Ok", dia ) );
    buttonLayout->addWidget( ok );
    connect( ok, SIGNAL(clicked()), dia, SLOT(accept()) );
    QPushButton *cancel( new QPushButton( "&Cancel", dia ) );
    buttonLayout->addWidget( cancel );
    connect( cancel, SIGNAL(clicked()), dia, SLOT(reject()) );

    // we want a signal when (if) the dialog is accepted
    connect( dia, SIGNAL(accepted()), this, SLOT(addAttributeAccepted()) );
    dia->show();
    }

void nodeEditDialog::addAttributeAccepted()
    {
    // add a new dialog
    if( _attributeTypeCombo->currentText() == "Colour" )
        {
        // add a colour
        _controller->addAttribute( _attributeName->text(), 0.0, calcPropertyInfo::Colour, _attributeName->text() );
        }
    else if( _attributeTypeCombo->currentText() == "2D Vector" )
        {
        // add a position
        _controller->addAttribute( _attributeName->text(), 0.0, propertyInfo::Component2D, _attributeName->text() );
        }
    else if( _attributeTypeCombo->currentText() == "3D Vector" )
        {
        // add a position
        _controller->addAttribute( _attributeName->text(), 0.0, propertyInfo::Component3D, _attributeName->text() );
        }
    else if( _attributeTypeCombo->currentText() == "4D Vector" )
        {
        // add a position
        _controller->addAttribute( _attributeName->text(), 0.0, propertyInfo::Component4D, _attributeName->text() );
        }
    else
        {
        // else add a double
        _controller->addAttribute( _attributeName->text(), 0.0, propertyInfo::Double, _attributeName->text() );
        }
    emit entityChanged();
    refreshItems();
    }

void nodeEditDialog::removeAttribute( )
    {
    // remove each attribute in the selected list
    foreach( QListWidgetItem *item, ui->attributes->selectedItems() )
        {
        // try to find the attributes
        foreach( AProperty *prop, _controller->properties() )
            {
            if( prop->attributes().contains("Attribute") && item->text() == prop->displayName() )
                {
                // if we found them, remove them
                _controller->removeProperty( prop->name() );
                break;
                }
            }
        }
    // entity changed
    emit entityChanged();
    refreshItems();
    }

void nodeEditDialog::saveNode()
    {
    // save the node to the database
    bool found( false );
    // try to find the node
    nodeDB::nodeType n( nodeDB::findType( _controller->property( "nodeEntity/group")->toString(), _controller->property( "nodeEntity/type")->toString(), &found ) );

    // if we found it, ask if they want to save it
    if( found )
        {
        QMessageBox msgBox;
        msgBox.setText( "Do you want to overwrite the existing node type?" );
        msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton( QMessageBox::No );
        int ret = msgBox.exec( );
        if( ret == QMessageBox::No )
            {
            // user denied, return
            return;
            }
        }

    // save node
    nodeDB::saveNode( _app, _controller );
    }

void nodeEditDialog::deleteNode( )
    {
    // try to find th enode
    bool found( false );
    nodeDB::nodeType n( nodeDB::findType( _controller->property( "nodeEntity/group")->toString(), _controller->property( "nodeEntity/type")->toString(), &found ) );

    // if they found it
    if( found )
        {
        QMessageBox msgBox;
        msgBox.setText( "Do you want to delete this node type?" );
        msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton( QMessageBox::No );
        int ret = msgBox.exec( );
        if( ret == QMessageBox::No )
            {
            // user denied, return
            return;
            }
        // delete the node
        nodeDB::deleteNode( n );
        }
    else
        {
        // node wasnt found, inform user we cant delete it.
        QMessageBox msgBox;
        msgBox.setText("This node type " + _controller->property( "nodeEntity/group")->toString() + " - " +  _controller->property( "nodeEntity/type")->toString() + "doesn't exist.");
        msgBox.exec();
        }
    }

QByteArray nodeEditDialog::saveState()
    {
    // save the dialog state for recall with resotreState
    QByteArray ret;
    QDataStream str( &ret, QIODevice::WriteOnly );

    str << geometry() << ui->splitter->saveState();

    return ret;
    }

void nodeEditDialog::restoreState( const QByteArray &arr )
    {
    // if its not an empty byte array
    if( !arr.isEmpty() )
        {
        // extract a rect and splitter from it
        QByteArray tmp( arr );
        QDataStream str( &tmp, QIODevice::ReadOnly );

        QRect geo;
        QByteArray splitter;

        str >> geo >> splitter;

        // restore data
        setGeometry( geo );
        ui->splitter->restoreState( splitter );
        }
    }
