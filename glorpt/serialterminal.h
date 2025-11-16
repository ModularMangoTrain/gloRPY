#ifndef SERIALTERMINAL_H
#define SERIALTERMINAL_H

#include <windows.h>

class SerialTerminal {
private:
    HANDLE hSerial;

public:
    SerialTerminal();
    ~SerialTerminal();
    bool connect(const char* portName, int baudRate = 9600);
    void disconnect();
    bool isConnected() const;
    int readData(char* buffer, int maxSize);
    bool writeData(const char* data, int size);
};

#endif
