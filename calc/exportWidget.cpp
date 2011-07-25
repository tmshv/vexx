#include "exportWidget.h"
#include "ui_exportWidget.h"
#include "APlugin"
#include "nodeEntity.h"
#include "AFileDialog"
#include "nodeRenderer.h"
#include "QProgressDialog"
#include "QTime"
#include "ALog"
#include "ASettings"
#include "viewerDock.h"
#include "XFileSequence"

exportWidget::exportWidget( nodeEntity *in, QWidget *parent ) : QWidget( parent )
    {
    // set the node were showing
    _node = in;

    // create ui
    _ui = new Ui::exportWidget( );
    _ui->setupUi( this );
    _ui->scrollArea->setWidgetResizable( true );

    ASettings settings( _node->array(), "exportNodeSettings" );

    _ui->directory->setText( settings.value( "directory" ).toString() );
    _ui->minFrame->setValue( settings.value( "maxFrame" ).toInt() );
    _ui->maxFrame->setValue( settings.value( "minFrame" ).toInt() );

    // setup ui maximums and minimums
    _ui->minFrame->setMinimum( -0xFFFF );
    _ui->minFrame->setMaximum( 0xFFFF );
    _ui->maxFrame->setMinimum( -0xFFFF );
    _ui->maxFrame->setMaximum( 0xFFFF );

    // connect this class to ui's signals
    connect( _ui->chooseDirectoryBtn, SIGNAL(clicked()), this, SLOT(chooseDirectory()) );
    connect( _ui->minFrame, SIGNAL(valueChanged(int)), this, SLOT(saveSettings()) );
    connect( _ui->maxFrame, SIGNAL(valueChanged(int)), this, SLOT(saveSettings()) );
    connect( _ui->exportBtn, SIGNAL(clicked()), this, SLOT(doExport()) );
    connect( _ui->previewBtn, SIGNAL(clicked()), this, SLOT(doPreview()) );

    viewerDock::installExport( this );

    _preview = false;
    }

void exportWidget::chooseDirectory( )
    {
    QString possibleDir( AFileDialog::getSaveFileName( tr("Choose Export Filename") ) );
    QString projectsFolder( nodeEntity::projectDirectory() );
    if( possibleDir.startsWith( projectsFolder ) )
        {
        possibleDir = possibleDir.mid( projectsFolder.length() );
        }
    //trigger a dialog to choose the directory we are exporting to
    _ui->directory->setText( possibleDir );
    saveSettings();
    }

void exportWidget::saveSettings()
    {
    ASettings settings( _node->array(), "exportNodeSettings" );

    settings.setValue( "directory", _ui->directory->text() );
    settings.setValue( "maxFrame", _ui->minFrame->value() );
    settings.setValue( "minFrame", _ui->maxFrame->value() );
    }

void exportWidget::doPreview()
    {
    _preview = true;
    doExport();
    }

void exportWidget::doExport( )
    {
    // if we have a valid renderer ( we really should )
    if( _node->exportRenderer() )
        {
        ALog << "Render began at " << QTime::currentTime().toString();

        //Find start frame form ui
        int startFrame( _ui->minFrame->value() );
        int endFrame( _ui->maxFrame->value()  );

        QDir::setCurrent( nodeEntity::projectDirectory() );

        // grab dir from ui, or place in my documents type thing
        QString dir( nodeEntity::projectDirectory() + "/" + QFileInfo( _ui->directory->text() ).dir().absolutePath() );

        if( !QFile::exists( dir ) )
            {
            dir = QFileInfo( _ui->directory->text() ).dir().absolutePath();
            }

        if( dir == "" || !QFile::exists( dir ) )
            {
            AWarning << "The export directory " << dir << " could not be found, " << QDir::homePath() << "will be used instead";
            dir = QDir::homePath();
            }

        //grab file name from ui
        QString fNameBase( QFileInfo( _ui->directory->text() ).fileName() );
        //or assume a default
        if( fNameBase == "" )
            {
            fNameBase = "test.$4F.jpg";
            }

        XFileSequence seq( dir + "/" + fNameBase );

        _cancel = false;
        for( int i=startFrame; i<=endFrame; i++ )
            {
            QApplication::processEvents();
            if( _cancel )
                {
                break;
                }

            // set the renders time
            _node->exportRenderer()->setTime( i );
            // trigger a render and grab the image
            jLib::graphics::pixel::image im( _node->exportRenderer()->texture() );

            viewerDock::addFrame( i, startFrame, endFrame, im.toQImage() );

            if( !_preview )
                {
                QString fName( seq.getFilename( i ) );
                // save out to file
                im.saveFile( fName.toStdString() );
                ALog << "Writing file" << fName;
                }
            }
        _preview = false;

        ALog << "Render finished at " <<  QTime::currentTime().toString();
        }
    else
        {
        // really shouldnt get here...
        AWarning << "No Renderer found.";
        }
    }

void exportWidget::cancelRender()
    {
    _cancel = true;
    }
