#include "precompiled.h"

#include "VulkanBaseException.h"
#include "VulkanBaseRenderPass.h"
#include "VulkanBase.h"

#include "VulkanBaseFramebuffer.h"

namespace Invision
{
	void VulkanBaseFramebuffer::CreateFramebuffer(SVulkanContext &vulkanContext, VulkanBaseRenderPass &renderPass, std::vector< VkImageView>& attachments, int width, int height)
	{

		//for (unsigned int i = 0; i < 3; i++)
		//{
			/*VkImageView attachments[] = {
				vulkanContext.swapChainImageViews[i]
			};*/

			//std::vector< VkImageView> attachments;
			//attachments.push_back(vulkanContext.swapChainImageViews[i]);

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass.GetRenderPass();
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = width;
			framebufferInfo.height = height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(vulkanContext.logicalDevice, &framebufferInfo, nullptr, &mFramebuffer) != VK_SUCCESS)
			{
				throw VulkanBaseException("failed to create framebuffer!");
			}
		//}
	}

	void VulkanBaseFramebuffer::DestroyFramebuffer(SVulkanContext &vulkanContext)
	{
		//for (unsigned int i = 0; i < mFramebuffer.size(); i++)
		//{
			vkDestroyFramebuffer(vulkanContext.logicalDevice, mFramebuffer, nullptr);
		//}
		//mFramebuffer.clear();
	}

	VkFramebuffer VulkanBaseFramebuffer::GetFramebuffers()
	{
		return mFramebuffer;
	}
}