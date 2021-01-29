#include "precompiled.h"


#include "VulkanBaseException.h"

#include "VulkanBaseColorRessources.h"

namespace Invision
{

	void VulkanBaseColorRessources::CreateColorRessources(SVulkanBase &vulkanInstance,  VulkanBaseCommandPool commandPool, SVulkanContext &vulkanContext)
	{
		VkFormat colorFormat = vulkanContext.swapChainImageFormat;

		CreateImage(vulkanInstance, vulkanContext.swapChainExtent.width, vulkanContext.swapChainExtent.height, 1, vulkanContext.MsaaFlagBits, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mColorImage, mColorImageMemory);
		mColorImageView = CreateImageView(vulkanInstance, mColorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}

	void VulkanBaseColorRessources::DestroyColorRessources(const SVulkanBase &vulkanInstance)
	{
		vkDestroyImageView(vulkanInstance.logicalDevice, mColorImageView, nullptr);
		vkDestroyImage(vulkanInstance.logicalDevice, mColorImage, nullptr);
		vkFreeMemory(vulkanInstance.logicalDevice, mColorImageMemory, nullptr);
	}

	VkImageView VulkanBaseColorRessources::GetColorImageView()
	{
		return mColorImageView;
	}

}