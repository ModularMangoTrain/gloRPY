#include "serialterminal.h"
#include <cstring>

SerialTerminal::SerialTerminal() : hSerial(INVALID_HANDLE_VALUE) {}

SerialTerminal::~SerialTerminal() {
    disconnect();
}

bool SerialTerminal::connect(const char* portName, int baudRate) {
    hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, 
                          OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
    if (hSerial == INVALID_HANDLE_VALUE) return false;

    DCB dcb;
    memset(&dcb, 0, sizeof(dcb));
    dcb.DCBlength = sizeof(dcb);
    if (!GetCommState(hSerial, &dcb)) return false;
    
    dcb.BaudRate = baudRate;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;
    dcb.fBinary = TRUE;
    dcb.fParity = FALSE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fDsrSensitivity = FALSE;
    dcb.fTXContinueOnXoff = FALSE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fErrorChar = FALSE;
    dcb.fNull = FALSE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    dcb.fAbortOnError = FALSE;
    if (!SetCommState(hSerial, &dcb)) return false;

    COMMTIMEOUTS timeouts;
    memset(&timeouts, 0, sizeof(timeouts));
    timeouts.ReadIntervalTimeout = 1;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    SetCommTimeouts(hSerial, &timeouts);
    
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
    return true;
}

void SerialTerminal::disconnect() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
}

bool SerialTerminal::isConnected() const {
    return hSerial != INVALID_HANDLE_VALUE;
}

int SerialTerminal::readData(char* buffer, int maxSize) {
    OVERLAPPED ov = {0};
    ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    DWORD bytesRead = 0;
    
    if (ReadFile(hSerial, buffer, maxSize, &bytesRead, &ov)) {
        CloseHandle(ov.hEvent);
        return bytesRead;
    }
    
    if (GetLastError() == ERROR_IO_PENDING) {
        if (WaitForSingleObject(ov.hEvent, 50) == WAIT_OBJECT_0) {
            GetOverlappedResult(hSerial, &ov, &bytesRead, FALSE);
        }
    }
    CloseHandle(ov.hEvent);
    return bytesRead;
}

bool SerialTerminal::writeData(const char* data, int size) {
    OVERLAPPED ov = {0};
    ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    DWORD bytesWritten = 0;
    
    if (!WriteFile(hSerial, data, size, &bytesWritten, &ov)) {
        if (GetLastError() == ERROR_IO_PENDING) {
            WaitForSingleObject(ov.hEvent, INFINITE);
            GetOverlappedResult(hSerial, &ov, &bytesWritten, FALSE);
        }
    }
    CloseHandle(ov.hEvent);
    return bytesWritten == (DWORD)size;
}
