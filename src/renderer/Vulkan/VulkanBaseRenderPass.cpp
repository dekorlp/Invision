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

	void VulkanBaseRenderPass::AddAttachment(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext)
	{
		VkAttachmentDescription attachment = {};
		attachment.format = vulkanContext.swapChainImageFormat;
		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		mAttachmentDescriptions.push_back(attachment);
	}
	void VulkanBaseRenderPass::AddSubpass(BaseSubPass subPass)
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
		subpassDesc.colorAttachmentCount = static_cast<unsigned int>(mSubpassesReferences.at(static_cast<unsigned int>(mSubpassesReferences.size()) - 1).mColorReference.size());
		subpassDesc.pColorAttachments = mSubpassesReferences.at(static_cast<unsigned int>(mSubpassesReferences.size()) - 1).mColorReference.data();
		mSubpasses.push_back(subpassDesc);
	}

	void VulkanBaseRenderPass::AddSubpassDependency(const SVulkanBase &vulkanInstance)
	{
		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

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