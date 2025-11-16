#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCursor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial(new QSerialPort(this))
{
    ui->setupUi(this);
    setWindowTitle("gloRPY - Serial Terminal");
    ui->terminalDisplay->setReadOnly(true);
    ui->terminalDisplay->setUndoRedoEnabled(false);
    ui->terminalDisplay->setMaximumBlockCount(1000);
    
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::onDisconnect);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendData);
    connect(ui->inputLine, &QLineEdit::returnPressed, this, &MainWindow::onSendData);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::onReadData);
}

MainWindow::~MainWindow()
{
    if (serial->isOpen())
        serial->close();
    delete ui;
}

bool MainWindow::connectSerial(const QString &port, int baudRate)
{
    serial->setPortName(port);
    serial->setBaudRate(baudRate);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    
    if (serial->open(QIODevice::ReadWrite)) {
        ui->statusbar->showMessage("Connected to " + port + " at " + QString::number(baudRate));
        return true;
    }
    return false;
}

void MainWindow::onDisconnect()
{
    serial->close();
    close();
}

void MainWindow::onReadData()
{
    QByteArray data = serial->readAll();
    ui->terminalDisplay->moveCursor(QTextCursor::End);
    ui->terminalDisplay->insertPlainText(QString::fromLatin1(data));
}

void MainWindow::onSendData()
{
    QString text = ui->inputLine->text();
    if (!text.isEmpty() && serial->isOpen()) {
        serial->write(text.toUtf8() + "\r");
        ui->inputLine->clear();
    }
}
