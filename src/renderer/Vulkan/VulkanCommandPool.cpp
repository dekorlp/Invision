#include "precompiled.h"


#include "VulkanException.h"

#include "VulkanCommandPool.h"

namespace Invision
{
	void VulkanCommandPool::CreateCommandPool(SVulkan &vulkanInstance)
	{
		SQueueFamilyIndices queueFamilyIndices = Invision::FindQueueFamilies(vulkanInstance.physicalDevice);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		poolInfo.flags = 0;

		if (vkCreateCommandPool(vulkanInstance.logicalDevice, &poolInfo, nullptr, &mCommandPool) != VK_SUCCESS)
		{
			throw VulkanException("failed to create Command pool!");
		}
	}

	void VulkanCommandPool::DestroyCommandPool(SVulkan &vulkanInstance)
	{
		vkDestroyCommandPool(vulkanInstance.logicalDevice, mCommandPool, nullptr);
	}

	VkCommandPool VulkanCommandPool::GetCommandPool()
	{
		return mCommandPool;
	}
}