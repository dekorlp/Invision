-- premake5.lua

--
-- include list of modules
--

include( "../config.lua" )
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
			AddQtLibrary(Invision.qt.pathX64, Invision.qt.pathX86, Invision.qt.qtModules, (projectDir..name.."/genqt/"))
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
			AddQtLibrary(Invision.qt.pathX64, Invision.qt.pathX86, Invision.qt.qtModules, (projectDir..name.."/genqt/"))
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

-----------------------------------------------------------------------------------------------------
-- INVISION SOLUTION
-----------------------------------------------------------------------------------------------------

workspace "Invision"
	configurations { "Debug" , "Release" }
	platforms { "x86" , "x64" }
	location (rootdir .. "/build/solutions/".._ACTION)
	
	filter "system:Windows"
		libpath = libroot .. "win32/"
	filter { }
	
-----------------------------------------------------------------------------------------------------
-- Engine PROJECT
-----------------------------------------------------------------------------------------------------
	
project "Invision"
	kind "SharedLib"
	language "C++"
	characterset ("MBCS")
		--cppdialect "C++17"
	
	pchheader "precompiled.h"
	pchsource (srcroot .. "Engine/common/precompiled.cpp")
	
	files {
		srcroot .. "Engine/common/**.h",
		srcroot .. "Engine/common/**.cpp",
		srcroot .. "Engine/lowlevel/**.h",
		srcroot .. "Engine/lowlevel/**.cpp",
		srcroot .. "Engine/math/**.h",
		srcroot .. "Engine/math/**.cpp",
		srcroot .. "Engine/renderer/**.h",
		srcroot .. "Engine/renderer/**.cpp",
		srcroot .. "Engine/input/**.h",
		srcroot .. "Engine/input/**.cpp",
		srcroot .. "Engine/InCommon.h",
		srcroot .. "Engine/InMath.h",
		srcroot .. "Engine/Invision.h"
	}	
	
	includedirs {
		srcroot .. "Engine/",
		srcroot .. "Engine/common/"
	}
	
	filter "system:Windows"	
		defines 
		{
			"INVISION_EXPORTS", "_WINDLL", Invision.inMath.matrixAlignment
		}
		
	filter {}
	
	--AddBoostLibrary(libpath)
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
		
		
-----------------------------------------------------------------------------------------------------
-- DEMO PROJECTS
-----------------------------------------------------------------------------------------------------
		
--function AddConsoleProject(name, projectDir, argFiles, argIncludes, argLinks, argDefines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)
AddConsoleProject(
	"SandboxConsole", srcroot .. "Examples/", {
	srcroot .. "Examples/sandboxConsole/**.h",
	srcroot .. "Examples/sandboxConsole/**.cpp"
	}, {srcroot, srcroot .. "/Engine/"}, { "Invision" }, {}, true, false, false, false, true, false, false)

--function AddConsoleProject(name, projectDir, argFiles, argIncludes, argLinks, argDefines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)	
AddConsoleProject(
	"UnitTests", srcroot .. "Unittests/", {
		srcroot .. "Unittests/**.h",
		srcroot .. "Unittests/**.cpp"
	}, {srcroot, srcroot .. "/Engine/"}, { "Invision" }, {}, true, false, false, false, true, false, false)

-----------------------------------------------------------------------------------------------------
-------- Examples
-----------------------------------------------------------------------------------------------------

--function AddWindowedProject(name, files, includes, links, defines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)
AddConsoleProject(
	"QTDemoApp", srcroot .. "Examples/", {
		srcroot .. "Examples/QTDemoApp/**.h",
		srcroot .. "Examples/QTDemoApp/**.cpp"
	}, {srcroot, srcroot .. "/Engine/"}, { "Invision" }, {"INVISION_BASE_DIR=" .. invision_root }, true, false, true, true, true, false, false)

--function AddWindowedProject(name, files, includes, links, defines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)
AddConsoleProject(
	"TextureDemo", srcroot .. "Examples/", {
		srcroot .. "Examples/TextureDemo/**.h",
		srcroot .. "Examples/TextureDemo/**.cpp"
	}, {srcroot, srcroot .. "/Engine/"}, { "Invision" }, {"INVISION_BASE_DIR=" .. invision_root }, true, false, true, true, true, true, false)

--function AddWindowedProject(name, files, includes, links, defines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)
AddConsoleProject(
	"DepthTextureDemo", srcroot .. "Examples/", {
		srcroot .. "Examples/DepthTextureDemo/**.h",
		srcroot .. "Examples/DepthTextureDemo/**.cpp"
	}, {srcroot, srcroot .. "/Engine/"}, { "Invision" }, {"INVISION_BASE_DIR=" .. invision_root }, true, false, true, true, true, true, false)
	
--function AddWindowedProject(name, files, includes, links, defines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)
AddConsoleProject(
	"LoadModelDemo", srcroot .. "Examples/", {
		srcroot .. "Examples/LoadModelDemo/**.h",
		srcroot .. "Examples/LoadModelDemo/**.cpp"
	}, {srcroot, srcroot .. "/Engine/"}, { "Invision" }, {"INVISION_BASE_DIR=" .. invision_root }, true, false, true, true, true, true, true)
	
AddConsoleProject(
	"DescriptorSets", srcroot .. "Examples/", {
		srcroot .. "Examples/DescriptorSets/**.h",
		srcroot .. "Examples/DescriptorSets/**.cpp"
	}, {srcroot, srcroot .. "/Engine/"}, { "Invision" }, {"INVISION_BASE_DIR=" .. invision_root }, true, false, true, true, true, true, true)

AddConsoleProject(
	"PushConstantDemo", srcroot .. "Examples/", {
		srcroot .. "Examples/PushConstantDemo/**.h",
		srcroot .. "Examples/PushConstantDemo/**.cpp"
	}, {srcroot, srcroot .. "/Engine/"}, { "Invision" }, {"INVISION_BASE_DIR=" .. invision_root }, true, false, true, true, true, true, true)

AddConsoleProject(
	"LoadModelDemoMipMaps", srcroot .. "Examples/", {
		srcroot .. "Examples/LoadModelDemoMipMaps/**.h",
		srcroot .. "Examples/LoadModelDemoMipMaps/**.cpp"
	}, {srcroot, srcroot .. "/Engine/"}, { "Invision" }, {"INVISION_BASE_DIR=" .. invision_root }, true, false, true, true, true, true, true)
	

-----------------------------------------------------------------------------------------------------
-------- University Examples
-----------------------------------------------------------------------------------------------------

--function AddWindowedProject(name, files, includes, links, defines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)
AddConsoleProject(
	"practical1", srcroot .. "Examples/", {
		srcroot .. "Examples/practical1/**.h",
		srcroot .. "Examples/practical1/**.cpp"
	}, {srcroot, srcroot .. "/Engine/"}, { "Invision" }, {"INVISION_BASE_DIR=" .. invision_root }, true, false, true, true, true, false, false)
	
	
	
	
	
	
	
	
-----------------------------------------------------------------------------------------------------
-- Documentation
-----------------------------------------------------------------------------------------------------	
	
		--function AddWindowedProject(name, files, includes, links, defines, useBoost, usewxWidget, useQt, useVulkan, useInvisionEgine)
--AddConsoleProject(
--	"DepthTextureDemo1", srcroot .. "tools/Apps/", {
--		srcroot .. "tools/Apps/DepthTextureDemo1/**.h",
--		srcroot .. "tools/Apps/DepthTextureDemo1/**.cpp"
--	}, srcroot, { "Invision" }, {"INVISION_BASE_DIR=" .. invision_root }, true, false, true, true, true, true, false)
