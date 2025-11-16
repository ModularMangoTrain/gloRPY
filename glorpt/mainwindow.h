#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool connectSerial(const QString &port, int baudRate);

private slots:
    void onDisconnect();
    void onReadData();
    void onSendData();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
};
#endif // MAINWINDOW_H
