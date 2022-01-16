#ifndef VULKAN_BASE_PRESENTATION_H
#define VULKAN_BASE_PRESENTATION_H

#include "vulkan\vulkan.h"

namespace Invision
{
	INVISION_API void CreateSurface(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, HWND hwnd);
	INVISION_API void DestroySurface(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext);
	INVISION_API void CreatePresentationSystem(SVulkanBase& vulkanInstance, SVulkanContext& vulkanContext, unsigned int width, unsigned int height);
	INVISION_API void DestroyPresentationSystem(SVulkanContext &vulkanContext);

	class INVISION_API VulkanBasePresentation
	{

		public:
			void CreatePresentation(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, unsigned int width, unsigned int height);
			void CreateSwapChain(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, unsigned int width, unsigned int height);
		
			void CreateImageViews(SVulkanContext &vulkanContext);

			bool IsDeviceSurfaceSuitable(SVulkanBasePhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR surface);

		private:
			VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, unsigned int width, unsigned int height);			
	};
}

#endif