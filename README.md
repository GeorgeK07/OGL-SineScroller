# OGL-SineScroller
A sine scroller effect made in OpenGL with C++. Made for 32x32 ASCII ordered bitmap font as seen here. (https://github.com/ianhan/BitmapFonts/raw/main/fonts/32X32-FL.png)

https://github.com/user-attachments/assets/6ba69bae-6643-4274-bf3a-74645ae5331e

## Build
This program was built and tested with:  
* Windows 10 22H2
  * Visual Studio Community 2022 17.14.23

It will most likely work with newer or older versions, however, there may be some breaking changes that cause it not to build or run. Requires code edits and a makefile to build on other operating systems or with GCC/Clang compilers. (Coming soon)

### Steps
1. Requires vcpkg to be installed for Visual Studio MSBuild. (https://learn.microsoft.com/en-us/vcpkg/get_started/get-started-msbuild)

2. Use this command to clone the repository with all submodules.
```git clone --recurse-submodules https://github.com/GeorgeK07/OGL-SineScroller.git```

3. Click the build button (green play button) in Visual Studio and it will download dependencies and build the program.

## License
This program is licensed under the GNU GPL-3.0-or-later

## Credits
Thanks to Learn OpenGL for their tutorials (https://learnopengl.com/)

### imgui
The MIT License (MIT)

Copyright (c) 2014-2026 Omar Cornut

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

