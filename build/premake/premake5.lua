-- premake5.lua

include( "files.lua" )
require( "qt/qt" )
local qt = premake.extensions.qt

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
		links { "wxregexud.lib" }
	filter "configurations:Release"
		links { "wxbase31u.lib" } -- necessary for WXWIDGET
		links { "wxmsw31u_core.lib" } -- necessary for WXWIDGET
		links { "wxpng.lib" } -- necessary for WXWIDGET
		links { "wxzlib.lib" } -- necessary for WXWIDGET
		links { "wxregexu.lib" }
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

function AddImageLibraries(libpath)
	includedirs
	{
		libpath .. "stb/",
	}
end

function AddTinyObjLibrary(libpath)
	includedirs
	{
		libpath .. "tinyobjloader/",
	}
end

function AddQtLibrary(qtPathX64, qtPathX86, usedModules, genMocsPath)
--QT

	filter {}
	qt.enable()
	qtmodules { usedModules }
	qtprefix "Qt5"
	qtgenerateddir (genMocsPath)
	configuration { "Debug" }
		qtsuffix "d"
	configuration { }
	filter "platforms:x64"
		qtpath ( qtPathX64)

	filter "platforms:x86"
		qtpath ( qtPathX86)
	filter {}
end

function AddWindowedProject(name, projectDir, argFiles, argIncludes, argLinks, argDefines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine, useImageLibs, useTinyObjLoader)
	project (name)
		kind "WindowedApp"
		entrypoint "WinMainCRTStartup"
		
		language "C++"
		--cppdialect "C++17"
		
		files (argFiles) 
		
		filter "system:Windows"		
		
		filter {}
		
		includedirs	{ argIncludes }

		links { argLinks }
		
		defines { argDefines }
		if (usewxWidget == true)
		then
			AddWXWidgetStaticLibrary(libpath)
		end

		if(useBoost == true)
		then
			AddBoostLibrary(libpath)
		end

		if(useVulkan == true)
		then
			AddVulkanLibrary(libpath)
		end
		if(useQt == true)
		then
			AddQtLibrary(Invision.qt.pathX64, Invision.qt.pathX86,  { "core", "gui", "widgets", "opengl" }, (projectDir..name.."/genqt/"))
		end

		if(useImageLibs == true)
		then
			AddImageLibraries(libpath)
		end
		
		if(useTinyObjLoader == true)
		then
			AddTinyObjLibrary(libpath)
		end

		filter "configurations:Debug"
			defines { "DEBUG" }
			symbols "On"
			
		filter "configurations:Release"
			defines { "NDEBUG" }
			optimize "On"

			
		filter "platforms:x86"
			architecture "x86"
			targetdir (rootdir.."/bin/"..name.."/win32/%{cfg.buildcfg}")
			if(useInvisionEgine == true)
			then	
				postbuildcommands
				{
					{"{COPY} "..rootdir.."/../bin/Invision/win32/%{cfg.buildcfg}/Invision.dll".." "..rootdir.."/../bin/"..name.."/win32/%{cfg.buildcfg}"}
				}
			end
		filter "platforms:x64"
			architecture "x86_64"
			targetdir (rootdir.."/bin/"..name.."/x64/%{cfg.buildcfg}")
			if(useInvisionEgine == true)
			then
				postbuildcommands
				{
					{"{COPY} "..rootdir.."/../bin/Invision/x64/%{cfg.buildcfg}/Invision.dll".." "..rootdir.."/../bin/"..name.."/x64/%{cfg.buildcfg}"}
				}
			end
end

