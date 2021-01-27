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
		
		VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		struct {
			// Swap chain image presentation
			VkSemaphore presentComplete;
			// Command buffer submission and execution
			VkSemaphore renderComplete;
		} mSemaphores;

		//std::vector<VkFence> mWaitFences;

		VkFence renderFence = {};

		VkSubmitInfo mSubmitInfo {};
		
	public:
		void INVISION_API CreateSyncObjects(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext);
		VkResult INVISION_API AquireNextImage(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, unsigned int& imageIndex);
		void INVISION_API DrawFrame(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, VulkanBaseCommandBuffer& commandBuffer);
		VkResult INVISION_API QueuePresent(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, unsigned int imageIndex);

		void INVISION_API DestroySemaphores(SVulkanBase &vulkanInstance);
	};
}
#endif // VULKAN_BASE_RENDERER_H