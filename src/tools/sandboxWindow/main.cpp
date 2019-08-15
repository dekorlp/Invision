﻿#include "VulkanWindow.h"
#include <iostream>
#include <sstream>

#include "wx\wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#ifdef _UNICODE
#ifdef _DEBUG
#pragma comment(lib, "wxbase31ud.lib")
#else
#pragma comment(lib, "wxbase31u.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "wxbase31d.lib")
#else
#pragma comment(lib, "wxbase31.lib")
#endif
#endif

#include <vulkan\vulkan.h>
#include "InCommon.h"
#include "renderer\Vulkan\Vulkan.h"

void debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
	std::stringstream ss;
	ss << "validation layer: " << pCallbackData->pMessage;
	INVISION_LOG_RAWTEXT(ss.str());
}

class VulkanApp : public wxApp
{
	public:
		VulkanApp()
		{
			log.Open("../../../logs/log.txt");
			Invision::Log::SetLogger(&log);
			//throw Invision::VulkanException("Exception wird geworfen");
			
			vulkan.Init();
			vulkan.SetDebugMessanger(debugCallback);
		}
		~VulkanApp()
		{
			vulkan.Destroy();
		}
		bool OnInit()
		{
			VulkanWindow* vkWindow;
			try
			{

				vkWindow = new VulkanWindow(nullptr, wxID_ANY, "VulkanWindow Sample", wxDefaultPosition, { 800,600 });
			}
			catch (std::runtime_error& err) {
				std::stringstream ss;
				ss << "Error encountered trying to create the Vulkan canvas:\n";
				ss << err.what();
				// TODO OutputDebug Replace with own MessageBox
				//wxMessageBox( ss.str(), "Info");
				INVISION_LOG_ERROR(ss.str());
				return false;
			}

			vkWindow->Show(true);

			return true;
		}
private:
	Invision::Vulkan vulkan;
	Invision::Log log;
};

IMPLEMENT_APP(VulkanApp);
