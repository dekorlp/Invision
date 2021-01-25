#ifndef VULKAN_BASE_RENDERPASS_H
#define VULKAN_BASE_RENDERPASS_H

#include "vulkan\vulkan.h"

namespace Invision
{
	struct SVulkanBase;
	struct SVulkanContext;

	// TODO: Convert this later to a class
	class BaseSubPass
	{
	public:
		std::vector<VkAttachmentReference> mColorReference;
		VkAttachmentReference mDepthReference;

		bool mHasDepthReference = false;

		std::vector<VkAttachmentDescription> mAttachmentDescriptions;

		void INVISION_API AddAttachment(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext, VkFormat format, VkSampleCountFlagBits numSamples,
			VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreop, VkImageLayout initialLayout, VkImageLayout finalLayout, VkAttachmentReference attachmentRef);
		void INVISION_API DestroyBaseSubPass();
	};


	class  VulkanBaseRenderPass {
	public:
		void INVISION_API CreateRenderPass(const SVulkanBase &vulkanInstance);
		void INVISION_API DestroyRenderPass(const SVulkanBase &vulkanInstance);
		
		void INVISION_API AddSubpass(BaseSubPass& subPass);
		void INVISION_API AddSubpassDependency(const SVulkanBase &vulkanInstance, VkPipelineStageFlags srcStageFlags, VkAccessFlags srcAccessFlags, VkPipelineStageFlags dstStageFlags, VkAccessFlags dstAccessFlags);
		VkRenderPass INVISION_API GetRenderPass();
	private:
	
		// A Render Pass Consists of multiple Subpasses  and a Subpass consists of multiple ColorReference, Depth References etc..
		// example: mSubpassesReferences[mSubpasses.Index].mColorReference
		std::vector<VkSubpassDescription> mSubpasses;
		std::vector<VkAttachmentDescription> mAttachmentDescriptions;

		VkRenderPass mRenderPass;
		std::vector<VkSubpassDependency> mDependencies;
	};

}

#endif // VULKAN_BASE_RENDERPASS_H