#ifndef DIRECTORYSETTINGSDIALOG_H
#define DIRECTORYSETTINGSDIALOG_H

#include <QDialog>
#include "Optimisation.h"

namespace Ui {
class DirectorySettingsDialog;
}

class DirectorySettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DirectorySettingsDialog( QWidget *parent = nullptr);
    ~DirectorySettingsDialog();
    void DirectorySetup(QString workdir);

private slots:
    void accept();

private:
    Ui::DirectorySettingsDialog *ui;
    Optimisation * mData;
};

#endif // DIRECTORYSETTINGSDIALOG_H
