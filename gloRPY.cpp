#include <iostream>
#include <windows.h>
#include <conio.h>
#include <iomanip>
#include <sstream>

using namespace std;

class SerialTerminal {
private:
    HANDLE hSerial;
    bool showHex;

public:
    SerialTerminal() : hSerial(INVALID_HANDLE_VALUE), showHex(false) {}
    
    ~SerialTerminal() {
        if (hSerial != INVALID_HANDLE_VALUE) {
            CloseHandle(hSerial);
        }
    }

    bool connect(const char* portName, int baudRate = 9600) {
        hSerial = CreateFileA(portName,
            GENERIC_READ | GENERIC_WRITE,
            0,
            0,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            0);

        if (hSerial == INVALID_HANDLE_VALUE) {
            cerr << "Error opening port " << portName << "!\n";
            return false;
        }

        DCB dcbSerialParams = { 0 };
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

        if (!GetCommState(hSerial, &dcbSerialParams)) {
            cerr << "Error getting port state!\n";
            return false;
        }

        dcbSerialParams.BaudRate = baudRate;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;
        dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

        if (!SetCommState(hSerial, &dcbSerialParams)) {
            cerr << "Error setting port state!\n";
            return false;
        }

        // Set timeouts
        COMMTIMEOUTS timeouts = { 0 };
        timeouts.ReadIntervalTimeout = 50;
        timeouts.ReadTotalTimeoutConstant = 50;
        timeouts.ReadTotalTimeoutMultiplier = 10;
        timeouts.WriteTotalTimeoutConstant = 50;
        timeouts.WriteTotalTimeoutMultiplier = 10;
        SetCommTimeouts(hSerial, &timeouts);

        cout << "Connected to " << portName << " at " << baudRate << " baud.\n";
        return true;
    }

    void printHelp() {
        cout << "\n=== Serial Terminal Commands ===\n";
        cout << "Ctrl+H - Toggle HEX/ASCII display\n";
        cout << "Ctrl+X - Exit\n";
        cout << "Type normally to send data\n";
        cout << "================================\n\n";
    }

    void printData(const char* data, DWORD length) {
        for (DWORD i = 0; i < length; i++) {
            unsigned char byte = data[i];
            
            if (showHex) {
                // Always show as hex
                cout << hex << setw(2) << setfill('0') 
                         << (int)byte << " ";
            } else {
                // Show printable as ASCII, others as hex
                if (byte >= 32 && byte <= 126 && byte != '\\') {
                    cout << byte;
                } else {
                    switch (byte) {
                        case '\r': cout << "\\r"; break;
                        case '\n': cout << "\\n"; break;
                        case '\t': cout << "\\t"; break;
                        default: 
                            cout << "\\x" << hex << setw(2) 
                                     << setfill('0') << (int)byte;
                            break;
                    }
                }
            }
        }
        cout.flush();
    }

    void run() {
        cout << "Serial Terminal Started\n";
        printHelp();

        char outBuf[256];
        char inBuf[256];
        DWORD bytesWritten, bytesRead;

        while (true) {
            // Check for keyboard input
            if (_kbhit()) {
                int ch = _getch();
                
                // Handle control keys
                if (ch == 3) { // Ctrl+C
                    cout << "\nExiting...\n";
                    break;
                } else if (ch == 24) { // Ctrl+X
                    cout << "\nExiting...\n";
                    break;
                } else if (ch == 8) { // Ctrl+H
                    showHex = !showHex;
                    cout << "\nDisplay mode: " << (showHex ? "HEX" : "ASCII") << "\n";
                } else if (ch == 13) { // Enter key
                    WriteFile(hSerial, "\r\n", 2, &bytesWritten, NULL);
                    cout << "\r\n";
                } else if (ch == 27) { // ESC - clear screen
                    system("cls");
                    printHelp();
                } else if (ch >= 32 && ch <= 126) { // Printable characters
                    WriteFile(hSerial, (char*)&ch, 1, &bytesWritten, NULL);
                    cout << (char)ch;
                }
            }

            // Read from serial port
            if (ReadFile(hSerial, inBuf, sizeof(inBuf) - 1, &bytesRead, NULL)) {
                if (bytesRead > 0) {
                    printData(inBuf, bytesRead);
                    cout << "\n";
                }
            }

            Sleep(10);
        }
    }
};

int main() {
    SerialTerminal terminal;
    
    if (terminal.connect("COM4", 9600)) {
        terminal.run();
    } else {
        cout << "Failed to connect. Press any key to exit.\n";
        _getch();
    }
    
    return 0;
}