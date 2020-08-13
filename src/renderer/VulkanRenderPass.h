#ifndef VULKAN_RENDER_PASS_H
#define VULKAN_RENDER_PASS_H


#include "IRenderPass.h"
#include "renderer\Vulkan\VulkanBaseRenderPass.h"

namespace Invision
{
	//class VulkanEngine;
	class VulkanInstance;

	class VulkanRenderPass : public IRenderPass
	{
	public:

		INVISION_API VulkanRenderPass() = delete;

		//INVISION_API VulkanRenderPass(VulkanEngine* engine);
		INVISION_API VulkanRenderPass(VulkanInstance* instance);

		INVISION_API VulkanBaseRenderPass GetRenderPass();

		INVISION_API ~VulkanRenderPass();

	private:
		Invision::VulkanBaseRenderPass renderPass;
		Invision::VulkanInstance *vulkanInstance;
		//Invision::VulkanEngine *vulkanEngine;

	};

}

#endif // VULKAN_RENDER_PASS_H