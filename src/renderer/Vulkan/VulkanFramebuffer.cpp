#include "precompiled.h"

#include "VulkanFramebuffer.h"

namespace Invision
{
	void VulkanFramebuffer::CreateFramebuffer(SVulkan &vulkanInstance, VulkanRenderPass &renderPass)
	{
		mFramebuffer.resize(vulkanInstance.swapChainImageViews.size());

		for (unsigned int i = 0; i < vulkanInstance.swapChainImageViews.size(); i++)
		{
			VkImageView attachments[] = {
				vulkanInstance.swapChainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass.GetRenderPass();
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = vulkanInstance.swapChainExtent.width;
			framebufferInfo.height = vulkanInstance.swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(vulkanInstance.logicalDevice, &framebufferInfo, nullptr, &mFramebuffer[i]) != VK_SUCCESS)
			{
				throw VulkanException("failed to create framebuffer!");
			}
		}
	}

	void VulkanFramebuffer::DestroyFramebuffer(SVulkan &vulkanInstance)
	{
		for (unsigned int i = 0; i < mFramebuffer.size(); i++)
		{
			vkDestroyFramebuffer(vulkanInstance.logicalDevice, mFramebuffer[i], nullptr);
		}
	}
}