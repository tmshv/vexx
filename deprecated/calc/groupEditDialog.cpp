#include "groupEditDialog.h"
#include "ui_groupEditDialog.h"
#include "APropertyManager"
#include "nodeEntity.h"
#include "QComboBox"
#include "ASettings"
#include "QInputDialog"
#include "QLabel"
#include "nodeDB.h"
#include "QMessageBox"

groupEditDialog::groupEditDialog( APlugin *app, nodeEntity *controller, QWidget *parent) :
        QDialog(parent), ui(new Ui::groupEditDialog), _controller( controller ), _app( app )
    {
    ui->setupUi(this);

    // add our basic properties from property aware widgets
    ui->dataLayout->addRow( "Name", APropertyManager::createWidget( _controller->property("nodeEntity/type") ) );
    ui->dataLayout->addRow( "Group", APropertyManager::createWidget( _controller->property("nodeEntity/group") ) );

    // add the short help
    QWidget *shortHelp( APropertyManager::createWidget( _controller->property("shortHelp") ) );
    shortHelp->setParent( this );
    ui->shortHelpLayout->addWidget( shortHelp );

    // add the long help
    QWidget *longHelp( APropertyManager::createWidget( _controller->property("longHelp") ) );
    longHelp->setParent( this );
    ui->longHelpLayout->addWidget( longHelp );

    // add a colour widget
    QWidget *col = APropertyManager::createWidget( _controller->property("nodeEntity/colour") );
    col->setParent( this );
    ui->colourLayout->addWidget( col );

    // add a combo box to select the output
    _combo = new QComboBox();
    _combo->addItem( "", "" );
    int select = -1;
    // search for nodes under the selected nodes
    AEntitySearch search( controller->array()->search().findDirectChildren( _controller->location() ) );
    foreach( AEntity *ent, search )
        {
        // if the entity is a node
        nodeEntity *node( ent->castTo<nodeEntity *>() );
        if( node )
            {
            // find nodes with no connected outputs
            nodeEntityList nodeList( node->connectedNodes("output") );
            if( nodeList.size() == 0 )
                {
                // set the selected index if a valid option is in use
                if( _controller->property("callPointer")->toString() == node->name() )
                    {
                    select = _combo->count();
                    }
                // add the combo item
                _combo->addItem( node->name() );
                }
            }
        }
    // select the item decided on
    _combo->setCurrentIndex( select );
    connect( _combo, SIGNAL(activated(int)), this, SLOT(setOutput(int)) );

    // add the combo to the UI
    ui->dataLayout->addRow( "Output", _combo );

    // find the children of the controller
    search = controller->array()->search().findDirectChildren( _controller->location() );
    foreach( AEntity *ent, search )
        {
        nodeEntity *node( ent->castTo<nodeEntity *>() );
        if( node )
            {
            // find all the properties of this node
            foreach( AProperty *prop, node->properties() )
                {
                // if its and input then we can add it to the properties list
                if( prop->attributes().contains("Linkable") && prop->name() != "output" )
                    {
                    nodeEntity *connectedNode( node->connectedNode( *prop ) );
                    if( !connectedNode )
                        {
                        _properties << prop;
                        }
                    }
                }
            }
        }

    // connect the UI to this
    connect( ui->addInput, SIGNAL(clicked()), this, SLOT(addInput()) );
    connect( ui->removeInput, SIGNAL(clicked()), this, SLOT(removeInput()) );

    connect( ui->addAttribute, SIGNAL(clicked()), this, SLOT(addAttribute()) );
    connect( ui->removeAttribute, SIGNAL(clicked()), this, SLOT(removeAttribute()) );

    connect( ui->saveType, SIGNAL(clicked()), this, SLOT(saveNode()) );
    connect( ui->deleteType, SIGNAL(clicked()), this, SLOT(deleteNode()) );

    // restore the settings and geometry from the data array
    ASettings settings( _controller->array(), "floatingPropertiesSettings" );
    restoreState( settings.value( "groupEditState").toByteArray() );

    // update the UI
    updateInputs();
    }

