#ifndef AFILEDIALOG_H
#define AFILEDIALOG_H

#include <QFileDialog>
#include "AGlobal.h"

class ALTER_EXPORT AFileDialog : public QFileDialog
    {
public:
    AFileDialog( const QString &caption=QString(), const QString &directory=QString(), const QString &filter=QString() );
    ~AFileDialog( );

    static QString getExistingDirectory( const QString &caption=QString(),
                                         Options options=ShowDirsOnly );
    static QString getOpenFileName( const QString &caption=QString(),
                                    const QString &filter=QString(),
                                    QString *selectedFilter=0,
                                    Options options=0 );
    static QStringList getOpenFileNames( const QString &caption=QString(),
                                         const QString & filter=QString(),
                                         QString *selectedFilter=0,
                                         Options options=0 );
    static QString getSaveFileName( const QString &caption=QString(),
                                    const QString &filter=QString(),
                                    QString *selectedFilter=0,
                                    Options options=0 );
    };

#endif // AFILEDIALOG_H
