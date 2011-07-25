#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
    class PreferencesDialog;
}

class PreferencesDialog : public QDialog
    {
    Q_OBJECT
public:
    PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

protected:
    void showEvent(QShowEvent *);

private slots:
    void setup();
    void chooseProjectDirectory();
    void chooseNodeDirectory();

private:
    Ui::PreferencesDialog *ui;
    };

#endif // PREFERENCESDIALOG_H
