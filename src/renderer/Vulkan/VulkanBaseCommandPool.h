#ifndef VULKAN_BASE_COMMAND_POOL_H
#define VULKAN_BASE_COMMAND_POOL_H

#include "vulkan\vulkan.h"
#include "VulkanBase.h"

namespace Invision
{
	class  VulkanBaseCommandPool
	{
	private:
		VkCommandPool mCommandPool;

	public:
		void INVISION_API CreateCommandPool(SVulkanBase &vulkanInstance);
		void INVISION_API DestroyCommandPool(SVulkanBase &vulkanInstance);

		VkCommandPool INVISION_API GetCommandPool();

	};
}


#endif // VULKAN_BASE_COMMAND_POOL_H