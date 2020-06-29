#ifndef VULKAN_BASE_DESCRIPTOR_POOL_H
#define VULKAN_BASE_DESCRIPTOR_POOL_H

#include "vulkan\vulkan.h"
#include "VulkanBase.h"

namespace Invision
{
	class VulkanBaseDescriptorPool
	{
	private:
		VkDescriptorPool mDescriptorPool;

	public:
		void INVISION_API CreateDescriptorPool(const SVulkanBase &vulkanInstance, std::vector<VkDescriptorPoolSize> poolSizeElements);
		void INVISION_API DestroyDescriptorPool(const SVulkanBase &vulkanInstance);
		
		VkDescriptorPool INVISION_API GetDescriptorPool();
	};
}


#endif // VULKA_BASEN_DESCRIPTOR_POOL_H