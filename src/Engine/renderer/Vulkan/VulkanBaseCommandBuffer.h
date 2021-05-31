#ifndef VULKAN_BASE_COMMAND_BUFFER_H
#define VULKAN_BASE_COMMAND_BUFFER_H

#include "vulkan\vulkan.h"

namespace Invision
{

	class VulkanBaseCommandPool;
	class VulkanBaseIndexBuffer;
	class VulkanBaseVertexBuffer;
	class VulkanBaseUniformBuffer;
	class VulkanBasePushConstant;
	class VulkanBasePipeline;
	class VulkanBaseRenderPass;
	class VulkanBaseFramebuffer;
	class VulkanBaseMemoryManager;

	class  VulkanBaseCommandBuffer
	{
	private:
		VkCommandBuffer mCommandBuffer;
		VkSemaphore mCommandBufferSemaphore = VK_NULL_HANDLE;
		bool mCommandBufferIsInitialized;
		bool mIsCommandBufferRecording;
		bool mIsRenderPassStarted;
		bool mIsIndexBufferBinded;
		bool mIsVertexBufferBinded;

	public:
		INVISION_API VulkanBaseCommandBuffer();
		INVISION_API VulkanBaseCommandBuffer(SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool, unsigned int countOfBuffers);
		INVISION_API VulkanBaseCommandBuffer& CreateCommandBuffer(SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool, unsigned int countOfBuffers);
		INVISION_API VulkanBaseCommandBuffer& BeginCommandBuffer();
		INVISION_API VulkanBaseCommandBuffer& SetViewport(VkViewport& viewport);
		INVISION_API VulkanBaseCommandBuffer& SetScissor(VkRect2D& rect);
		INVISION_API VulkanBaseCommandBuffer& BeginRenderPass(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, VulkanBaseRenderPass &renderPass, VulkanBaseFramebuffer &vulkanFramebuffer, unsigned int offsetX, unsigned int offsetY, unsigned int width, unsigned int height, std::vector<VkClearValue> clearValues = {});
		INVISION_API VulkanBaseCommandBuffer& BindPipeline(VulkanBasePipeline& pipeline, VkPipelineBindPoint bindPoint);
		INVISION_API VulkanBaseCommandBuffer& BindVertexBuffer(std::vector<VulkanBaseVertexBuffer> vertexBuffers, VulkanBaseMemoryManager& memoryManager, uint32_t firstBinding, uint32_t bindingCount);
		INVISION_API VulkanBaseCommandBuffer& BindIndexBuffer(VulkanBaseIndexBuffer &indexBuffer, VkIndexType indexType);
		INVISION_API VulkanBaseCommandBuffer& BindDescriptorSets(VulkanBaseUniformBuffer &uniformBuffer, VulkanBasePipeline& pipeline, VkPipelineBindPoint bindPoint);
		INVISION_API VulkanBaseCommandBuffer& BindDescriptorSets(VulkanBaseUniformBuffer &uniformBuffer, VulkanBasePipeline& pipeline, VkPipelineBindPoint bindPoint, uint32_t set);
		INVISION_API VulkanBaseCommandBuffer& PushConstant(VulkanBasePushConstant pushConstant, VulkanBasePipeline& pipeline, const void* data);
		INVISION_API VulkanBaseCommandBuffer& Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t firstInstance = 0);
		INVISION_API VulkanBaseCommandBuffer& DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex = 0, uint32_t vertexOffset = 0, uint32_t firstInstance = 0);
		INVISION_API VulkanBaseCommandBuffer& EndRenderPass();
		INVISION_API VulkanBaseCommandBuffer& EndCommandBuffer();
		INVISION_API void DestroyCommandBuffer(SVulkanBase &vulkanInstance, VulkanBaseCommandPool &commandPool);

		INVISION_API const VkCommandBuffer* GetCommandBuffer();
		INVISION_API VkSemaphore GetSemaphore();
		//VkCommandBuffer* GetCommandBuffer(int index);
	};
}


#endif // VULKAN_BASE_COMMAND_BUFFER_H