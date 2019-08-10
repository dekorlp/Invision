#include "precompiled.h"

#include "VulkanPresentation.h"

CVulkanPresentation::CVulkanPresentation() : m_swapchain(VK_NULL_HANDLE), m_swapchainImageViews(VK_NULL_HANDLE), m_surface(VK_NULL_HANDLE), m_swapchainImages(VK_NULL_HANDLE)
{
	
}

void CVulkanPresentation::CreateSwapChain(VkPhysicalDevice physicalDevice, SVulkanLogicalDevice logicalDevice, VkSurfaceKHR surface,  SVulkanSwapchain* swapChain, const int width, const int height)
{
	m_surface = surface;
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);
	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, width, height);
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}
	VkSwapchainCreateInfoKHR createInfo = CreateSwapchainCreateInfo(physicalDevice, swapChainSupport,
		surfaceFormat, imageCount, extent);
	VkSwapchainKHR oldSwapchain = swapChain->m_swapchain;

	VkSwapchainKHR newSwapchain;
	VkResult result = vkCreateSwapchainKHR(logicalDevice.m_logicalDevice, &createInfo, nullptr, &newSwapchain);
	createInfo.oldSwapchain = oldSwapchain;

	if (result != VK_SUCCESS) {
		throw VulkanException(result, "Error attempting to create a swapchain:");
	}
	swapChain->m_swapchain = newSwapchain;
	m_swapchain = newSwapchain;

	result = vkGetSwapchainImagesKHR(logicalDevice.m_logicalDevice, swapChain->m_swapchain, &imageCount, nullptr);
	if (result != VK_SUCCESS) {
		throw VulkanException(result, "Error attempting to retrieve the count of swapchain images:");
	}
	m_swapchainImages.resize(imageCount);
	result = vkGetSwapchainImagesKHR(logicalDevice.m_logicalDevice, swapChain->m_swapchain, &imageCount, m_swapchainImages.data());
	if (result != VK_SUCCESS) {
		throw VulkanException(result, "Error attempting to retrieve the swapchain images:");
	}
	m_swapchainImageFormat = surfaceFormat.format;
	m_swapchainExtent = extent;
	swapChain->m_swapchainImages = m_swapchainImages;
	swapChain->m_swapchainImageViews = m_swapchainImageViews;
	swapChain->m_swapchainImageFormat = m_swapchainImageFormat;
	swapChain->m_swapchainExtent = extent;
}

void CVulkanPresentation::CreateImageViews(SVulkanLogicalDevice logicalDevice, SVulkanSwapchain* swapChain)
{
	m_swapchainImageViews.resize(m_swapchainImages.size());
	for (uint32_t i = 0; i < m_swapchainImages.size(); i++) {
		VkImageViewCreateInfo createInfo = CreateImageViewCreateInfo(i);

		VkResult result = vkCreateImageView(logicalDevice.m_logicalDevice, &createInfo, nullptr, &m_swapchainImageViews[i]);
		if (result != VK_SUCCESS) {
			throw VulkanException(result, "Unable to create an image view for a swap chain image");
		}
	}
	swapChain->m_swapchainImageViews = m_swapchainImageViews;
}

SwapChainSupportDetails CVulkanPresentation::QuerySwapChainSupport(const VkPhysicalDevice& device) const
{
	SwapChainSupportDetails details;

	VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);
	if (result != VK_SUCCESS) {
		throw VulkanException(result, "Unable to retrieve physical device surface capabilities:");
	}
	uint32_t formatCount = 0;
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);
	if (result != VK_SUCCESS) {
		throw VulkanException(result, "Unable to retrieve the number of formats for a surface on a physical device:");
	}
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
		if (result != VK_SUCCESS) {
			throw VulkanException(result, "Unable to retrieve the formats for a surface on a physical device:");
		}
	}

	uint32_t presentModeCount = 0;
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);
	if (result != VK_SUCCESS) {
		throw VulkanException(result, "Unable to retrieve the count of present modes for a surface on a physical device:");
	}
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
		if (result != VK_SUCCESS) {
			throw VulkanException(result, "Unable to retrieve the present modes for a surface on a physical device:");
		}
	}
	return details;
}

VkSurfaceFormatKHR CVulkanPresentation::ChooseSwapSurfaceFormat(
	const std::vector<VkSurfaceFormatKHR>& availableFormats) const noexcept
{
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}
	return availableFormats[0];
}

VkImageViewCreateInfo CVulkanPresentation::CreateImageViewCreateInfo(uint32_t swapchainImage) const noexcept
{
	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = m_swapchainImages[swapchainImage];
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = m_swapchainImageFormat;
	createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;
	return createInfo;
}

VkExtent2D CVulkanPresentation::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,
	const int width, const int height) const noexcept
{



	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		actualExtent.width = std::max(capabilities.minImageExtent.width,
			std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height,
			std::min(capabilities.maxImageExtent.height, actualExtent.height));
		return actualExtent;
	}
}

VkSwapchainCreateInfoKHR CVulkanPresentation::CreateSwapchainCreateInfo(
	const VkPhysicalDevice physicalDevice,
	const SwapChainSupportDetails& swapChainSupport,
	const VkSurfaceFormatKHR& surfaceFormat,
	uint32_t imageCount, const VkExtent2D& extent)
{
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = m_surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	SQueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = { static_cast<uint32_t>(indices.graphicsFamily),
		static_cast<uint32_t>(indices.presentFamily) };
	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	return createInfo;
}

VkPresentModeKHR CVulkanPresentation::ChooseSwapPresentMode(
	const std::vector<VkPresentModeKHR>& availablePresentModes) const noexcept
{
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

SQueueFamilyIndices CVulkanPresentation::FindQueueFamilies(const VkPhysicalDevice& device) const
{
	SQueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}
		VkBool32 presentSupport = false;
		VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);
		if (result != VK_SUCCESS) {
			throw VulkanException(result, "Error while attempting to check if a surface supports presentation:");
		}
		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}
		if (indices.IsComplete()) {
			break;
		}
		++i;
	}
	return indices;
}

void CVulkanPresentation::CleanupPresentation(SVulkanLogicalDevice logicalDevice)
{
	if (m_swapchain != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(logicalDevice.m_logicalDevice, m_swapchain, nullptr);
	}
	for (auto& imageView : m_swapchainImageViews) {
		vkDestroyImageView(logicalDevice.m_logicalDevice, imageView, nullptr);
	}
}