groupEditDialog::~groupEditDialog()
    {
    // save state for node recall later
    ASettings settings( _controller->array(), "floatingPropertiesSettings" );
    settings.setValue("groupEditState", saveState() );
    delete ui;
    }

void groupEditDialog::setOutput( int )
    {
    // set the controller up to the selected option
    _controller->property( "callPointer" ) = _combo->currentText();
    }

void groupEditDialog::updateInputs()
    {
    // clear the ui
    ui->inputs->clear();
    ui->attributes->clear();

    // foreach of the controllers properties
    foreach( AProperty *prop, _controller->properties() )
        {
        // add to the inputs list if its an input
        if( prop->attributes().contains("Linkable") && prop->name() != "output" )
            {
            QListWidgetItem *item( new QListWidgetItem( prop->name() ) );
            item->setData( 32, prop->name() );
            ui->inputs->addItem( item );
            }

        // add to the attributes list if its an attribute
        if( prop->attributes().contains("forwardAttributes") )
            {
            QListWidgetItem *item( new QListWidgetItem( prop->name() ) );
            item->setData( 32, prop->name() );
            ui->attributes->addItem( item );
            }
        }
    }

void groupEditDialog::addInput( )
    {
    QStringList list;

    // foreach property in the pre determined (contructor) list
    foreach( AProperty *prop, _properties )
        {
        AEntity *ent( prop->parent()->castTo<AEntity*>() );

        // add to the list if its an attriubte
        if( ent )
            {
            bool found( FALSE );
            foreach( AProperty *cProp, _controller->properties() )
                {
                if( cProp->attributes().contains("Linkable") &&
                    cProp->name() != "output" &&
                    prop->attribute("inputForwardAttribute")->toString() == cProp->name() )
                    {
                    found = TRUE;
                    }
                }

            if( !found )
                {
                // format the string
                list << ent->name() + " -> " + prop->name();
                }
            }
        }

    // if there are items in the list
    if( list.size() )
        {
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

        _inputName = new QLineEdit( dia );
        layout->addRow( "External Name", _inputName );
        _inputTypeCombo = new QListWidget( dia );
        _inputTypeCombo->setSelectionMode( QAbstractItemView::MultiSelection );
        _inputTypeCombo->addItems( list );
        layout->addRow( "Internal Input", _inputTypeCombo );

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
        connect( dia, SIGNAL(accepted()), this, SLOT(addInputAccepted()) );
        dia->show();
        }
    else
        {
        AWarning << "No inputs available to add";
        }
    }

void groupEditDialog::addInputAccepted()
    {
    // if the input has been accepted
    AProperty *foundProp = 0;
    // if the selected input isnt "", the property doesnt exist in the cointroller, and there is a selected internal input
    if( _inputName->text() != "" && !_controller->propertyExists( _inputName->text() ) && _inputTypeCombo->selectedItems().size() )
        {
        // foreach selected item
        foreach( QListWidgetItem *item, _inputTypeCombo->selectedItems() )
            {
            // search for the property in the found list (constructor)
            foreach( AProperty *prop, _properties )
                {
                AEntity *ent( prop->parent()->castTo<AEntity*>() );
                if( ent && ( ent->name() + " -> " + prop->name() ) == item->text() )
                    {
                    foundProp = prop;
                    }
                }

            // we found the internal property
            if( foundProp )
                {
                // link it up
                _controller->addInput( _inputName->text(), _inputName->text(), false );

                foundProp->attribute("inputForwardAttribute") = _inputName->text();
                }
            }
        // update the entity
        emit entityChanged();
        updateInputs();
        }
    else
        {
        AWarning << "Can't have two inputs or attributes with the same name";
        }
    }

void groupEditDialog::removeInput()
    {
    // find selected items in the UI
    foreach( QListWidgetItem *item, ui->inputs->selectedItems() )
        {
        // search for the properties they want to remove
        AProperty *foundProp = 0;
        foreach( AProperty *prop, _properties )
            {
            AEntity *ent( prop->parent()->castTo<AEntity*>() );
            if( ent && ( ent->name() + " -> " + prop->name() ) == item->text() )
                {
                // remove the actual attribute indicating it a forwarder
                foundProp->removeAttribute("inputForwardAttribute");
                }
            }

        // if the controller has a property equal to the selected list item, remove it
        if( _controller->propertyExists( item->data(32).toString() ) )
            {
            _controller->removeProperty( item->data(32).toString() );
            }
        }

    emit entityChanged();
    updateInputs();
    }

