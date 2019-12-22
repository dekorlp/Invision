#ifndef VULKAN_COMMAND_POOL_H
#define VULKAN_COMMAND_POOL_H

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"
#include "VulkanFramebuffer.h"
#include "VulkanPipeline.h"
#include "VulkanRenderPass.h"


namespace Invision
{
	class  VulkanCommandPool
	{
	private:
		VkCommandPool mCommandPool;

	public:
		void INVISION_API CreateCommandPool(SVulkan &vulkanInstance);
		void INVISION_API DestroyCommandPool(SVulkan &vulkanInstance);

		VkCommandPool INVISION_API GetCommandPool();

	};
}


#endif // VULKAN_COMMAND_POOL_H