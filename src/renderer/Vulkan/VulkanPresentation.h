#ifndef VULKAN_PRESENTATION_H
#define VULKAN_PRESENTATION_H

#include "vulkan\vulkan.h"

namespace Invision
{
	INVISION_API void CreateSurface(SVulkan &vulkanInstance, HWND hwnd);
	INVISION_API void DestroySurface(SVulkan &vulkanInstance);
	INVISION_API void CreatePresentationSystem(SVulkan &vulkanInstance, unsigned int width, unsigned int height);
	INVISION_API void DestroyPresentationSystem(SVulkan &vulkanInstance);

	class INVISION_API VulkanPresentation
	{

		public:
			void CreatePresentation(SVulkan &vulkanInstance, unsigned int width, unsigned int height);
			void CreateSwapChain(SVulkan &vulkanInstance, unsigned int width, unsigned int height);
			void CreateImageViews(SVulkan &vulkanInstance);

		private:
			VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, unsigned int width, unsigned int height);
	};
}

#endif