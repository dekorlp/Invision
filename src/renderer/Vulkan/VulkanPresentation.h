#ifndef VULKAN_PRESENTATION_H
#define VULKAN_PRESENTATION_H

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#else
#error The code in VulkanCanvas::CreateWindowSurface only supports Win32. Changes are \
required to support other windowing systems.
#endif

#include "Vulkan.h"
#include "VulkanException.h"

namespace Invision
{
	INVISION_API void CreateSurface(SVulkan &vulkanInstance, HWND hwnd);
	INVISION_API void DestroySurface(SVulkan &vulkanInstance);
}

#endif