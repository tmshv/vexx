#ifndef GROUPEDITDIALOG_H
#define GROUPEDITDIALOG_H

#include "QDialog"
#include "nodeEntity.h"

namespace Ui {
    class groupEditDialog;
}

class QComboBox;
class QListWidget;
class QLineEdit;

class groupEditDialog : public QDialog {
    Q_OBJECT
public:
    groupEditDialog( APlugin *app, nodeEntity *, QWidget *parent = 0);
    ~groupEditDialog();

signals:
    void entityChanged();

private slots:
    void setOutput(int);
    void addInput();
    void removeInput();
    void addInputAccepted();
    void addAttribute();
    void addAttributeAccepted();
    void removeAttribute();
    void saveNode();
    void deleteNode();

private:
    QLineEdit *_inputName;
    QListWidget *_inputTypeCombo;
    QLineEdit *_attributeName;
    QListWidget *_attributeTypeCombo;
    void updateInputs();
    QByteArray saveState();
    void restoreState( const QByteArray & );
    Ui::groupEditDialog *ui;
    nodeEntity *_controller;
    propertyList _properties;
    QComboBox *_combo;
    APlugin *_app;
};

#endif // GROUPEDITDIALOG_H
