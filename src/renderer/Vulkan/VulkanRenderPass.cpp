#include "precompiled.h"

#include "VulkanRenderPass.h"

namespace Invision
{

	void VulkanRenderPass::CreateRenderPass(const SVulkan &vulkanInstance)
	{
		/*if (subPasses.mSubpass.size() == 0) {
			throw InvisionBaseRendererException("Subpass Descriptions are empty!");
		}*/

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = mAttachmentDescriptions.size();
		renderPassInfo.pAttachments = mAttachmentDescriptions.data();
		renderPassInfo.subpassCount = mSubpasses.size();
		renderPassInfo.pSubpasses = mSubpasses.data();

		if (vkCreateRenderPass(vulkanInstance.logicalDevice, &renderPassInfo, nullptr, &mRenderPass) != VK_SUCCESS) {
			throw InvisionBaseRendererException("failed to create render pass!");
		}
	}

	void VulkanRenderPass::AddAttachment(const SVulkan &vulkanInstance)
	{
		VkAttachmentDescription attachment = {};
		attachment.format = vulkanInstance.swapChainImageFormat;
		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		mAttachmentDescriptions.push_back(attachment);
	}
	void VulkanRenderPass::AddSubpass(SubPass subPass)
	{
		// TODO: This is the first Version
		// This has to be refactured later
		VkAttachmentReference attachmentRef = {};
		attachmentRef.attachment = 0;
		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		subPass.mColorReference.push_back(attachmentRef);
		mSubpassesReferences.push_back(subPass);

		VkSubpassDescription subpassDesc = {};
		//VkSubpassDescription subpass = {};
		subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDesc.colorAttachmentCount = mSubpassesReferences.at(mSubpassesReferences.size() - 1).mColorReference.size();
		subpassDesc.pColorAttachments = mSubpassesReferences.at(mSubpassesReferences.size() - 1).mColorReference.data();
		mSubpasses.push_back(subpassDesc);
	}

	VkRenderPass VulkanRenderPass::GetRenderPass()
	{
		return mRenderPass;
	}

	void VulkanRenderPass::DestroyRenderPass(const SVulkan &vulkanInstance)
	{
		vkDestroyRenderPass(vulkanInstance.logicalDevice, mRenderPass, nullptr);
	}

}