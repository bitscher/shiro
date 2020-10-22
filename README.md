# Shiro

Shiro is a free open source Nintendo Game Boy emulator written as a hobby
project.
It is written in C++11 and relies on SDL for display and keyboard interfaces
but since it is wrapped in just two files ([controls.cpp](controls.cpp) and
[display.cpp](display.cpp)), it is really easy to port.
 
 It currently only supports the original DMG/Pocket Game Boy.

## Dependencies
* SDL2: https://www.libsdl.org/

## Build
On POSIX systems with GNU make available, simply:
```bash
make
```
It will also compile on Microsoft Visual C++ assuming you setup your
environment properly.

## Usage
```bash
shiro [-b] path_to_rom
```
`-b` will attempt to load the Game Boy BIOS from file ``bios.bin`` and play it
before running the rom. The file is not included since it is copyrighted.
