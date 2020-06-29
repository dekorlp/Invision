#include "VulkanWindow.h"
#include "AdditionalFunctions.h"
#include "renderer\Vulkan\VulkanBaseException.h"
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

#include "InCommon.h"


class VulkanApp : public wxApp
{
	public:
		VulkanApp()
		{
			log.Open(std::string(INVISION_BASE_DIR).append("/logs/log.txt"));
			Invision::Log::SetLogger(&log);
			//throw Invision::VulkanException("Exception wird geworfen");
			
			
		}
		~VulkanApp()
		{
			
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
			catch (Invision::InvisionException& iEx)
			{
				std::stringstream ss;
				ss << "Error encountered trying to create the Vulkan canvas:\n";
				ss << iEx.what();
				// TODO OutputDebug Replace with own MessageBox
				//wxMessageBox( ss.str(), "Info");
				INVISION_LOG_ERROR(ss.str());
				return false;
			}
			catch (Invision::InvisionBaseRendererException& iEx)
			{
				std::stringstream ss;
				ss << "Vulkan Error encountered trying to create the Vulkan canvas:\n";
				ss << iEx.what();
				// TODO OutputDebug Replace with own MessageBox
				//wxMessageBox( ss.str(), "Info");
				INVISION_LOG_ERROR(ss.str());
				return false;
			}

			vkWindow->Show(true);

			return true;
		}
private:
	Invision::Log log;
};

IMPLEMENT_APP(VulkanApp);
