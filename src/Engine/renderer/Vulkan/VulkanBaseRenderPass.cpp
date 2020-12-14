#include "precompiled.h"
#include "VulkanBase.h"
#include "VulkanBaseException.h"

#include "VulkanBaseRenderPass.h"

namespace Invision
{

	void VulkanBaseRenderPass::CreateRenderPass(const SVulkanBase &vulkanInstance)
	{
		/*if (subPasses.mSubpass.size() == 0) {
			throw InvisionBaseRendererException("Subpass Descriptions are empty!");
		}*/

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<unsigned int>(mAttachmentDescriptions.size());
		renderPassInfo.pAttachments = mAttachmentDescriptions.data();
		renderPassInfo.subpassCount = static_cast<unsigned int>(mSubpasses.size());
		renderPassInfo.pSubpasses = mSubpasses.data();
		renderPassInfo.dependencyCount = static_cast<unsigned int>(mDependencies.size());
		renderPassInfo.pDependencies = mDependencies.data();

		if (vkCreateRenderPass(vulkanInstance.logicalDevice, &renderPassInfo, nullptr, &mRenderPass) != VK_SUCCESS) {
			throw InvisionBaseRendererException("failed to create render pass!");
		}
	}

	void VulkanBaseRenderPass::AddAttachment(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext, VkFormat format, VkAttachmentStoreOp storeOp, VkImageLayout finalLayout)
	{
		VkAttachmentDescription attachment = {};
		attachment.format = format;
		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachment.storeOp = storeOp;
		attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachment.finalLayout = finalLayout;

		mAttachmentDescriptions.push_back(attachment);
	}
	void VulkanBaseRenderPass::AddSubpass(BaseSubPass subPass, bool useDepthRessource)
	{
		// TODO: This is the first Version
		// This has to be refactured later
		VkAttachmentReference attachmentRef = {};
		attachmentRef.attachment = 0;
		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		subPass.mColorReference.push_back(attachmentRef);

		if (useDepthRessource)
		{
			VkAttachmentReference depthAttachmentRef = {};
			depthAttachmentRef.attachment = 1;
			depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			subPass.mDepthReference.push_back(depthAttachmentRef);
		}

		mSubpassesReferences.push_back(subPass);

		

		VkSubpassDescription subpassDesc = {};
		//VkSubpassDescription subpass = {};
		subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDesc.colorAttachmentCount = static_cast<unsigned int>(mSubpassesReferences.at(static_cast<unsigned int>(mSubpassesReferences.size()) - 1).mColorReference.size());
		subpassDesc.pColorAttachments = mSubpassesReferences.at(static_cast<unsigned int>(mSubpassesReferences.size()) - 1).mColorReference.data();

		if (useDepthRessource)
		{
			subpassDesc.pDepthStencilAttachment = mSubpassesReferences.at(static_cast<unsigned int>(mSubpassesReferences.size()) - 1).mDepthReference.data();
		}

	

		mSubpasses.push_back(subpassDesc);
	}

	void VulkanBaseRenderPass::AddSubpassDependency(const SVulkanBase &vulkanInstance, VkPipelineStageFlags srcStageFlags, VkAccessFlags srcAccessFlags, VkPipelineStageFlags dstStageFlags, VkAccessFlags dstAccessFlags)
	{
		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = srcStageFlags;
		dependency.srcAccessMask = srcAccessFlags;
		dependency.dstStageMask = dstStageFlags;
		dependency.dstAccessMask = dstAccessFlags;

		mDependencies.push_back(dependency);
	}

	VkRenderPass VulkanBaseRenderPass::GetRenderPass()
	{
		return mRenderPass;
	}

	void VulkanBaseRenderPass::DestroyRenderPass(const SVulkanBase &vulkanInstance)
	{
		vkDestroyRenderPass(vulkanInstance.logicalDevice, mRenderPass, nullptr);
		mAttachmentDescriptions.clear();
		mSubpasses.clear();
		mSubpassesReferences.clear();
		mDependencies.clear();
	}

}