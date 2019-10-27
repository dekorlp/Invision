#ifndef VULKAN_PRESENTATION_H
#define VULKAN_PRESENTATION_H

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#else
#error The code in VulkanCanvas::CreateWindowSurface only supports Win32. Changes are \
required to support other windowing systems.
#endif

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"

namespace Invision
{
	INVISION_API void CreateSurface(SVulkan &vulkanInstance, HWND hwnd);
	INVISION_API void DestroySurface(SVulkan &vulkanInstance);
	INVISION_API void CreatePresentationSystem(SVulkan &vulkanInstance, unsigned int width, unsigned int height);
	

	class INVISION_API VulkanPresentation
	{

		public:
			void CreatePresentation(SVulkan &vulkanInstance, unsigned int width, unsigned int height);
			void CreateSwapChain(SVulkan &vulkanInstance, unsigned int width, unsigned int height);

		private:
			VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, unsigned int width, unsigned int height);
	};
}

#endif