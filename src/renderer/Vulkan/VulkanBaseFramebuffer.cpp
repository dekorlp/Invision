#include "precompiled.h"

#include "VulkanBaseException.h"
#include "VulkanBaseRenderPass.h"
#include "VulkanBase.h"

#include "VulkanBaseFramebuffer.h"

namespace Invision
{
	void VulkanBaseFramebuffer::CreateFramebuffer(SVulkanBase &vulkanInstance, SVulkanContext &vulkanContext, VulkanBaseRenderPass &renderPass)
	{
		mFramebuffer.resize(vulkanContext.swapChainImageViews.size());

		for (unsigned int i = 0; i < vulkanContext.swapChainImageViews.size(); i++)
		{
			VkImageView attachments[] = {
				vulkanContext.swapChainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass.GetRenderPass();
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = vulkanContext.swapChainExtent.width;
			framebufferInfo.height = vulkanContext.swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(vulkanInstance.logicalDevice, &framebufferInfo, nullptr, &mFramebuffer[i]) != VK_SUCCESS)
			{
				throw VulkanBaseException("failed to create framebuffer!");
			}
		}
	}

	void VulkanBaseFramebuffer::DestroyFramebuffer(SVulkanBase &vulkanInstance)
	{
		for (unsigned int i = 0; i < mFramebuffer.size(); i++)
		{
			vkDestroyFramebuffer(vulkanInstance.logicalDevice, mFramebuffer[i], nullptr);
		}
		mFramebuffer.clear();
	}

	std::vector<VkFramebuffer> VulkanBaseFramebuffer::GetFramebuffers()
	{
		return mFramebuffer;
	}
}