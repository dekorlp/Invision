#ifndef VULKAN_RENDER_PASS_H
#define VULKAN_RENDER_PASS_H


#include "IRenderPass.h"
#include "renderer\Vulkan\VulkanBaseRenderPass.h"
//#include "renderer\Vulkan\VulkanBaseTexture.h"

namespace Invision
{
	class VulkanInstance;
	class VulkanTexture;
	class VulkanBaseTexture;

	class VulkanRenderPass : public IRenderPass
	{
	public:

		INVISION_API VulkanRenderPass() = delete;

		INVISION_API VulkanRenderPass(VulkanInstance* instance);

		INVISION_API VulkanBaseRenderPass GetRenderPass();

		INVISION_API void CreateMainRenderPass(VulkanBaseTexture& depthTexture, VulkanBaseTexture& colorTexture); // without MSAA
		INVISION_API void CreateMainRenderPass(VulkanBaseTexture& depthTexture); // with MSAA
		INVISION_API std::vector<Invision::VulkanBaseTexture*> GetAttachmentTextures();
		INVISION_API ~VulkanRenderPass();

	private:
		unsigned int mSubPassIndex = 0;
		Invision::VulkanBaseRenderPass mRenderPass;
		std::vector<Invision::VulkanBaseTexture*> mAttachmentTextures;
		Invision::VulkanInstance *mVulkanInstance;

	};

}

#endif // VULKAN_RENDER_PASS_H