# OGL-SineScroller
A sine scroller effect made in OpenGL with C++. Made for 32x32 ASCII ordered bitmap font as seen here. (https://github.com/ianhan/BitmapFonts/raw/main/fonts/32X32-FL.png)

https://github.com/user-attachments/assets/6ba69bae-6643-4274-bf3a-74645ae5331e

## Build
This program was built and tested with:  
* Windows 10 22H2
  * Visual Studio Community 2022 17.14.23

It will most likely work with newer or older versions, however, there may be some breaking changes that cause it not to build or run. Requires code edits and a makefile to build on other operating systems or with GCC/Clang compilers. (Coming soon)

Steps:
1. Requires vcpkg to be installed for Visual Studio MSBuild. (https://learn.microsoft.com/en-us/vcpkg/get_started/get-started-msbuild)

2. Use this command to clone the repository with all submodules.
```git clone --recurse-submodules https://github.com/GeorgeK07/OGL-SineScroller.git```

3. Click the build button (green play button) in Visual Studio and it will download dependencies and build the program.

## License
This program is licensed under the GNU GPL-3.0-or-later

## Credits
Thanks to Learn OpenGL for their tutorials (https://learnopengl.com/)
