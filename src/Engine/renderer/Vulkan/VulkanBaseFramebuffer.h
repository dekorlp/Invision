#ifndef VULKAN_BASE_FRAMEBUFFER_H
#define VULKAN_BASE_FRAMEBUFFER_H

#include "vulkan\vulkan.h"

namespace Invision
{
	class VulkanBaseRenderPass;

	class  VulkanBaseFramebuffer
	{
	private:
		VkFramebuffer mFramebuffer;
	public:
		void INVISION_API CreateFramebuffer(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, VulkanBaseRenderPass &renderPass, std::vector< VkImageView>& attachments);
		void INVISION_API DestroyFramebuffer(SVulkanBase &vulkanInstance);
		INVISION_API VkFramebuffer GetFramebuffers();
	};
}

#endif //VULKAN_BASE_FRAMEBUFFER_H