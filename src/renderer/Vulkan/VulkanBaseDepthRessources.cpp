#include "precompiled.h"


#include "VulkanBaseException.h"

#include "VulkanBaseDepthRessources.h"

namespace Invision
{
	VulkanBaseDepthRessources::VulkanBaseDepthRessources() : mDepthImageView(VK_NULL_HANDLE), mDepthImage(VK_NULL_HANDLE), mDepthImageMemory(VK_NULL_HANDLE), mUseDepthRessources(false)
	{
		mUseDepthRessources = false;
	}

	void VulkanBaseDepthRessources::CreateDepthRessources(SVulkanBase &vulkanInstance,  VulkanBaseCommandPool commandPool, SVulkanContext &vulkanContext)
	{
		VkFormat depthFormat = findDepthFormat(vulkanInstance);
		
		CreateImage(vulkanInstance, vulkanContext.swapChainExtent.width, vulkanContext.swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mDepthImage, mDepthImageMemory);
		mDepthImageView = CreateImageView(vulkanInstance, mDepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

		mUseDepthRessources = true;
	}
	
	VkFormat VulkanBaseDepthRessources::findSupportedFormat(const SVulkanBase &vulkanInstance, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			
			vkGetPhysicalDeviceFormatProperties(vulkanInstance.physicalDeviceStruct.physicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}

		throw VulkanBaseException("failed to find supported format!");
	}

	VkFormat VulkanBaseDepthRessources::findDepthFormat(const SVulkanBase &vulkanInstance)
	{
		return findSupportedFormat(vulkanInstance,
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	bool VulkanBaseDepthRessources::hasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	VkImageView VulkanBaseDepthRessources::GetDepthImageView()
	{
		return mDepthImageView;
	}

	bool VulkanBaseDepthRessources::AreDepthRessourcesActivated()
	{
		return mUseDepthRessources;
	}

	void VulkanBaseDepthRessources::DestroyDepthRessources(const SVulkanBase &vulkanInstance)
	{
		vkDestroyImageView(vulkanInstance.logicalDevice, mDepthImageView, nullptr);
		vkDestroyImage(vulkanInstance.logicalDevice, mDepthImage, nullptr);
		vkFreeMemory(vulkanInstance.logicalDevice, mDepthImageMemory, nullptr);
	}


}