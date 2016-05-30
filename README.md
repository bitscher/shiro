Shiro
=======================

Shiro is a free crossplatform Nintendo Gameboy emulator written as a hobby project.
It is written in C++11 and relies on SDL for display and keyboard interface (although the code structure allows it to be easily swapped for some other library).
It currently only support the original DMG Gameboy.

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


Screenshots
-----------------------
![Pokemon Green](/screenshots/pokemongreen.png)
![Dr Mario](/screenshots/drmario.png)
![Tetris](/screenshots/tetris.png)

License (BSD 3-Clause)
-----------------------
<i>Copyright (c) 2016, Romain Bitschené
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyrightnotice, this list of conditions and the following disclaimer in thedocumentation and/or other materials provided with the distribution.
* Neither the name of Romain Bitschené nor the names of its contributors may be used to endorse or promote productsderived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ROMAIN BITSCHENE BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.</i>