function AddConsoleProject(name, projectDir, argFiles, argIncludes, argLinks, argDefines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine, useImageLibs, useTinyObjLoader)
	project (name)
		kind "ConsoleApp"		
		language "C++"
		--cppdialect "C++17"
		
		files (argFiles) 
		
		filter "system:Windows"		
		
		filter {}
		
		includedirs	{ argIncludes }

		links { argLinks }
		
		defines { argDefines }
		if (usewxWidget == true)
		then
			AddWXWidgetStaticLibrary(libpath)
		end

		if(useBoost == true)
		then
			AddBoostLibrary(libpath)
		end

		if(useVulkan == true)
		then
			AddVulkanLibrary(libpath)
		end
		if(useQt == true)
		then
			AddQtLibrary(Invision.qt.pathX64, Invision.qt.pathX86,  { "core", "gui", "widgets", "opengl" }, (srcroot .. "tools/"..name.."/genqt/"))
		end

		if(useImageLibs == true)
		then
			AddImageLibraries(libpath)
		end

		if(useTinyObjLoader == true)
		then
			AddTinyObjLibrary(libpath)
		end

		filter "configurations:Debug"
			defines { "DEBUG" }
			symbols "On"
			
		filter "configurations:Release"
			defines { "NDEBUG" }
			optimize "On"

			
		filter "platforms:x86"
			architecture "x86"
			targetdir (rootdir.."/bin/"..name.."/win32/%{cfg.buildcfg}")
			if(useInvisionEgine == true)
			then	
				postbuildcommands
				{
					{"{COPY} "..rootdir.."/../bin/Invision/win32/%{cfg.buildcfg}/Invision.dll".." "..rootdir.."/../bin/"..name.."/win32/%{cfg.buildcfg}"}
				}
			end
		filter "platforms:x64"
			architecture "x86_64"
			targetdir (rootdir.."/bin/"..name.."/x64/%{cfg.buildcfg}")
			if(useInvisionEgine == true)
			then
				postbuildcommands
				{
					{"{COPY} "..rootdir.."/../bin/Invision/x64/%{cfg.buildcfg}/Invision.dll".." "..rootdir.."/../bin/"..name.."/x64/%{cfg.buildcfg}"}
				}
			end
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
		targetdir (rootdir.."/bin/Invision/win32/%{cfg.buildcfg}")		
	filter "platforms:x64"
		architecture "x86_64"
		targetdir (rootdir.."/bin/Invision/x64/%{cfg.buildcfg}")

--function AddConsoleProject(name, projectDir, argFiles, argIncludes, argLinks, argDefines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)
AddConsoleProject(
	"SandboxConsole", srcroot .. "tools/", {
	srcroot .. "tools/sandboxConsole/**.h",
	srcroot .. "tools/sandboxConsole/**.cpp"
	}, srcroot, { "Invision" }, {}, true, false, false, false, true, false, false)

--function AddConsoleProject(name, projectDir, argFiles, argIncludes, argLinks, argDefines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)	
AddConsoleProject(
	"UnitTests", srcroot .. "tests/", {
		srcroot .. "tests/**.h",
		srcroot .. "tests/**.cpp"
	}, srcroot, { "Invision" }, {}, true, false, false, false, true, false, false)

-----------------------------------------------------------------------------------------------------
--Examples
-----------------------------------------------------------------------------------------------------

--function AddWindowedProject(name, files, includes, links, defines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)
AddConsoleProject(
	"QTDemoApp", srcroot .. "tools/", {
		srcroot .. "tools/QTDemoApp/**.h",
		srcroot .. "tools/QTDemoApp/**.cpp"
	}, srcroot, { "Invision" }, {"INVISION_BASE_DIR=" .. invision_root }, true, false, true, true, true, false, false)

--function AddWindowedProject(name, files, includes, links, defines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)
AddConsoleProject(
	"TextureDemo", srcroot .. "tools/", {
		srcroot .. "tools/TextureDemo/**.h",
		srcroot .. "tools/TextureDemo/**.cpp"
	}, srcroot, { "Invision" }, {"INVISION_BASE_DIR=" .. invision_root }, true, false, true, true, true, true, false)

--function AddWindowedProject(name, files, includes, links, defines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)
AddConsoleProject(
	"DepthTextureDemo", srcroot .. "tools/", {
		srcroot .. "tools/DepthTextureDemo/**.h",
		srcroot .. "tools/DepthTextureDemo/**.cpp"
	}, srcroot, { "Invision" }, {"INVISION_BASE_DIR=" .. invision_root }, true, false, true, true, true, true, false)
	
--function AddWindowedProject(name, files, includes, links, defines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)
AddConsoleProject(
	"LoadModelDemo", srcroot .. "tools/", {
		srcroot .. "tools/LoadModelDemo/**.h",
		srcroot .. "tools/LoadModelDemo/**.cpp"
	}, srcroot, { "Invision" }, {"INVISION_BASE_DIR=" .. invision_root }, true, false, true, true, true, true, true)

-----------------------------------------------------------------------------------------------------
-- University Examples
-----------------------------------------------------------------------------------------------------

--function AddWindowedProject(name, files, includes, links, defines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)
AddConsoleProject(
	"practical1", srcroot .. "tools/", {
		srcroot .. "tools/practical1/**.h",
		srcroot .. "tools/practical1/**.cpp"
	}, srcroot, { "Invision" }, {"INVISION_BASE_DIR=" .. invision_root }, true, false, true, true, true, false, false)
