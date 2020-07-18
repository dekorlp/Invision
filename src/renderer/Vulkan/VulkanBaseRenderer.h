#ifndef VULKAN_BASE_RENDERER_H
#define VULKAN_BASE_RENDERER_H

#include "vulkan\vulkan.h"

namespace Invision
{

	struct SVulkanBase;
	class VulkanBaseCommandBuffer;

	class  VulkanBaseRenderer
	{
	private:
		std::vector<VkSemaphore> mImageAvailableSemaphore;
		std::vector<VkSemaphore> mRenderFinishedSemaphore;
		size_t mCurrentFrame = 0;
		std::vector<VkFence> mInFlightFences;
		std::vector<VkFence> mImagesInFlight;
		

	public:
		void INVISION_API CreateSyncObjects(SVulkanBase &vulkanInstance);
		VkResult INVISION_API AquireNextImage(SVulkanBase &vulkanInstance);
		VkResult INVISION_API DrawFrame(SVulkanBase &vulkanInstance, VulkanBaseCommandBuffer& commandBuffer);

		void INVISION_API DestroySemaphores(SVulkanBase &vulkanInstance);
	};
}
#endif // VULKAN_BASE_RENDERER_H