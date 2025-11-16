#include "serialworker.h"
#include <QThread>

SerialWorker::SerialWorker(const QString &port, int baudRate)
    : serial(new SerialTerminal()), running(false), portName(port), baud(baudRate)
{
}

SerialWorker::~SerialWorker()
{
    stop();
    delete serial;
}

void SerialWorker::start()
{
    if (!serial->connect(portName.toStdString().c_str(), baud)) {
        emit error("Failed to open port");
        return;
    }
    
    running = true;
    char buffer[1024];
    
    while (running) {
        int bytesRead = serial->readData(buffer, sizeof(buffer));
        if (bytesRead > 0) {
            emit dataReceived(QByteArray(buffer, bytesRead));
        }
        QThread::msleep(10);
    }
}

void SerialWorker::stop()
{
    running = false;
    serial->disconnect();
}

void SerialWorker::writeData(const QByteArray &data)
{
    if (serial && serial->isConnected()) {
        serial->writeData(data.constData(), data.size());
    }
}
