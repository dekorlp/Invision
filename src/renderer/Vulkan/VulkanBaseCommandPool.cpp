#include "precompiled.h"


#include "VulkanBaseException.h"

#include "VulkanBaseCommandPool.h"

namespace Invision
{
	void VulkanBaseCommandPool::CreateCommandPool(SVulkanBase &vulkanInstance)
	{
		SQueueFamilyIndices queueFamilyIndices = Invision::FindQueueFamilies(vulkanInstance.physicalDevice);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		poolInfo.flags = 0;

		if (vkCreateCommandPool(vulkanInstance.logicalDevice, &poolInfo, nullptr, &mCommandPool) != VK_SUCCESS)
		{
			throw VulkanBaseException("failed to create Command pool!");
		}
	}

	void VulkanBaseCommandPool::DestroyCommandPool(SVulkanBase &vulkanInstance)
	{
		vkDestroyCommandPool(vulkanInstance.logicalDevice, mCommandPool, nullptr);
	}

	VkCommandPool VulkanBaseCommandPool::GetCommandPool()
	{
		return mCommandPool;
	}
}