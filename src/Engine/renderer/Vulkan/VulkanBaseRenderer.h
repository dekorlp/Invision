#ifndef VULKAN_BASE_RENDERER_H
#define VULKAN_BASE_RENDERER_H

#include "vulkan\vulkan.h"

namespace Invision
{

	struct SVulkanBase;
	struct SVulkanContext;
	class VulkanBaseCommandBuffer;

	class  VulkanBaseRenderer
	{
	private:
		
		VkPipelineStageFlags mWaitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		struct {
			// Swap chain image presentation
			VkSemaphore presentComplete;
			// Command buffer submission and execution
			VkSemaphore renderComplete;
		} mSemaphores;

		//std::vector<VkFence> mWaitFences;

		VkFence mRenderFence = {};

		VkSubmitInfo mSubmitInfo {};
		
	public:
		void INVISION_API CreateSyncObjects(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext);
		void INVISION_API AlterSubmitInfo(const uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, const VkPipelineStageFlags* pWaitDstStageMask, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers, uint32_t signalSemaphoreCount, const VkSemaphore* pSignalSemaphores);
		VkResult INVISION_API AquireNextImage(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, unsigned int& imageIndex);
		void INVISION_API DrawFrame(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, VulkanBaseCommandBuffer& commandBuffer);
		VkResult INVISION_API QueuePresent(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, unsigned int imageIndex);

		void INVISION_API DestroySemaphores(SVulkanBase &vulkanInstance);
	};
}
#endif // VULKAN_BASE_RENDERER_H