#include "VulkanWindow.h"
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

using namespace std;

class VulkanApp : public wxApp
{
	public:
		VulkanApp()
		{
			
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
				return false;
			}

			vkWindow->Show(true);

			return true;
		}
};

IMPLEMENT_APP(VulkanApp);