#include "precompiled.h"


#include "VulkanBaseException.h"

#include "VulkanBaseCommandPool.h"

namespace Invision
{
	void VulkanBaseCommandPool::CreateCommandPool(SVulkanContext &vulkanContext)
	{
		//SQueueFamilyIndices queueFamilyIndices = Invision::FindQueueFamilies(vulkanInstance.physicalDeviceStruct.physicalDevice, VK_QUEUE_GRAPHICS_BIT);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = vulkanContext.GetQueueFamilyIndices(true, false, false, false)[0];
		poolInfo.flags = 0;

		if (vkCreateCommandPool(vulkanContext.logicalDevice, &poolInfo, nullptr, &mCommandPool) != VK_SUCCESS)
		{
			throw VulkanBaseException("failed to create Command pool!");
		}
	}

	void VulkanBaseCommandPool::DestroyCommandPool(SVulkanContext &vulkanContext)
	{
		vkDestroyCommandPool(vulkanContext.logicalDevice, mCommandPool, nullptr);
	}

	VkCommandPool VulkanBaseCommandPool::GetCommandPool()
	{
		return mCommandPool;
	}
}