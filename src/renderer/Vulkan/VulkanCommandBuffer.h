#ifndef VULKAN_COMMAND_BUFFER_H
#define VULKAN_COMMAND_BUFFER_H

#include "vulkan\vulkan.h"
#include "VulkanCommandPool.h"

namespace Invision
{
	class  VulkanCommandBuffer
	{
	private:
		std::vector<VkCommandBuffer> mCommandBuffers;
		bool mCommandBufferIsInitialized;
		bool mIsCommandBufferRecording;
		bool mIsRenderPassStarted;

	public:
		INVISION_API VulkanCommandBuffer();
		INVISION_API VulkanCommandBuffer(SVulkan &vulkanInstance, VulkanCommandPool &commandPool, unsigned int countOfBuffers);
		INVISION_API VulkanCommandBuffer& CreateCommandBuffer(SVulkan &vulkanInstance, VulkanCommandPool &commandPool, unsigned int countOfBuffers);
		INVISION_API VulkanCommandBuffer& BeginCommandBuffer();
		INVISION_API VulkanCommandBuffer& SetViewport(VkViewport& viewport);
		INVISION_API VulkanCommandBuffer& SetScissor(VkRect2D& rect);
		INVISION_API VulkanCommandBuffer& BeginRenderPass(SVulkan &vulkanInstance, VulkanRenderPass &renderPass, VulkanFramebuffer &vulkanFramebuffer);
		INVISION_API VulkanCommandBuffer& BindPipeline(VulkanPipeline pipeline, VkPipelineBindPoint bindPoint);
		INVISION_API VulkanCommandBuffer& BindVertexBuffer(std::vector<VulkanVertexBuffer> vertexBuffers, uint32_t firstBinding, uint32_t bindingCount);
		INVISION_API VulkanCommandBuffer& Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t firstInstance = 0);
		INVISION_API VulkanCommandBuffer& EndRenderPass();
		INVISION_API VulkanCommandBuffer& EndCommandBuffer();

		std::vector<VkCommandBuffer> INVISION_API GetCommandBuffers();
		VkCommandBuffer* GetCommandBuffer(int index);
	};
}


#endif // VULKAN_COMMAND_BUFFER_H