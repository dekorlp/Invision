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

function AddVulkanLibrary(libpath)
	includedirs{
		libpath .. "vulkan/include/"
	}
	
	filter "platforms:x86"
		libdirs
		{
			libpath .. "vulkan/Lib32/" -- x32 bit variant
		}
		links { "vulkan-1.lib" }
		
	filter "platforms:x64"
		libdirs
		{
			libpath .. "vulkan/Lib/" -- x64 bit variant
		}
		links { "vulkan-1.lib" }
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
invision_root = "\"" .. path.getabsolute(rootdir) .. "\""

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
	characterset ("MBCS")
		--cppdialect "C++17"
	
	pchheader "precompiled.h"
	pchsource (srcroot .. "common/precompiled.cpp")
	
	files {
		srcroot .. "common/**.h",
		srcroot .. "common/**.cpp",
		srcroot .. "lowlevel/**.h",
		srcroot .. "lowlevel/**.cpp",
		srcroot .. "math/**.h",
		srcroot .. "math/**.cpp",
		srcroot .. "renderer/**.h",
		srcroot .. "renderer/**.cpp",
		srcroot .. "InCommon.h",
		srcroot .. "InMath.h",
		srcroot .. "Invision.h"
	}	
	
	includedirs {
		srcroot,
		srcroot .. "common/"
	}
	
	filter "system:Windows"	
		defines 
		{
			"INVISION_EXPORTS",
			"_WINDLL"
		}
	filter {}
	
	AddBoostLibrary(libpath)
	AddVulkanLibrary(libpath)
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		
	filter "platforms:x86"
		architecture "x86"
		
	filter "platforms:x64"
		architecture "x86_64"
	
project "SandboxWindow"
	kind "WindowedApp"
	entrypoint "WinMainCRTStartup"
	language "C++"
	targetdir (rootdir.."/bin/%{cfg.buildcfg}")
	--cppdialect "C++17"
	
	files
	{
		srcroot .. "tools/sandboxWindow/**.h",
		srcroot .. "tools/sandboxWindow/**.cpp"
	}
	
	filter "system:Windows"		
		
	filter {}
	
	includedirs
	{
		-- Invision libraries
		srcroot
	}
	
	links { "Invision" }
	
	defines {"INVISION_BASE_DIR=" .. invision_root }
	
	AddWXWidgetStaticLibrary(libpath)
	AddBoostLibrary(libpath)
	AddVulkanLibrary(libpath)
		
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

		
	filter "platforms:x86"
		architecture "x86"	
		
	filter "platforms:x64"
		architecture "x86_64"
	
project "SandboxConsole"
	kind "ConsoleApp"
	language "C++"
	targetdir (rootdir.."/bin/%{cfg.buildcfg}")
	--cppdialect "C++17"
	
	files
	{
		srcroot .. "tools/sandboxConsole/**.h",
		srcroot .. "tools/sandboxConsole/**.cpp"
	}
	
	filter "system:Windows"		
		
	filter {}
	
	includedirs
	{
		-- Invision libraries
		srcroot
	}
	
	links { "Invision" }
	
	AddBoostLibrary(libpath)
		
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
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
		srcroot
	}
		
	links { "Invision" }
	
	AddBoostLibrary(libpath)
		
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		
	filter "platforms:x86"
		architecture "x86"
		
	filter "platforms:x64"
		architecture "x86_64"
		
-----------------------------------------------------------------------------------------------------
--Examples
-----------------------------------------------------------------------------------------------------		
project "Triangle"
	kind "WindowedApp"
	entrypoint "WinMainCRTStartup"
	language "C++"
	targetdir (rootdir.."/bin/%{cfg.buildcfg}")
	--cppdialect "C++17"
	
	files
	{
		srcroot .. "tools/examples/triangle/**.h",
		srcroot .. "tools/examples/triangle/**.cpp"
	}
	
	filter "system:Windows"		
		
	filter {}
	
	includedirs
	{
		-- Invision libraries
		srcroot
	}
	
	links { "Invision" }
	
	defines {"INVISION_BASE_DIR=" .. invision_root }
	
	AddWXWidgetStaticLibrary(libpath)
	AddBoostLibrary(libpath)
	AddVulkanLibrary(libpath)
		
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

		
	filter "platforms:x86"
		architecture "x86"	
		
	filter "platforms:x64"
		architecture "x86_64"

project "GraphicsInstance"
	kind "WindowedApp"
	entrypoint "WinMainCRTStartup"
	language "C++"
	targetdir (rootdir.."/bin/%{cfg.buildcfg}")
	--cppdialect "C++17"
	
	files
	{
		srcroot .. "tools/examples/GraphicsInstance/**.h",
		srcroot .. "tools/examples/GraphicsInstance/**.cpp"
	}
	
	filter "system:Windows"		
		
	filter {}
	
	includedirs
	{
		-- Invision libraries
		srcroot
	}
	
	links { "Invision" }
	
	defines {"INVISION_BASE_DIR=" .. invision_root }
	
	AddWXWidgetStaticLibrary(libpath)
	AddBoostLibrary(libpath)
	AddVulkanLibrary(libpath)
		
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

		
	filter "platforms:x86"
		architecture "x86"	
		
	filter "platforms:x64"
		architecture "x86_64"