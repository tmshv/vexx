#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include "QDialog"
#include "XProperty"

class nodeEntity;

namespace Ui {
    class exportWidget;
}

class exportWidget : public QWidget
    {
    Q_OBJECT
public:
    exportWidget( nodeEntity *, QWidget *parent=0 );

private slots:
    void chooseDirectory();
    void doExport();
    void doPreview();
    void saveSettings();
    void cancelRender();

private:
    Ui::exportWidget *_ui;
    nodeEntity *_node;
    bool _cancel;
    bool _preview;
    };

#endif // EXPORTDIALOG_H
