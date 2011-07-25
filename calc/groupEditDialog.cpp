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

    _combo = new QComboBox();
    _combo->addItem( "", "" );
    int select = -1;
    AEntitySearch search( controller->array()->search().findDirectChildren( _controller->location() ) );
    foreach( AEntity *ent, search )
        {
        nodeEntity *node( ent->castTo<nodeEntity *>() );
        if( node )
            {
            nodeEntityList nodeList( node->connectedNodes("output") );
            if( nodeList.size() == 0 )
                {
                if( _controller->property("callPointer")->toString() == node->name() )
                    {
                    select = _combo->count();
                    }
                _combo->addItem( node->name() );
                }
            }
        }
    _combo->setCurrentIndex( select );
    connect( _combo, SIGNAL(activated(int)), this, SLOT(setOutput(int)) );


    ui->dataLayout->addRow( "Output", _combo );

    search = controller->array()->search().findDirectChildren( _controller->location() );
    foreach( AEntity *ent, search )
        {
        nodeEntity *node( ent->castTo<nodeEntity *>() );
        if( node )
            {
            foreach( AProperty *prop, node->properties() )
                {
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
    connect( ui->addInput, SIGNAL(clicked()), this, SLOT(addInput()) );
    connect( ui->removeInput, SIGNAL(clicked()), this, SLOT(removeInput()) );

    connect( ui->addAttribute, SIGNAL(clicked()), this, SLOT(addAttribute()) );
    connect( ui->removeAttribute, SIGNAL(clicked()), this, SLOT(removeAttribute()) );

    connect( ui->saveType, SIGNAL(clicked()), this, SLOT(saveNode()) );
    connect( ui->deleteType, SIGNAL(clicked()), this, SLOT(deleteNode()) );

    ASettings settings( _controller->array(), "floatingPropertiesSettings" );
    restoreState( settings.value( "groupEditState").toByteArray() );

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
    _controller->property( "callPointer" ) = _combo->currentText();
    }

void groupEditDialog::updateInputs()
    {
    ui->inputs->clear();
    ui->attributes->clear();

    foreach( AProperty *prop, _controller->properties() )
        {
        if( prop->attributes().contains("Linkable") && prop->name() != "output" )
            {
            QListWidgetItem *item( new QListWidgetItem( prop->name() ) );
            item->setData( 32, prop->name() );
            ui->inputs->addItem( item );
            }

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

    foreach( AProperty *prop, _properties )
        {
        AEntity *ent( prop->parent()->castTo<AEntity*>() );

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
                list << ent->name() + " -> " + prop->name();
                }
            }
        }

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
    AProperty *foundProp = 0;
    if( _inputName->text() != "" && !_controller->propertyExists( _inputName->text() ) && _inputTypeCombo->selectedItems().size() )
        {
        foreach( QListWidgetItem *item, _inputTypeCombo->selectedItems() )
            {
            foreach( AProperty *prop, _properties )
                {
                AEntity *ent( prop->parent()->castTo<AEntity*>() );
                if( ent && ( ent->name() + " -> " + prop->name() ) == item->text() )
                    {
                    foundProp = prop;
                    }
                }

            if( foundProp )
                {
                _controller->addInput( _inputName->text(), _inputName->text(), false );

                foundProp->attribute("inputForwardAttribute") = _inputName->text();
                }
            }
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
    foreach( QListWidgetItem *item, ui->inputs->selectedItems() )
        {
        AProperty *foundProp = 0;
        foreach( AProperty *prop, _properties )
            {
            AEntity *ent( prop->parent()->castTo<AEntity*>() );
            if( ent && ( ent->name() + " -> " + prop->name() ) == item->text() )
                {
                foundProp->removeAttribute("inputForwardAttribute");
                }
            }

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
    AEntitySearch search( _controller->array()->search().findDirectChildren( _controller->location() ) );
    foreach( AEntity *ent, search )
        {
        nodeEntity *node( ent->castTo<nodeEntity *>() );
        if( node )
            {
            foreach( AProperty *prop, node->properties() )
                {
                if( prop->attributes().contains("Attribute") && !prop->hidden() )
                    {
                    list << node;
                    pList << prop;
                    }
                }
            }
        }

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
    if( _attributeName->text() != "" && !_controller->propertyExists( _attributeName->text() ) && _attributeTypeCombo->selectedItems().size() )
        {
        XVariantList list;
        foreach( QListWidgetItem *item, _attributeTypeCombo->selectedItems() )
            {
            XVariantList append;
            append << item->data(32).toString() << item->data(33).toString();
            list << XVariant( append );
            }

        AProperty *foundProp = 0;
        AEntitySearch search( _controller->array()->search().findDirectChildren( _controller->location() ) );
        foreach( AEntity *ent, search )
            {
            nodeEntity *node( ent->castTo<nodeEntity *>() );
            if( node )
                {
                foreach( AProperty *prop, node->properties() )
                    {
                    if( ( ent->name() + " -> " + prop->name() ) == _attributeTypeCombo->selectedItems()[0]->data(0) )
                        {
                        foundProp = prop;
                        }
                    }
                }
            }

        if( foundProp )
            {
            _controller->addProperty( _attributeName->text(), foundProp->value(), foundProp->type(), _attributeName->text() );
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
    foreach( QListWidgetItem *item, ui->attributes->selectedItems() )
        {
        if( _controller->propertyExists( item->data(32).toString() ) )
            {
            _controller->removeProperty( item->data(32).toString() );
            }

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
