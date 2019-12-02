#ifndef VULKAN_COMMAND_BUFFER_H
#define VULKAN_COMMAND_BUFFER_H

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"
#include "VulkanFramebuffer.h"
#include "VulkanPipeline.h"
#include "VulkanRenderPass.h"

namespace Invision
{
	class  VulkanCommandBuffer
	{
	private:
		VkCommandPool mCommandPool;
		std::vector<VkCommandBuffer> mCommandBuffers;
		VkSemaphore mImageAvailableSemaphore;
		VkSemaphore mRenderFinishedSemaphore;

	public:
		void INVISION_API CreateCommandPool(SVulkan &vulkanInstance);
		void INVISION_API CreateCommandBuffers(SVulkan &vulkanInstance, VulkanFramebuffer &vulkanFramebuffer, VulkanPipeline &vulkanPipeline, VulkanRenderPass &renderPass);
		void INVISION_API CreateSemaphores(SVulkan &vulkanInstance);
		void INVISION_API DrawFrame(SVulkan &vulkanInstance);

		void INVISION_API DestroyCommandPool(SVulkan &vulkanInstance);
		void INVISION_API DestroySemaphores(SVulkan &vulkanInstance);
	};
}


#endif // VULKAN_COMMAND_BUFFER_H