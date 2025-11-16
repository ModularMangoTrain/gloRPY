#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    setWindowTitle("gloRPY - New Connection");
    ui->dataBitsCombo->setCurrentIndex(1);
    ui->baudCombo->setCurrentIndex(4);
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

QString ConfigDialog::getPort() const
{
    return ui->portCombo->currentText();
}

int ConfigDialog::getBaudRate() const
{
    return ui->baudCombo->currentText().toInt();
}

int ConfigDialog::getDataBits() const
{
    return ui->dataBitsCombo->currentText().toInt();
}

int ConfigDialog::getParity() const
{
    return ui->parityCombo->currentIndex();
}

int ConfigDialog::getStopBits() const
{
    return ui->stopBitsCombo->currentIndex() + 1;
}

int ConfigDialog::getFlowControl() const
{
    return ui->flowControlCombo->currentIndex();
}
