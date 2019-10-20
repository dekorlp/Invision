#include "precompiled.h"

#include "VulkanPresentation.h"

namespace Invision
{
	void CreateSurface(SVulkan &vulkanInstance, HWND hwnd)
	{
#ifdef _WIN32
		VkWin32SurfaceCreateInfoKHR sci = {};
		sci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		sci.hwnd = hwnd;
		sci.hinstance = GetModuleHandle(NULL);


		VkResult err = vkCreateWin32SurfaceKHR(vulkanInstance.instance, &sci, nullptr, &vulkanInstance.surface);
		if (err != VK_SUCCESS) {
			throw VulkanException(err, "Cannot create a Win32 Vulkan surface:");
		}
#else
#error The code in VulkanCanvas::CreateWindowSurface only supports Win32. Changes are \
required to support other windowing systems.
#endif
	}

	void DestroySurface(SVulkan &vulkanInstance)
	{
		vkDestroySurfaceKHR(vulkanInstance.instance, vulkanInstance.surface, nullptr);
	}


}