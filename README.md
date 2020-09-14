Shiro
=======================

Shiro is a free crossplatform Nintendo Gameboy emulator written as a hobby project.
It is written in C++11 and relies on SDL for display and keyboard interface (although the code structure allows it to be easily swapped for some other library).
It currently only supports the original DMG Gameboy.

Platforms
-----------------------
* Windows (Visual Studio 2015 Project included)
* Linux (Makefile included)
* Probably any system that can be targeted by a C++11 compiler and for which SDL has been ported

Usage
-----------------------
```
shiro [-b] path_to_rom
```
-b will try to load the Gameboy bios from file "bios.bin" and play it before running the rom. The file is not included since it is copyrighted.
