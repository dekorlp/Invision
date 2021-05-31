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
		INVISION_API void CreateSyncObjects(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext);
		INVISION_API void AlterSubmitInfo(const uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, const VkPipelineStageFlags* pWaitDstStageMask, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers, uint32_t signalSemaphoreCount, const VkSemaphore* pSignalSemaphores);
		INVISION_API void AlterSubmitInfo(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
		INVISION_API void AlterSubmitInfoSignalSemaphore(uint32_t signalSemaphoreCount, const VkSemaphore* pSignalSemaphores);
		INVISION_API void AlterSubmitInfoWaitSemaphore(uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores);

		INVISION_API VkSemaphore* GetSemaphoresPresentComplete();
		INVISION_API VkSemaphore* GetSemaphoresRenderComplete();

		INVISION_API VkResult AquireNextImage(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, unsigned int& imageIndex);
		INVISION_API void DrawFrame(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, VulkanBaseCommandBuffer& commandBuffer);
		INVISION_API VkResult QueuePresent(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, unsigned int imageIndex);

		INVISION_API void DestroySemaphores(SVulkanBase &vulkanInstance);
	};
}
#endif // VULKAN_BASE_RENDERER_H