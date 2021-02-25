#ifndef VULKAN_FRAMEBUFFER_H
#define VULKAN_FRAMEBUFFER_H


#include "IFramebuffer.h"
#include "renderer\Vulkan\VulkanBaseFramebuffer.h"

namespace Invision
{
	class VulkanInstance;
	class VulkanRenderPass;

	class VulkanFramebuffer : public IFramebuffer
	{
	public:

		INVISION_API VulkanFramebuffer() = delete;

		INVISION_API VulkanFramebuffer(VulkanInstance* instance, std::shared_ptr<Invision::IRenderPass> renderPass);

		INVISION_API VulkanFramebuffer(VulkanInstance* instance, std::shared_ptr<Invision::IRenderPass> renderPass, bool isMainFrameBuffer);

		INVISION_API void CreateMainFramebuffer(std::shared_ptr<Invision::IRenderPass> renderPass);

		INVISION_API VulkanBaseFramebuffer GetFramebuffer(unsigned int index = 0);

		INVISION_API std::vector < VulkanBaseFramebuffer> GetFramebuffers();


		INVISION_API ~VulkanFramebuffer();
	private:
		Invision::VulkanInstance *mVulkanInstance;

		std::vector<Invision::VulkanBaseFramebuffer> mFramebuffers;

	};

}

#endif // VULKAN_FRAMEBUFFER_H