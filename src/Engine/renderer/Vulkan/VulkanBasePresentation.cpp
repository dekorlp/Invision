#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"

#include "VulkanBasePresentation.h"

namespace Invision
{
	/*void CreatePresentationSystem(SVulkanBase& vulkanInstance, SVulkanContext& vulkanContext, unsigned int width, unsigned int height)
	{
		VulkanBasePresentation().CreatePresentation(vulkanInstance, vulkanContext, width, height);
	}

	void DestroyPresentationSystem(SVulkanContext &vulkanContext)
	{
		for (auto imageView : vulkanContext.swapChainImageViews) {
			vkDestroyImageView(vulkanContext.logicalDevice, imageView, nullptr);
		}

		vkDestroySwapchainKHR(vulkanContext.logicalDevice, vulkanContext.swapChain, nullptr);
		vulkanContext.swapChainImages.clear();
		vulkanContext.swapChainImageViews.clear();
	}*/

	VkSurfaceFormatKHR VulkanBasePresentation::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VulkanBasePresentation::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanBasePresentation::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, unsigned int width, unsigned int height)
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

	void VulkanBasePresentation::CreatePresentation(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, unsigned int width, unsigned int height)
	{
		CreateSwapChain(vulkanInstance, vulkanContext, width, height);
		CreateImageViews(vulkanContext);
	}

	void VulkanBasePresentation::CreateSwapChain(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, unsigned int width, unsigned int height)
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(vulkanInstance.physicalDeviceStruct.physicalDevice, vulkanContext.surface);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, width, height);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = vulkanContext.surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		//SQueueFamilyIndices indices =   FindQueueFamilies(vulkanInstance.physicalDeviceStruct.physicalDevice, vulkanContext, vulkanContext.surface);
		uint32_t queueFamilyIndices[] = { (uint32_t)vulkanContext.indices.graphicsFamily, (uint32_t)vulkanContext.indices.presentFamily };

		if (vulkanContext.indices.graphicsFamily != vulkanContext.indices.presentFamily) {
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

		if (vkCreateSwapchainKHR(vulkanContext.logicalDevice, &createInfo, nullptr, &vulkanContext.swapChain) != VK_SUCCESS) {
			throw InvisionBaseRendererException("failed to create swap chain!");
		}

		vkGetSwapchainImagesKHR(vulkanContext.logicalDevice, vulkanContext.swapChain, &imageCount, nullptr);
		vulkanContext.swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(vulkanContext.logicalDevice, vulkanContext.swapChain, &imageCount, vulkanContext.swapChainImages.data());

		vulkanContext.swapChainImageFormat = surfaceFormat.format;
		vulkanContext.swapChainExtent = extent;
	}

	void VulkanBasePresentation::CreateImageViews( SVulkanContext &vulkanContext)
	{
		vulkanContext.swapChainImageViews.resize(vulkanContext.swapChainImages.size());

		for (uint32_t i = 0; i < vulkanContext.swapChainImages.size(); i++) {
			vulkanContext.swapChainImageViews[i] = CreateImageView(vulkanContext, vulkanContext.swapChainImages[i], VK_IMAGE_VIEW_TYPE_2D, vulkanContext.swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, 1);
		}

		/*vulkanContext.swapChainImageViews.resize(vulkanContext.swapChainImages.size());
		for (size_t i = 0; i < vulkanContext.swapChainImages.size(); i++) {
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = vulkanContext.swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = vulkanContext.swapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(vulkanInstance.logicalDevice, &createInfo, nullptr, &vulkanContext.swapChainImageViews[i]) != VK_SUCCESS) {
				throw InvisionBaseRendererException("failed to create image views!");
			}
		}*/
	}

	void VulkanBasePresentation::DestroyPresentation(SVulkanContext& vulkanContext)
	{
		for (auto imageView : vulkanContext.swapChainImageViews) {
			vkDestroyImageView(vulkanContext.logicalDevice, imageView, nullptr);
		}

		vkDestroySwapchainKHR(vulkanContext.logicalDevice, vulkanContext.swapChain, nullptr);
		vulkanContext.swapChainImages.clear();
		vulkanContext.swapChainImageViews.clear();
	}
}