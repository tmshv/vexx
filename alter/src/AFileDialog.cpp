#include "../SDK/AFileDialog"
#include "QSettings"
#include "QApplication"
#include "../SDK/ALog"

/** \class AFileDialog
  \brief Similar to a QFileDialog, except it remembers its position and location from its last use.
  */

/** Create an AFileDialog
  */
AFileDialog::AFileDialog( const QString &caption, const QString &directory, const QString &filter )
        : QFileDialog( 0, caption, directory, filter )
    {
    QSettings settings;
    QByteArray arr( settings.value( "fileDialog" ).toByteArray() );

    QRect oldRct( geometry() );
    QRect lgRct( oldRct );
    if( QApplication::activeWindow() )
        {
        lgRct = QApplication::activeWindow()->normalGeometry();
        }
    oldRct.moveCenter( lgRct.center() );

    QRect rect( settings.value( "fileDialogRect", oldRct ).toRect() );
    if( !arr.isEmpty() )
        {
        restoreState( arr );
        }
    setGeometry( rect );
    }

/** Destory the AFileDialog
  */
AFileDialog::~AFileDialog( )
    {
    QSettings settings;
    settings.setValue( "fileDialogRect", geometry() );
    settings.setValue( "fileDialog", saveState() );
    }

/** Get the name of an existing filename to open
  */
QString AFileDialog::getOpenFileName( const QString &caption,
                               const QString &filter,
                               QString *selectedFilter,
                               Options options )
    {
    // create a qt dialog
    AFileDialog dialog;
    dialog.setWindowTitle( caption );
    dialog.setNameFilter( filter );
    dialog.setFileMode( ExistingFile );
    dialog.setOptions( options );

    if (selectedFilter)
        {
        dialog.selectNameFilter(*selectedFilter);
        }

    if ( dialog.exec() == QDialog::Accepted )
        {
        if (selectedFilter)
            {
            *selectedFilter = dialog.selectedFilter();
            }
        return dialog.selectedFiles().value(0);
        }
    return QString();
    }

/** Get a list of names of existing files to open
  */
QStringList AFileDialog::getOpenFileNames( const QString &caption,
                                          const QString &filter,
                                          QString *selectedFilter,
                                          Options options )
    {
    // create a qt dialog
    AFileDialog dialog;
    dialog.setWindowTitle( caption );
    dialog.setNameFilter( filter );
    dialog.setFileMode( ExistingFile );
    dialog.setOptions( options );

    if (selectedFilter)
        {
        dialog.selectNameFilter(*selectedFilter);
        }

    if ( dialog.exec() == QDialog::Accepted )
        {
        if (selectedFilter)
            {
            *selectedFilter = dialog.selectedFilter();
            }
        return dialog.selectedFiles();
        }
    return QStringList();
    }

/** This is a convenience static function that will return a file name selected by the user. The file does not have to exist.
  */
QString AFileDialog::getSaveFileName( const QString &caption,
                                     const QString &filter,
                                     QString *selectedFilter,
                                     Options options )
    {
    // create a qt dialog
    AFileDialog dialog;
    dialog.setWindowTitle( caption );
    dialog.setNameFilter( filter );
    dialog.setFileMode( AnyFile );
    dialog.setOptions( options );
    dialog.setAcceptMode( QFileDialog::AcceptSave );

    if (selectedFilter)
        {
        dialog.selectNameFilter(*selectedFilter);
        }

    if ( dialog.exec() == QDialog::Accepted )
        {
        if (selectedFilter)
            {
            *selectedFilter = dialog.selectedFilter();
            }
        return dialog.selectedFiles().value(0);
        }
    return QString();
    }

/** This is a convenience static function that will return an existing directory selected by the user.
  */
QString AFileDialog::getExistingDirectory( const QString &caption,
                                          Options options )
    {
    // create a qt dialog
    AFileDialog dialog;
    dialog.setWindowTitle( caption );
    dialog.setFileMode( (options & ShowDirsOnly ? DirectoryOnly : Directory) );
    dialog.setOptions( options );

    if ( dialog.exec() == QDialog::Accepted )
        {
        return dialog.selectedFiles().value(0);
        }
    return QString();
    }
