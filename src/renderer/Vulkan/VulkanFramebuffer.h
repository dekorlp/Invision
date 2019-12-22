#ifndef VULKAN_FRAMEBUFFER_H
#define VULKAN_FRAMEBUFFER_H

#include "vulkan\vulkan.h"
#include "VulkanException.h"
#include "VulkanRenderPass.h"
#include "Vulkan.h"

namespace Invision
{
	class  VulkanFramebuffer
	{
	private:
		std::vector<VkFramebuffer> mFramebuffer;
	public:
		void INVISION_API CreateFramebuffer(SVulkan &vulkanInstance, VulkanRenderPass &renderPass);
		void INVISION_API DestroyFramebuffer(SVulkan &vulkanInstance);
		INVISION_API std::vector<VkFramebuffer> GetFramebuffers();
	};
}

#endif //VULKAN_FRAMEBUFFER_H