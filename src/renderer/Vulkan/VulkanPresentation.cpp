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

	void DestroyPresentationSystem(SVulkan &vulkanInstance)
	{
		vkDestroySwapchainKHR(vulkanInstance.logicalDevice, vulkanInstance.swapChain, nullptr);
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

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = vulkanInstance.surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		SQueueFamilyIndices indices = FindQueueFamilies(vulkanInstance.physicalDevice);
		uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(vulkanInstance.logicalDevice, &createInfo, nullptr, &vulkanInstance.swapChain) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}

		vkGetSwapchainImagesKHR(vulkanInstance.logicalDevice, vulkanInstance.swapChain, &imageCount, nullptr);
		vulkanInstance.swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(vulkanInstance.logicalDevice, vulkanInstance.swapChain, &imageCount, vulkanInstance.swapChainImages.data());

		vulkanInstance.swapChainImageFormat = surfaceFormat.format;
		vulkanInstance.swapChainExtent = extent;
	}

}