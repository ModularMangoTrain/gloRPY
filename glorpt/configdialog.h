#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = nullptr);
    ~ConfigDialog();
    QString getPort() const;
    int getBaudRate() const;
    int getDataBits() const;
    int getParity() const;
    int getStopBits() const;
    int getFlowControl() const;

private:
    Ui::ConfigDialog *ui;
};

#endif
