#ifndef VULKAN_BASE_RENDERPASS_H
#define VULKAN_BASE_RENDERPASS_H

#include "vulkan\vulkan.h"

namespace Invision
{
	struct SVulkanBase;
	struct SVulkanContext;

	class VulkanBaseSubPass
	{
	public:
		std::vector<VkAttachmentReference> mColorReference;
		VkAttachmentReference mDepthReference;
		VkAttachmentReference mResolveReference;

		bool mHasDepthReference = false;
		bool mHasResolveReference = false;

		std::vector<VkAttachmentDescription> mAttachmentDescriptions;

		bool mIsMainSubPass = false;

		void INVISION_API AddAttachment(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext, VkFormat format, VkSampleCountFlagBits numSamples,
			VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreop, 
			VkImageLayout initialLayout, VkImageLayout finalLayout, VkAttachmentReference attachmentRef, bool isResolveAttachment = false); 

		VkAttachmentDescription GetAttachmentDescription(uint32_t index);

		void INVISION_API DestroyBaseSubPass();
	};


	class  VulkanBaseRenderPass {
	public:
		void INVISION_API CreateRenderPass(const SVulkanBase &vulkanInstance);
		void INVISION_API DestroyRenderPass(const SVulkanBase &vulkanInstance);
		
		void INVISION_API AddSubpass(VulkanBaseSubPass& subPass);
		void INVISION_API AddSubpassDependency(const SVulkanBase &vulkanInstance, uint32_t srcSubpass, uint32_t dstSubpass, VkPipelineStageFlags srcStageFlags, VkAccessFlags srcAccessFlags, VkPipelineStageFlags dstStageFlags, VkAccessFlags dstAccessFlags);
		void INVISION_API AddSubpassDependency(const SVulkanBase &vulkanInstance, uint32_t srcSubpass, uint32_t dstSubpass, VkPipelineStageFlags srcStageFlags, VkAccessFlags srcAccessFlags, VkPipelineStageFlags dstStageFlags, VkAccessFlags dstAccessFlags, VkDependencyFlags dependencyFlags);
		VkRenderPass INVISION_API GetRenderPass();
	private:
	
		// A Render Pass Consists of multiple Subpasses  and a Subpass consists of multiple ColorReference, Depth References etc..
		// example: mSubpassesReferences[mSubpasses.Index].mColorReference
		std::vector<VkSubpassDescription> mSubpasses;
		std::vector<VkAttachmentDescription> mAttachmentDescriptions;
		VkRenderPass mRenderPass = VK_NULL_HANDLE;
		std::vector<VkSubpassDependency> mDependencies;
	};

}

#endif // VULKAN_BASE_RENDERPASS_H