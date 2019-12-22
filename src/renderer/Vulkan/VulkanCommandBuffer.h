#ifndef VULKAN_COMMAND_BUFFER_H
#define VULKAN_COMMAND_BUFFER_H

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"
#include "VulkanFramebuffer.h"
#include "VulkanPipeline.h"
#include "VulkanRenderPass.h"
#include "VulkanCommandPool.h"

namespace Invision
{
	class  VulkanCommandBuffer
	{
	private:
		std::vector<VkCommandBuffer> mCommandBuffers;

	public:
		void INVISION_API CreateCommandBuffers(SVulkan &vulkanInstance, VulkanCommandPool &commandPool, VulkanFramebuffer &vulkanFramebuffer, VulkanPipeline &vulkanPipeline, VulkanRenderPass &renderPass, VkViewport viewport = {}, VkRect2D scissor = {});
		std::vector<VkCommandBuffer> INVISION_API GetCommandBuffers();
		VkCommandBuffer* GetCommandBuffer(int index);
	};
}


#endif // VULKAN_COMMAND_BUFFER_H