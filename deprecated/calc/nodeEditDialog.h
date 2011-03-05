#ifndef NODEEDITDIALOG_H
#define NODEEDITDIALOG_H

#include "QDialog"
#include "XColour"

namespace Ui {
    class nodeEditDialog;
}

class XColourWidget;
class scriptEdit;
class nodeEntity;
class QComboBox;
class QLineEdit;
class APlugin;

class nodeEditDialog : public QDialog
    {
    Q_OBJECT
public:
    nodeEditDialog( APlugin *, nodeEntity *, QWidget *parent = 0);
    ~nodeEditDialog();

signals:
    void entityChanged();

private slots:
    void setSource( );
    void addInput( );
    void removeInput( );
    void addAttribute( );
    void removeAttribute( );
    void addAttributeAccepted( );
    void saveNode( );
    void deleteNode( );

private:
    QByteArray saveState();
    void restoreState( const QByteArray & );
    void refreshItems();
    Ui::nodeEditDialog *ui;
    scriptEdit *_script;
    nodeEntity *_controller;
    QComboBox *_attributeTypeCombo;
    QLineEdit *_attributeName;
    APlugin *_app;
    };

#endif // NODEEDITDIALOG_H
