# gloRPY

A lightweight serial terminal application for Windows, similar to PuTTY/TeraTerm but focused on serial communication only.

## Features

- Clean terminal interface with black background and green text
- Configuration dialog for serial port settings
- Support for standard baud rates (9600 - 115200)
- Configurable data bits, parity, stop bits, and flow control
- Real-time data display
- Command input with Enter to send

## Building

### Requirements
- Qt 6 and above
- MinGW or MSVC compiler
- Qt Creator (recommended)

### Compile
```bash
cd glorpt
qmake
mingw32-make
```

Or open `glorpt.pro` in Qt Creator and press Ctrl+R.

## Usage

1. Launch the application
2. Select your COM port and settings in the configuration dialog
3. Click OK to connect
4. Type commands in the input box and press Enter to send
5. Use File → Disconnect to close the connection

## Default Settings

- Baud Rate: 115200
- Data Bits: 8
- Parity: None
- Stop Bits: 1
- Flow Control: None

## License

Free to use and modify.
