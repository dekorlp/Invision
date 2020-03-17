#include "precompiled.h"


#include "VulkanException.h"

#include "VulkanDescriptorPool.h"

namespace Invision
{

	void VulkanDescriptorPool::CreateDescriptorPool(const SVulkan &vulkanInstance, std::vector<VkDescriptorPoolSize> poolSizeElements)
	{
		//VkDescriptorPoolSize poolSize = {};
		//poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		//poolSize.descriptorCount = static_cast<uint32_t>(vulkanInstance.swapChainImages.size());

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = poolSizeElements.size();
		poolInfo.pPoolSizes = poolSizeElements.data();
		poolInfo.maxSets = static_cast<uint32_t>(vulkanInstance.swapChainImages.size());

		if (vkCreateDescriptorPool(vulkanInstance.logicalDevice, &poolInfo, nullptr, &mDescriptorPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}

	void VulkanDescriptorPool::DestroyDescriptorPool(const SVulkan &vulkanInstance)
	{
		vkDestroyDescriptorPool(vulkanInstance.logicalDevice, mDescriptorPool, nullptr);
	}

	VkDescriptorPool VulkanDescriptorPool::GetDescriptorPool()
	{
		return mDescriptorPool;
	}

}