# Invision Engine

## Deployment (Windows)

In order to deplay the Invision Engine follow these steps below:
1. Make sure that Windows Universal CRT SDK is installed by using Windows SDK Version 8.1 (Configuration Properties/General/Windows SDK Version)
1. [Install Qt](https://www.qt.io)
1. Open a terminal and execute /Invision/build/solutions/update-solutions.bat
1. Run Bat Invision/lib/win32/loadLibraries.bat
1. Create File Invision/build/premake/files.lua
1. add following Content
```lua
-- Configuration 

--QT
Invision = {
    qt = {
        pathX64 = "<64 bit QT>",
        pathX86 = "<32 bit QT>"
    }
}

```

To compile tests, (boost unit test libs are needed):
1. Go in libs/win32/boost
1. start: bootstrap.bat
1. choose correct Option for your Visual Studio Version
    1. start: b2.exe toolset=msvc-14.0 => for Microsoft Visual Studio 2015
    1. start: b2.exe toolset=msvc-14.1 => for Microsoft Visual Studio 2017
    1. start: b2.exe toolset=msvc-14.2 => for Microsoft Visual Studio 2019


To compile WxWidget GUI (deprecated, will be removed, soon):
1. Open /lib/wxWidgets/build/msw/wx_vc12.sln
1. Compile solution for both release and debug


