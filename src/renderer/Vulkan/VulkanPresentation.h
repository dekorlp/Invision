#ifndef VULKAN_PRESENTATION_H
#define VULKAN_PRESENTATION_H

#define VK_USE_PLATFORM_WIN32_KHR
#include "Vulkan.h"
#include "VulkanException.h"

namespace Invision
{
	INVISION_API void CreateSurface(SVulkan &vulkanInstance, HWND hwnd);
	INVISION_API void DestroySurface(SVulkan &vulkanInstance);
}

#endif