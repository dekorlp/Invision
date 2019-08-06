-- premake5.lua

rootdir = "../.."
srcroot = rootdir .. "/src/"
libroot = rootdir .. "/lib/"
wxwidgetOption = "lib" -- options {dll, lib}

workspace "Invision"
	configurations { "Debug" , "Release" }
	platforms { "x86" , "x64" }
	location (rootdir .. "/build/solutions/".._ACTION)
	
project "Invision"
	kind "SharedLib"
	language "C++"
	targetdir (rootdir.."/bin/%{cfg.buildcfg}")
	--cppdialect "C++17"
	
	pchheader "precompiled.h"
	pchsource (srcroot .. "common/precompiled.cpp")
	
	files {
		srcroot .. "common/**.h",
		srcroot .. "common/**.cpp",
		srcroot .. "lowlevel/**.h",
		srcroot .. "lowlevel/**.cpp",
		srcroot .. "math/**.h",
		srcroot .. "math/**.cpp"
	}	
	
	filter "system:Windows"
		libpath = libroot .. "win32/"
		disablewarnings {}
		
		defines 
		{
			"CORE_EXPORTS",
			"HAL_EXPORTS",
			"_WINDLL"
		}
	filter {}
	
	includedirs
	{
		libpath .. "boost/",
		libpath .. "wxWidgets/include/"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		
	filter "platforms:x86"
		architecture "x86"
		
		libdirs
		{
			libpath .. "wxWidgets/lib/vc_".. wxwidgetOption .."/"
		}
		
		includedirs
		{
			libpath .. "wxWidgets/lib/vc_" .. wxwidgetOption .."/mswu/"
		}
		
	filter "platforms:x64"
		architecture "x86_64"
			
		libdirs
		{
			libpath .. "wxWidgets/lib/vc_x64_".. wxwidgetOption .."/"
		}
		
		includedirs
		{
			libpath .. "wxWidgets/lib/vc_x64_".. wxwidgetOption .."/mswu/"
		}
		
project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	targetdir (rootdir.."/bin/%{cfg.buildcfg}")
	--cppdialect "C++17"
	
	files
	{
		srcroot .. "sandbox/**.h",
		srcroot .. "sandbox/**.cpp"
	}
	
	filter "system:Windows"
		libpath = libroot .. "win32/"
		disablewarnings {}
		
		links { "comctl32.lib" } -- necessary for WXWIDGET
		links { "rpcrt4.lib" } -- necessary for WXWIDGET
		defines { "UNICODE" } -- necessary for WXWIDGET
		defines { "_UNICODE" } -- necessary for WXWIDGET
	
	filter {}
	
	includedirs
	{
		-- Invision libraries
		srcroot .. "/common/",
		srcroot .. "/lowlevel/",
		srcroot .. "/math/",
		
		-- third party libraries
		libpath .. "boost/",
		libpath .. "wxWidgets/include/"
	}
		
	links { "Invision" }
	links { "wxbase31ud.lib" } -- necessary for WXWIDGET
	links { "wxmsw31ud_core.lib" } -- necessary for WXWIDGET
	links { "wxpngd.lib" } -- necessary for WXWIDGET
	links { "wxzlibd.lib" } -- necessary for WXWIDGET
	
		if (wxwidgetOption == "dll") then 
			defines { "WXUSINGDLL" } -- necessary for WXWIDGET (DLL)
			
		end
		
	filter "configurations:Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		
	filter "platforms:x86"
		architecture "x86"
		
		libdirs
		{
			libpath .. "wxWidgets/lib/vc_" .. wxwidgetOption .."/"
		}
		
		includedirs
		{
			libpath .. "wxWidgets/lib/vc_" .. wxwidgetOption .."/mswu/"
		}
		
	filter "platforms:x64"
		architecture "x86_64"
		
		libdirs
		{
			libpath .. "wxWidgets/lib/vc_x64_".. wxwidgetOption .."/"
		}
		
		includedirs
		{
			libpath .. "wxWidgets/lib/vc_x64_" .. wxwidgetOption .."/mswu/"
		}
		
project "UnitTests"
	kind "ConsoleApp"
	language "C++"
	targetdir (rootdir.."/bin/%{cfg.buildcfg}")
	--cppdialect "C++17"
	
	files
	{
		srcroot .. "tests/**.h",
		srcroot .. "tests/**.cpp"
	}
	
	filter "system:Windows"
		libpath = libroot .. "win32/"
		disablewarnings {}
	
	filter {}
	
	includedirs
	{
		-- Invision libraries
		srcroot .. "/common/",
		srcroot .. "/lowlevel/",
		srcroot .. "/math/",
		
		-- third party libraries
		libpath .. "boost/"
	}
		
	libdirs
	{
		libpath .. "boost/stage/lib/"
	}
		
	links { "Invision" }
		
	filter "configurations:Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		
	filter "platforms:x86"
		architecture "x86"
		
	filter "platforms:x64"
		architecture "x86_64"