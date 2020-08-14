#ifndef VULKAN_FRAMEBUFFER_H
#define VULKAN_FRAMEBUFFER_H


#include "IFramebuffer.h"
#include "renderer\Vulkan\VulkanBaseFramebuffer.h"

namespace Invision
{
	class VulkanInstance;
	class VulkanFramebuffer : public IFramebuffer
	{
	public:

		INVISION_API VulkanFramebuffer() = delete;

		INVISION_API VulkanFramebuffer(VulkanInstance* instance, std::shared_ptr<Invision::IRenderPass> renderPass);

		INVISION_API VulkanBaseFramebuffer GetFramebuffer();


		INVISION_API ~VulkanFramebuffer();
	private:
		Invision::VulkanInstance *vulkanInstance;

		Invision::VulkanBaseFramebuffer framebuffer;

	};

}

#endif // VULKAN_FRAMEBUFFER_H