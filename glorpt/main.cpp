#include "mainwindow.h"
#include "configdialog.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    ConfigDialog config;
    if (config.exec() == QDialog::Accepted) {
        MainWindow *w = new MainWindow();
        if (w->connectSerial(config.getPort(), config.getBaudRate())) {
            w->show();
            return a.exec();
        } else {
            QMessageBox::critical(nullptr, "Error", "Failed to open serial port");
            delete w;
        }
    }
    return 0;
}
