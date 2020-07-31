#ifndef VULKAN_BASE_RENDERPASS_H
#define VULKAN_BASE_RENDERPASS_H

#include "vulkan\vulkan.h"

namespace Invision
{
	struct SVulkanBase;
	struct SVulkanContext;

	// TODO: Convert this later to a class
	struct BaseSubPass
	{
		std::vector<VkAttachmentReference> mColorReference;
		std::vector<VkAttachmentReference> mDepthReference; // actual unused
	};


	class  VulkanBaseRenderPass {
	public:
		void INVISION_API CreateRenderPass(const SVulkanBase &vulkanInstance);
		void INVISION_API DestroyRenderPass(const SVulkanBase &vulkanInstance);
		void INVISION_API AddAttachment(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext);
		void INVISION_API AddSubpass(BaseSubPass subPass = {});
		void INVISION_API AddSubpassDependency(const SVulkanBase &vulkanInstance);
		VkRenderPass INVISION_API GetRenderPass();
	private:
		std::vector<VkAttachmentDescription> mAttachmentDescriptions;
	
		// A Render Pass Consists of multiple Subpasses  and a Subpass consists of multiple ColorReference, Depth References etc..
		// example: mSubpassesReferences[mSubpasses.Index].mColorReference
		std::vector<VkSubpassDescription> mSubpasses;
		std::vector<BaseSubPass> mSubpassesReferences;

		VkRenderPass mRenderPass;
		std::vector<VkSubpassDependency> mDependencies;
	};

}

#endif // VULKAN_BASE_RENDERPASS_H