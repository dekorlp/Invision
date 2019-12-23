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
		bool mCommandBufferIsInitialized;
		bool mIsCommandBufferRecording;
		bool mIsRenderPassStarted;

	public:
		INVISION_API VulkanCommandBuffer();
		INVISION_API VulkanCommandBuffer(SVulkan &vulkanInstance, VulkanCommandPool &commandPool, unsigned int countOfBuffers);
		void INVISION_API CreateCommandBuffer(SVulkan &vulkanInstance, VulkanCommandPool &commandPool, unsigned int countOfBuffers);
		void INVISION_API BeginCommandBuffer();
		void INVISION_API SetViewport(VkViewport& viewport);
		void INVISION_API SetScissor(VkRect2D& rect);
		void INVISION_API BeginRenderPass(SVulkan &vulkanInstance, VulkanRenderPass &renderPass, VulkanFramebuffer &vulkanFramebuffer);
		void INVISION_API BindPipeline(VulkanPipeline pipeline, VkPipelineBindPoint bindPoint);
		void INVISION_API Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t firstInstance = 0);
		void INVISION_API EndRenderPass();
		void INVISION_API EndCommandBuffer();

		std::vector<VkCommandBuffer> INVISION_API GetCommandBuffers();
		VkCommandBuffer* GetCommandBuffer(int index);
	};
}


#endif // VULKAN_COMMAND_BUFFER_H