#ifndef VIEWPORTDOCK_H
#define VIEWPORTDOCK_H

#include "AAbstractDock"
#include "viewer.h"
#include "QVBoxLayout"

class PreferencesDialog;
class QComboBox;
class timelineWidget;
class colourSampleWidget;
class colourPickerWidget;
class QToolBar;
class QTimer;
class QToolButton;
class QProgressBar;
class QStackedWidget;
class exportWidget;

class viewerDock : public AAbstractDock
    {
    Q_OBJECT
public:
    viewerDock();
    ~viewerDock();
    void added();

    static void registerColourPicker( colourPickerWidget * );
    static void removeColourPicker( colourPickerWidget * );

    QAction *showPreferences();

    static void addFrame( int now, int min, int max, QImage frame );
    static void installExport( exportWidget * );

private slots:
    void colourPicked( XColour );
    void changeDisplayMode(QString);
    void highlightMode( int );
    void refreshNodeToolbar();
    void skipBegin();
    void playPause( bool );
    void skipEnd();
    void stepFrame();
    void closeRender();
    void toggleSplit();

private:
    viewer *_ui;
    QVBoxLayout *_layout;
    QHBoxLayout *_toolLayout;
    QComboBox *_box;
    timelineWidget *_timeline;
    colourSampleWidget *_colour;
    QToolBar *_nodeTools;
    static QList <colourPickerWidget *> _colourPickers;
    PreferencesDialog *_preferences;
    QTimer *_timer;
    QToolButton *_playButton;
    QToolButton *_closeRender;
    QProgressBar *_progress;
    QStackedWidget *_stack;
    QAction *_split;
    int _splitMode;
    };

#endif // VIEWPORTDOCK_H
