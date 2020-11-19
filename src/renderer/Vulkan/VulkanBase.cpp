#include "precompiled.h"

#include "VulkanBaseException.h"

#include "VulkanBase.h"
namespace Invision
{

	SQueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, VkQueueFlags queueFlags)
	{
		SQueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		for (unsigned int i = 0; i < static_cast<uint32_t>(queueFamilies.size()); i++)
		{
			if (queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				if ((queueFamilies[i].queueCount > 0) && (queueFamilies[i].queueFlags & queueFlags) && !indices.GraphicsFamilyIsSet())
				{

					indices.graphicsFamily = i;
				}
			}
			if (queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				if ((queueFamilies[i].queueCount > 0) && (queueFamilies[i].queueFlags & queueFlags) && ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && !indices.ComputeFamilyIsSet())
				{
					indices.computeFamily = i;
				}
			}
			
			if (queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				if ((queueFamilies[i].queueCount > 0) && (queueFamilies[i].queueFlags & queueFlags) && ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0) && !indices.TransferFamilyIsSet())
				{
					indices.transferFamily = i;
				}
			
			}

			if ((indices.GraphicsFamilyIsSet() || !(queueFlags & VK_QUEUE_GRAPHICS_BIT))
				&& (indices.TransferFamilyIsSet() || !(queueFlags & VK_QUEUE_TRANSFER_BIT))
				&& (indices.ComputeFamilyIsSet()) || !(queueFlags & VK_QUEUE_COMPUTE_BIT))
			{
				break;
			}
		}

		if (!indices.GraphicsFamilyIsSet() || !indices.TransferFamilyIsSet() || !indices.TransferFamilyIsSet())
		{
			for (unsigned int i = 0; i < static_cast<uint32_t>(queueFamilies.size()); i++)
			{
				if (queueFamilies[i].queueFlags & queueFlags)
				{
					if (!indices.GraphicsFamilyIsSet() && (queueFlags & VK_QUEUE_GRAPHICS_BIT) && !indices.GraphicsFamilyIsSet())
					{
						indices.graphicsFamily = i;
					}

					if (!indices.TransferFamilyIsSet() && (queueFlags & VK_QUEUE_TRANSFER_BIT) && indices.TransferFamilyIsSet())
					{
						indices.transferFamily = i;
					}

					if (!indices.ComputeFamilyIsSet() && (queueFlags & VK_QUEUE_COMPUTE_BIT) && !indices.ComputeFamilyIsSet())
					{
						indices.computeFamily = i;
					}

				}

				if ((indices.GraphicsFamilyIsSet() || !(queueFlags & VK_QUEUE_GRAPHICS_BIT))
					&& (indices.TransferFamilyIsSet() || !(queueFlags & VK_QUEUE_TRANSFER_BIT))  
					&& (indices.ComputeFamilyIsSet()) || !(queueFlags & VK_QUEUE_COMPUTE_BIT))
				{
					break;
				}
			}
		}

		if (!(indices.GraphicsFamilyIsSet()) && (queueFlags & VK_QUEUE_GRAPHICS_BIT)
			|| !(indices.TransferFamilyIsSet()) && (queueFlags & VK_QUEUE_TRANSFER_BIT)
			|| !(indices.ComputeFamilyIsSet()) && (queueFlags & VK_QUEUE_COMPUTE_BIT))
		{
			throw Invision::VulkanBaseException("Could not find a matching Queue Family index");
		}

		return indices;
	}

	SQueueFamilyIndices FindPresentQueueFamiliy(const VkPhysicalDevice& device, SVulkanContext& vulkanContext, const VkSurfaceKHR surface)
	{
		SQueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			VkBool32 presentSupport = false;
			VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			if (result != VK_SUCCESS) {
				throw Invision::VulkanBaseException(result, "Error while attempting to check if a surface supports presentation:");
			}
			if (queueFamily.queueCount > 0 && presentSupport && VK_QUEUE_GRAPHICS_BIT) {
				vulkanContext.presentFamily = i;
			}
			if (vulkanContext.PresentFamilyIsSet()) {
				break;
			}
			++i;
		}
		return indices;

	}

	SQueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, SVulkanContext& vulkanContext, const VkSurfaceKHR surface)
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
			VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			if (result != VK_SUCCESS) {
				throw Invision::VulkanBaseException(result, "Error while attempting to check if a surface supports presentation:");
			}
			if (queueFamily.queueCount > 0 && presentSupport) {
				vulkanContext.presentFamily = i;
			}
			if (indices.GraphicsFamilyIsSet() && vulkanContext.PresentFamilyIsSet()) {
				break;
			}
			++i;
		}
		return indices;
	}

	SQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) {
		SQueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) {
				indices.graphicsFamily = i;
			}

			if (indices.GraphicsFamilyIsSet()) {
				break;
			}

			i++;
		}

		return indices;
	}

	SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR surface)
	{
		SwapChainSupportDetails details;

		VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
		if (result != VK_SUCCESS) {
			throw Invision::VulkanBaseException(result, "Unable to retrieve physical device surface capabilities:");
		}
		uint32_t formatCount = 0;
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
		if (result != VK_SUCCESS) {
			throw Invision::VulkanBaseException(result, "Unable to retrieve the number of formats for a surface on a physical device:");
		}
		if (formatCount != 0) {
			details.formats.resize(formatCount);
			result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
			if (result != VK_SUCCESS) {
				throw Invision::VulkanBaseException(result, "Unable to retrieve the formats for a surface on a physical device:");
			}
		}

		uint32_t presentModeCount = 0;
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
		if (result != VK_SUCCESS) {
			throw Invision::VulkanBaseException(result, "Unable to retrieve the count of present modes for a surface on a physical device:");
		}
		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
			if (result != VK_SUCCESS) {
				throw Invision::VulkanBaseException(result, "Unable to retrieve the present modes for a surface on a physical device:");
			}
		}
		return details;
	}


	uint32_t findMemoryType(const VkPhysicalDevice& device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(device, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	VkImageView CreateImageView(SVulkanBase &vulkanInstance, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(vulkanInstance.logicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture image view!");
		}

		return imageView;
	}
}