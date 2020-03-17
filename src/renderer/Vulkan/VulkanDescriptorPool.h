#ifndef VULKAN_DESCRIPTOR_POOL_H
#define VULKAN_DESCRIPTOR_POOL_H

#include "vulkan\vulkan.h"
#include "Vulkan.h"

namespace Invision
{
	class VulkanDescriptorPool
	{
	private:
		VkDescriptorPool mDescriptorPool;

	public:
		void INVISION_API CreateDescriptorPool(const SVulkan &vulkanInstance, std::vector<VkDescriptorPoolSize> poolSizeElements);
		void INVISION_API DestroyDescriptorPool(const SVulkan &vulkanInstance);
		
		VkDescriptorPool INVISION_API GetDescriptorPool();
	};
}


#endif // VULKAN_DESCRIPTOR_POOL_H