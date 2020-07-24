# Invision Engine

## Deployment

In order to deplay the Invision Engine follow these steps below:
*[Install Qt](https://www.qt.io)
* Open a terminal and execute /Invision/build/solutions/update-solutions.bat
* Run Bat Invision/lib/win32/loadLibraries.bat
* Create File Invision/build/premake/files.lua
* add following Content
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
* Go in libs/win32/boost
* start: bootstrap.bat
* choose correct Option for your Visual Studio Version
    *start: b2.exe toolset=msvc-14.0 => for Microsoft Visual Studio 2015
    *start: b2.exe toolset=msvc-14.1 => for Microsoft Visual Studio 2017
    *start: b2.exe toolset=msvc-14.2 => for Microsoft Visual Studio 2019


To compile WxWidget GUI (deprecated, will be removed, soon):
* Open /lib/wxWidgets/build/msw/wx_vc12.sln
* Compile solution for both release and debug


