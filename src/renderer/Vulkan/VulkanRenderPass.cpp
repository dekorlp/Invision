#include "precompiled.h"

#include "VulkanRenderPass.h"

namespace Invision
{

	void VulkanRenderPass::CreateRenderPass(const SVulkan &vulkanInstance)
	{
		if (mSubpassDescriptions.size() == 0) {
			throw InvisionException("Subpass Descriptions are empty!");
		}

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = mAttachmentDescriptions.size();
		renderPassInfo.pAttachments = mAttachmentDescriptions.data();
		renderPassInfo.subpassCount = mSubpassDescriptions.size();
		renderPassInfo.pSubpasses = mSubpassDescriptions.data();

		if (vkCreateRenderPass(vulkanInstance.logicalDevice, &renderPassInfo, nullptr, &mRenderPass) != VK_SUCCESS) {
			throw VulkanException("failed to create render pass!");
		}
	}

	void VulkanRenderPass::DestroyRenderPass(const SVulkan &vulkanInstance)
	{
		vkDestroyRenderPass(vulkanInstance.logicalDevice, mRenderPass, nullptr);
	}

}