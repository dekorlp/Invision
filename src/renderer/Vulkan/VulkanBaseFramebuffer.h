#ifndef VULKAN_BASE_FRAMEBUFFER_H
#define VULKAN_BASE_FRAMEBUFFER_H

#include "vulkan\vulkan.h"

namespace Invision
{
	class VulkanBaseRenderPass;
	class VulkanBaseDepthRessources;

	class  VulkanBaseFramebuffer
	{
	private:
		std::vector<VkFramebuffer> mFramebuffer;
	public:
		void INVISION_API CreateFramebuffer(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, VulkanBaseRenderPass &renderPass, unsigned int countFrameBuffers);
		void INVISION_API CreateFramebuffer(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, VulkanBaseRenderPass &renderPass, VulkanBaseDepthRessources &depthRessources, unsigned int countFrameBuffers);
		void INVISION_API DestroyFramebuffer(SVulkanBase &vulkanInstance);
		INVISION_API std::vector<VkFramebuffer> GetFramebuffers();
	};
}

#endif //VULKAN_BASE_FRAMEBUFFER_H