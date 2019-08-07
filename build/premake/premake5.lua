-- premake5.lua

function AddWXWidgetStaticLibrary(libpath)

	wxwidgetOption = "lib"

	--links { "comctl32.lib" } -- necessary for WXWIDGET
	--links { "rpcrt4.lib" } -- necessary for WXWIDGET
	--defines { "UNICODE" } -- necessary for WXWIDGET
	--defines { "_UNICODE" } -- necessary for WXWIDGET

	includedirs{
		libpath .. "wxWidgets/include/"
	}

	filter "system:Windows"
		links { "comctl32.lib" } -- necessary for WXWIDGET
	    links { "rpcrt4.lib" } -- necessary for WXWIDGET
		defines { "UNICODE" } -- necessary for WXWIDGET
		defines { "_UNICODE" } -- necessary for WXWIDGET
	filter "configurations:Debug"
		links { "wxbase31ud.lib" } -- necessary for WXWIDGET
		links { "wxmsw31ud_core.lib" } -- necessary for WXWIDGET
		links { "wxpngd.lib" } -- necessary for WXWIDGET
		links { "wxzlibd.lib" } -- necessary for WXWIDGET
	filter "configurations:Release"
		links { "wxbase31u.lib" } -- necessary for WXWIDGET
		links { "wxmsw31u_core.lib" } -- necessary for WXWIDGET
		links { "wxpng.lib" } -- necessary for WXWIDGET
		links { "wxzlib.lib" } -- necessary for WXWIDGET
	filter "platforms:x86"
		libdirs
		{
			libpath .. "wxWidgets/lib/vc_".. wxwidgetOption .."/"
		}
		
		includedirs
		{
			libpath .. "wxWidgets/lib/vc_" .. wxwidgetOption .."/mswu/"
		}
		
	
	filter "platforms:x64"
		libdirs
		{
			libpath .. "wxWidgets/lib/vc_x64_".. wxwidgetOption .."/"
		}
		
		includedirs
		{
			libpath .. "wxWidgets/lib/vc_x64_".. wxwidgetOption .."/mswu/"
		}
	
	filter {}
	
end

function AddWXWidgetDynamicLibrary(libpath)

	wxwidgetOption = "dll"

	--links { "comctl32.lib" } -- necessary for WXWIDGET
	--links { "rpcrt4.lib" } -- necessary for WXWIDGET
	--defines { "UNICODE" } -- necessary for WXWIDGET
	--defines { "_UNICODE" } -- necessary for WXWIDGET

	defines { "WXUSINGDLL" } -- necessary for WXWIDGET (DLL)
	
	includedirs{
		libpath .. "wxWidgets/include/"
	}

	filter "system:Windows"
		links { "comctl32.lib" } -- necessary for WXWIDGET
	    links { "rpcrt4.lib" } -- necessary for WXWIDGET
		defines { "UNICODE" } -- necessary for WXWIDGET
		defines { "_UNICODE" } -- necessary for WXWIDGET
	filter "configurations:Debug"
		links { "wxbase31ud.lib" } -- necessary for WXWIDGET
		links { "wxmsw31ud_core.lib" } -- necessary for WXWIDGET
		links { "wxpngd.lib" } -- necessary for WXWIDGET
		links { "wxzlibd.lib" } -- necessary for WXWIDGET
	filter "configurations:Release"
		links { "wxbase31u.lib" } -- necessary for WXWIDGET
		links { "wxmsw31u_core.lib" } -- necessary for WXWIDGET
		links { "wxpng.lib" } -- necessary for WXWIDGET
		links { "wxzlib.lib" } -- necessary for WXWIDGET
	filter "platforms:x86"
		libdirs
		{
			libpath .. "wxWidgets/lib/vc_".. wxwidgetOption .."/"
		}
		
		includedirs
		{
			libpath .. "wxWidgets/lib/vc_" .. wxwidgetOption .."/mswu/"
		}
		
	
	filter "platforms:x64"
		libdirs
		{
			libpath .. "wxWidgets/lib/vc_x64_".. wxwidgetOption .."/"
		}
		
		includedirs
		{
			libpath .. "wxWidgets/lib/vc_x64_".. wxwidgetOption .."/mswu/"
		}
	
	filter {}

end

function AddBoostLibrary(libpath)
	includedirs
	{
		libpath .. "boost/",
	}
	
	libdirs
	{
		libpath .. "boost/stage/lib/"
	}

end

rootdir = "../.."
srcroot = rootdir .. "/src/"
libroot = rootdir .. "/lib/"

workspace "Invision"
	configurations { "Debug" , "Release" }
	platforms { "x86" , "x64" }
	location (rootdir .. "/build/solutions/".._ACTION)
	
	filter "system:Windows"
		libpath = libroot .. "win32/"
	filter { }
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
		defines 
		{
			"CORE_EXPORTS",
			"HAL_EXPORTS",
			"_WINDLL"
		}
	filter {}
	
	AddBoostLibrary(libpath)
	
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
		
		
	filter {}
	
	includedirs
	{
		-- Invision libraries
		srcroot .. "/common/",
		srcroot .. "/lowlevel/",
		srcroot .. "/math/",
	}
	
	links { "Invision" }
	
	AddWXWidgetStaticLibrary(libpath)
	AddBoostLibrary(libpath)
		
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
	
	filter {}
	
	includedirs
	{
		-- Invision libraries
		srcroot .. "/common/",
		srcroot .. "/lowlevel/",
		srcroot .. "/math/",
	}
		
	links { "Invision" }
	
	AddBoostLibrary(libpath)
		
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