#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include "vulkan\vulkan.h"


const int MAX_FRAMES_IN_FLIGHT = 2;

namespace Invision
{
	class  VulkanRenderer
	{
	private:
		std::vector<VkSemaphore> mImageAvailableSemaphore;
		std::vector<VkSemaphore> mRenderFinishedSemaphore;
		size_t mCurrentFrame = 0;
		std::vector<VkFence> mInFlightFences;
		std::vector<VkFence> mImagesInFlight;
		uint32_t mImageIndex;

	public:
		void INVISION_API CreateSyncObjects(SVulkan &vulkanInstance);
		VkResult INVISION_API AquireNextImage(SVulkan &vulkanInstance);
		VkResult INVISION_API DrawFrame(SVulkan &vulkanInstance, VulkanCommandBuffer& commandBuffer);

		void INVISION_API DestroySemaphores(SVulkan &vulkanInstance);
	};
}
#endif // VULKAN_RENDERER_H