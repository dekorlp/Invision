#include "precompiled.h"


#include "VulkanBaseException.h"

#include "VulkanBaseDescriptorPool.h"

namespace Invision
{

	void VulkanBaseDescriptorPool::CreateDescriptorPool(const SVulkanContext &vulkanContext, std::vector<VkDescriptorPoolSize> poolSizeElements)
	{
		//VkDescriptorPoolSize poolSize = {};
		//poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		//poolSize.descriptorCount = static_cast<uint32_t>(vulkanInstance.swapChainImages.size());

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizeElements.size());
		poolInfo.pPoolSizes = poolSizeElements.data();
		poolInfo.maxSets = static_cast<uint32_t>(vulkanContext.swapChainImages.size());

		if (vkCreateDescriptorPool(vulkanContext.logicalDevice, &poolInfo, nullptr, &mDescriptorPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}

	void VulkanBaseDescriptorPool::DestroyDescriptorPool(const SVulkanContext &vulkanContext)
	{
		vkDestroyDescriptorPool(vulkanContext.logicalDevice, mDescriptorPool, nullptr);
	}

	VkDescriptorPool VulkanBaseDescriptorPool::GetDescriptorPool()
	{
		return mDescriptorPool;
	}

}