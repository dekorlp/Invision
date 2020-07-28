#ifndef VULKAN_BASE_PRESENTATION_H
#define VULKAN_BASE_PRESENTATION_H

#include "vulkan\vulkan.h"

namespace Invision
{
	INVISION_API void CreateSurface(SVulkanBase &vulkanInstance, HWND hwnd);
	INVISION_API void DestroySurface(SVulkanBase &vulkanInstance);
	INVISION_API void CreatePresentationSystem(SVulkanBase &vulkanInstance, unsigned int width, unsigned int height);
	INVISION_API void DestroyPresentationSystem(SVulkanBase &vulkanInstance);

	class INVISION_API VulkanBasePresentation
	{

		public:
			void CreatePresentation(SVulkanBase &vulkanInstance, unsigned int width, unsigned int height);
			void CreateSwapChain(SVulkanBase &vulkanInstance, unsigned int width, unsigned int height);
			void CreateImageViews(SVulkanBase &vulkanInstance);

			bool IsDeviceSurfaceSuitable(SVulkanBasePhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR surface);

		private:
			VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, unsigned int width, unsigned int height);			
	};
}

#endif