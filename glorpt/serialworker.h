#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include <QObject>
#include <QThread>
#include "serialterminal.h"

class SerialWorker : public QObject
{
    Q_OBJECT

public:
    explicit SerialWorker(const QString &port, int baudRate);
    ~SerialWorker();

public slots:
    void start();
    void stop();
    void writeData(const QByteArray &data);

signals:
    void dataReceived(const QByteArray &data);
    void error(const QString &msg);

private:
    SerialTerminal *serial;
    bool running;
    QString portName;
    int baud;
};

#endif