void groupEditDialog::addAttribute()
    {
    nodeEntityList list;
    propertyList pList;
    // for all children of the controller
    AEntitySearch search( _controller->array()->search().findDirectChildren( _controller->location() ) );
    foreach( AEntity *ent, search )
        {
        nodeEntity *node( ent->castTo<nodeEntity *>() );
        if( node )
            {
            // for each properties in the node
            foreach( AProperty *prop, node->properties() )
                {
                // if its an attribute, add it to the list
                if( prop->attributes().contains("Attribute") && !prop->hidden() )
                    {
                    list << node;
                    pList << prop;
                    }
                }
            }
        }

    // if we found any properties
    if( list.size() )
        {
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
        layout->addRow( "External Name", _attributeName );
        _attributeTypeCombo = new QListWidget( dia );
        _attributeTypeCombo->setSelectionMode( QAbstractItemView::MultiSelection );

        for( int x=0; x<list.size(); x++ )
            {
            QListWidgetItem *item( new QListWidgetItem( _attributeTypeCombo ) );
            item->setText( list[x]->name() + " -> " + pList[x]->name() );
            item->setData( 32, list[x]->name() );
            item->setData( 33, pList[x]->name() );
            }

        layout->addRow( "Internal Attributes", _attributeTypeCombo );

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
    else
        {
        AWarning << "No attributes available to add";
        }
    }

void groupEditDialog::addAttributeAccepted()
    {
    // if the accepted dialog has valid values
    if( _attributeName->text() != "" && !_controller->propertyExists( _attributeName->text() ) && _attributeTypeCombo->selectedItems().size() )
        {
        // search the selected items and add them to the list "list"
        XVariantList list;
        foreach( QListWidgetItem *item, _attributeTypeCombo->selectedItems() )
            {
            XVariantList append;
            append << item->data(32).toString() << item->data(33).toString();
            list << XVariant( append );
            }

        // for each child of controller
        AProperty *foundProp = 0;
        AEntitySearch search( _controller->array()->search().findDirectChildren( _controller->location() ) );
        foreach( AEntity *ent, search )
            {
            nodeEntity *node( ent->castTo<nodeEntity *>() );
            if( node )
                {
                // for each property of the nodes
                foreach( AProperty *prop, node->properties() )
                    {
                    // if we find the property in the selected items
                    if( ( ent->name() + " -> " + prop->name() ) == _attributeTypeCombo->selectedItems()[0]->data(0) )
                        {
                        // this property will become the default of the parent property
                        foundProp = prop;
                        }
                    }
                }
            }

        if( foundProp )
            {
            // add the property to the controller
            _controller->addProperty( _attributeName->text(), foundProp->value(), foundProp->type(), _attributeName->text() );
            // setup the attribute for controlling the children properties
            AProperty &addedProp( _controller->property( _attributeName->text() ) );
            addedProp.attribute( "forwardAttributes" ) = list;
            }
        else
            {
            AWarning << "Internal error adding attribute.";
            }

        emit entityChanged();
        updateInputs();
        }
    else
        {
        AWarning << "Can't have two attributes or inputs with the same name";
        }
    }

void groupEditDialog::removeAttribute()
    {
    // search for the attribute, and remove if found
    foreach( QListWidgetItem *item, ui->attributes->selectedItems() )
        {
        if( _controller->propertyExists( item->data(32).toString() ) )
            {
            _controller->removeProperty( item->data(32).toString() );
            }

        // trigger signals and redraws
        emit entityChanged();
        updateInputs();
        }
    }

void groupEditDialog::saveNode()
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

void groupEditDialog::deleteNode()
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


QByteArray groupEditDialog::saveState()
    {
    // save the dialog state for recall with resotreState
    QByteArray ret;
    QDataStream str( &ret, QIODevice::WriteOnly );

    str << geometry();

    return ret;
    }

void groupEditDialog::restoreState( const QByteArray &arr )
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
        }
    }
