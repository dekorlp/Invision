#ifndef VULKAN_COMMAND_BUFFER_H
#define VULKAN_COMMAND_BUFFER_H

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"
#include "VulkanFramebuffer.h"
#include "VulkanPipeline.h"
#include "VulkanRenderPass.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace Invision
{
	class  VulkanCommandBuffer
	{
	private:
		VkCommandPool mCommandPool;
		std::vector<VkCommandBuffer> mCommandBuffers;
		std::vector<VkSemaphore> mImageAvailableSemaphore;
		std::vector<VkSemaphore> mRenderFinishedSemaphore;
		size_t mCurrentFrame = 0;
		std::vector<VkFence> mInFlightFences;
		std::vector<VkFence> mImagesInFlight;
		uint32_t mImageIndex;

	public:
		void INVISION_API CreateCommandPool(SVulkan &vulkanInstance);
		void INVISION_API CreateCommandBuffers(SVulkan &vulkanInstance, VulkanFramebuffer &vulkanFramebuffer, VulkanPipeline &vulkanPipeline, VulkanRenderPass &renderPass);
		void INVISION_API CreateSyncObjects(SVulkan &vulkanInstance);
		VkResult INVISION_API AquireNextImage(SVulkan &vulkanInstance);
		VkResult INVISION_API DrawFrame(SVulkan &vulkanInstance);

		void INVISION_API DestroyCommandPool(SVulkan &vulkanInstance);
		void INVISION_API DestroySemaphores(SVulkan &vulkanInstance);
	};
}


#endif // VULKAN_COMMAND_BUFFER_H