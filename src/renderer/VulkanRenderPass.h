#ifndef VULKAN_RENDER_PASS_H
#define VULKAN_RENDER_PASS_H


#include "IRenderPass.h"
namespace Invision
{
	class VulkanEngine;
	class VulkanRenderPass : public IRenderPass
	{
	public:

		INVISION_API VulkanRenderPass() = delete;

		INVISION_API VulkanRenderPass(VulkanEngine* engine);

	};

}

#endif // VULKAN_RENDER_PASS_H