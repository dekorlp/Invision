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
		void INVISION_API CreateFramebuffer(SVulkanContext &vulkanContext, VulkanBaseRenderPass &renderPass, std::vector< VkImageView>& attachments, int width, int height);
		void INVISION_API DestroyFramebuffer(SVulkanContext& vulkanContext);
		INVISION_API VkFramebuffer GetFramebuffers();
	};
}

#endif //VULKAN_BASE_FRAMEBUFFER_H