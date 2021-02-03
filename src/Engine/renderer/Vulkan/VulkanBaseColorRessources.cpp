#include "precompiled.h"


#include "VulkanBaseException.h"
#include "VulkanBaseMemoryManager.h"

#include "VulkanBaseColorRessources.h"

namespace Invision
{

	void VulkanBaseColorRessources::CreateColorRessources(SVulkanBase &vulkanInstance,  VulkanBaseCommandPool commandPool, VulkanBaseMemoryManager& memoryManager, SVulkanContext &vulkanContext)
	{
		mMemoryManager = &memoryManager;
		VkFormat colorFormat = vulkanContext.swapChainImageFormat;

		mpImage = CreateImage(vulkanInstance, memoryManager, vulkanContext.swapChainExtent.width, vulkanContext.swapChainExtent.height, 1, vulkanInstance.MsaaFlagBits, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mColorImage);
		mColorImageView = CreateImageView(vulkanInstance, mColorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}

	void VulkanBaseColorRessources::DestroyColorRessources(const SVulkanBase &vulkanInstance)
	{
		vkDestroyImageView(vulkanInstance.logicalDevice, mColorImageView, nullptr);
		vkDestroyImage(vulkanInstance.logicalDevice, mColorImage, nullptr);
		mMemoryManager->Unbind(vulkanInstance, mpImage);
	}

	VkImageView VulkanBaseColorRessources::GetColorImageView()
	{
		return mColorImageView;
	}

}