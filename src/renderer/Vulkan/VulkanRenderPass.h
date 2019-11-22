#ifndef VULKAN_RENDERPASS_H
#define VULKAN_RENDERPASS_H

#include <iostream>
using namespace std;

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"


namespace Invision
{

	class  VulkanRenderPass {
	public:
		void INVISION_API CreateRenderPass(const SVulkan &vulkanInstance);
		void INVISION_API DestroyRenderPass(const SVulkan &vulkanInstance);
	private:
		std::vector<VkAttachmentDescription> mAttachmentDescriptions;
		std::vector<VkSubpassDescription> mSubpassDescriptions;
		VkRenderPass mRenderPass;
	};

}

#endif // VULKAN_RENDERPASS_H