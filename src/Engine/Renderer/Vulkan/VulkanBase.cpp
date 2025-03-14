#include "precompiled.h"

#include "VulkanBaseException.h"

#include "VulkanBase.h"
namespace Invision
{

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

	VkImageView CreateImageView(SVulkanContext &vulkanContext, VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevel, uint32_t layerCount)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = viewType;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevel;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = layerCount;

		VkImageView imageView;
		if (vkCreateImageView(vulkanContext.logicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture image view!");
		}

		return imageView;
	}

	std::vector<uint32_t> SVulkanContext::GetQueueFamilyIndices(bool graphicsQueueSupporte, bool transferQueueSupported, bool computeFamilySupported, bool presentFamilySupported)
	{
		std::vector<uint32_t> queueFamilyIndices;

		for (unsigned int i = 0; i < queueFamilies.size(); i++)
		{
			bool specificationChecked = false;
			if (graphicsQueueSupporte == true)
			{
				if (queueFamilies[i].GraphicsFamilyIsSet() == graphicsQueueSupporte)
				{
					specificationChecked = true;
				}
				else
				{
					continue;
				}
			}

			if (transferQueueSupported == true)
			{
				if (queueFamilies[i].TransferFamilyIsSet() == transferQueueSupported)
				{
					specificationChecked = true;
				}
				else
				{
					continue;
				}
			}

			if (computeFamilySupported == true)
			{
				if (queueFamilies[i].ComputeFamilyIsSet() == computeFamilySupported)
				{
					specificationChecked = true;
				}
				else
				{
					continue;
				}
			}

			if (presentFamilySupported == true)
			{
				if (queueFamilies[i].PresentFamilyIsSet() == presentFamilySupported)
				{
					specificationChecked = true;
				}
				else
				{
					continue;
				}
			}

			if (specificationChecked)
			{
				queueFamilyIndices.push_back(i);
			}
		}

		return queueFamilyIndices;
	}

	std::vector<uint32_t> SVulkanContext::GetUniqueQueueFamilyIndices(bool graphicsQueueSupporte, bool transferQueueSupported, bool computeFamilySupported, bool presentFamilySupported)
	{
		std::vector<uint32_t> queueFamilyIndices;


		for (unsigned int i = 0; i < queueFamilies.size(); i++)
		{

			if (graphicsQueueSupporte == true)
			{
				if (queueFamilies[i].GraphicsFamilyIsSet() == graphicsQueueSupporte)
				{
					graphicsQueueSupporte = false;
					queueFamilyIndices.push_back(i);
					continue;
				}
			}

			if (transferQueueSupported == true)
			{
				if (queueFamilies[i].TransferFamilyIsSet() == transferQueueSupported)
				{
					transferQueueSupported = false;
					queueFamilyIndices.push_back(i);
					continue;
				}
			}

			if (computeFamilySupported == true)
			{
				if (queueFamilies[i].ComputeFamilyIsSet() == computeFamilySupported)
				{
					computeFamilySupported = false;
					queueFamilyIndices.push_back(i);
					continue;
				}
			}

			if (presentFamilySupported == true)
			{
				if (queueFamilies[i].PresentFamilyIsSet() == presentFamilySupported)
				{
					presentFamilySupported = false;
					queueFamilyIndices.push_back(i);
					continue;
				}
			}
		}

		return queueFamilyIndices;
	}
}