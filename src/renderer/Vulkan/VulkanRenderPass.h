#ifndef VULKAN_RENDERPASS_H
#define VULKAN_RENDERPASS_H

#include <iostream>
using namespace std;

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"


namespace Invision
{
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
	private:
		std::vector<VkAttachmentDescription> mAttachmentDescriptions;
	
		std::vector<VkSubpassDescription> mSubpasses;
		std::vector<SubPass> mSubpassesReferences;

		VkRenderPass mRenderPass;
	};

}

#endif // VULKAN_RENDERPASS_H