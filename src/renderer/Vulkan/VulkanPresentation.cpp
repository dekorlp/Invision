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

	void CreatePresentationSystem(SVulkan &vulkanInstance, unsigned int width, unsigned int height)
	{
		VulkanPresentation().CreatePresentation(vulkanInstance, width, height);
	}

	VkSurfaceFormatKHR VulkanPresentation::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VulkanPresentation::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanPresentation::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, unsigned int width, unsigned int height)
	{
		if (capabilities.currentExtent.width != UINT32_MAX) {
			return capabilities.currentExtent;
		}
		else {
			VkExtent2D actualExtent = { width, height };

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	void VulkanPresentation::CreatePresentation(SVulkan &vulkanInstance, unsigned int width, unsigned int height)
	{
		CreateSwapChain(vulkanInstance, width, height);
	}

	void VulkanPresentation::CreateSwapChain(SVulkan &vulkanInstance, unsigned int width, unsigned int height)
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(vulkanInstance.physicalDevice, vulkanInstance.surface);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, width, height);
	}

}