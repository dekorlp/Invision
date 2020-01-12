#ifndef VULKAN_RENDERPASS_H
#define VULKAN_RENDERPASS_H

#include "vulkan\vulkan.h"

namespace Invision
{
	struct SVulkan;

	// TODO: Convert this later to a class
	struct SubPass
	{
		std::vector<VkAttachmentReference> mColorReference;
		std::vector<VkAttachmentReference> mDepthReference; // actual unused
	};


	class  VulkanRenderPass {
	public:
		void INVISION_API CreateRenderPass(const SVulkan &vulkanInstance);
		void INVISION_API DestroyRenderPass(const SVulkan &vulkanInstance);
		void INVISION_API AddAttachment(const SVulkan &vulkanInstance);
		void INVISION_API AddSubpass(SubPass subPass = {});
		void INVISION_API AddSubpassDependency(const SVulkan &vulkanInstance);
		VkRenderPass INVISION_API GetRenderPass();
	private:
		std::vector<VkAttachmentDescription> mAttachmentDescriptions;
	
		// A Render Pass Consists of multiple Subpasses  and a Subpass consists of multiple ColorReference, Depth References etc..
		// example: mSubpassesReferences[mSubpasses.Index].mColorReference
		std::vector<VkSubpassDescription> mSubpasses;
		std::vector<SubPass> mSubpassesReferences;

		VkRenderPass mRenderPass;
		std::vector<VkSubpassDependency> mDependencies;
	};

}

#endif // VULKAN_RENDERPASS_H