#include "precompiled.h"
#include "VulkanBase.h"
#include "VulkanBaseException.h"

#include "VulkanBaseRenderPass.h"

namespace Invision
{

	void VulkanBaseSubPass::AddAttachment(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext, VkFormat format, VkSampleCountFlagBits numSamples,
		VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreop, VkImageLayout initialLayout, VkImageLayout finalLayout
		, VkAttachmentReference attachmentRef = {}, bool isResolveAttachment)
	{
		VkAttachmentDescription attachment = {};
		attachment.format = format;
		attachment.samples = numSamples;
		attachment.loadOp = loadOp;
		attachment.storeOp = storeOp;
		attachment.stencilLoadOp = stencilLoadOp;
		attachment.stencilStoreOp = stencilStoreop;
		attachment.initialLayout = initialLayout;
		attachment.finalLayout = finalLayout;

		mAttachmentDescriptions.push_back(attachment);

		if (attachmentRef.layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL  && isResolveAttachment == false)
		{
			mColorReference.push_back(attachmentRef);
		}
		else if(attachmentRef.layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && isResolveAttachment == true)
		{
			mResolveReference = attachmentRef;
			mHasResolveReference = true;

		}
		else if (attachmentRef.layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			mDepthReference = attachmentRef;
			mHasDepthReference = true;
		}
	}

	void VulkanBaseSubPass::DestroyBaseSubPass()
	{
		mColorReference.clear();
		mAttachmentDescriptions.clear();
	}

	VkAttachmentDescription VulkanBaseSubPass::GetAttachmentDescription(uint32_t index)
	{
		return mAttachmentDescriptions[index];
	}

	void VulkanBaseRenderPass::CreateRenderPass(const SVulkanContext &vulkanContext)
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

		if (vkCreateRenderPass(vulkanContext.logicalDevice, &renderPassInfo, nullptr, &mRenderPass) != VK_SUCCESS) {
			throw InvisionBaseRendererException("failed to create render pass!");
		}
	}

	void VulkanBaseRenderPass::AddSubpass(VulkanBaseSubPass& subPass)
	{
		VkSubpassDescription subpassDesc = {};
		//VkSubpassDescription subpass = {};
		subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDesc.colorAttachmentCount = static_cast<unsigned int>(subPass.mColorReference.size());
		subpassDesc.pColorAttachments = subPass.mColorReference.data();

		if (subPass.mHasDepthReference)
		{
			subpassDesc.pDepthStencilAttachment = &subPass.mDepthReference;
		}

		if (subPass.mHasResolveReference)
		{
			subpassDesc.pResolveAttachments = &subPass.mResolveReference;
		}

		mSubpasses.push_back(subpassDesc);
		mAttachmentDescriptions.insert(std::end(mAttachmentDescriptions), std::begin(subPass.mAttachmentDescriptions), std::end(subPass.mAttachmentDescriptions));   //push_back(subPass.mAttachmentDescriptions);
	}

	void VulkanBaseRenderPass::AddSubpassDependency(const SVulkanBase &vulkanInstance, uint32_t srcSubpass, uint32_t dstSubpass, VkPipelineStageFlags srcStageFlags, VkAccessFlags srcAccessFlags, VkPipelineStageFlags dstStageFlags, VkAccessFlags dstAccessFlags)
	{
		VkSubpassDependency dependency = {};
		dependency.srcSubpass = srcSubpass;// VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = dstSubpass;// 0
		dependency.srcStageMask = srcStageFlags;
		dependency.srcAccessMask = srcAccessFlags;
		dependency.dstStageMask = dstStageFlags;
		dependency.dstAccessMask = dstAccessFlags;

		mDependencies.push_back(dependency);
	}

	void VulkanBaseRenderPass::AddSubpassDependency(const SVulkanBase &vulkanInstance, uint32_t srcSubpass, uint32_t dstSubpass, VkPipelineStageFlags srcStageFlags, VkAccessFlags srcAccessFlags, VkPipelineStageFlags dstStageFlags, VkAccessFlags dstAccessFlags, VkDependencyFlags dependencyFlags)
	{
		VkSubpassDependency dependency = {};
		dependency.srcSubpass = srcSubpass;// VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = dstSubpass;// 0
		dependency.srcStageMask = srcStageFlags;
		dependency.srcAccessMask = srcAccessFlags;
		dependency.dstStageMask = dstStageFlags;
		dependency.dstAccessMask = dstAccessFlags;
		dependency.dependencyFlags = dependencyFlags;

		mDependencies.push_back(dependency);
	}

	VkRenderPass VulkanBaseRenderPass::GetRenderPass()
	{
		return mRenderPass;
	}

	void VulkanBaseRenderPass::DestroyRenderPass(const SVulkanContext &vulkanContext)
	{
		if (mRenderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(vulkanContext.logicalDevice, mRenderPass, nullptr);
		}
		mAttachmentDescriptions.clear();

		mSubpasses.clear();
		mDependencies.clear();
	}

}