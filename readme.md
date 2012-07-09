# Rockete - libRocket files editor

## Status
[![Build Status](https://secure.travis-ci.org/FishingCactus/Rockete.png?branch=master)](http://travis-ci.org/FishingCactus/Rockete)

## Description
Rockete is a Qt-based libRocket RML/RCSS files editor.
The main purpose is to provide a useful tool to create game graphical user interfaces.
The second 'e' means "Editor".
It is pronunced like "Machete" ;)

## Requirements

#### Linux
- Qt4 library
- libRocket
- ${LIBROCKET} environment variable if libRocket is not in /usr/include and /usr/lib

#### Windows
- Qt4 library
- libRocket
- ${LIBROCKET} environment variable referring to libRocket root directory

## Build

#### Linux
- qmake
- make

#### Windows (Visual Studio project)
- qmake -tp vc rockete.pro
- Open VS and build project...

## License (MIT)
 
 Copyright (c) 2011-2012 Fishing Cactus SA
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
  
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.