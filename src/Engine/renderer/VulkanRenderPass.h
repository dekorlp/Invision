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
	enum AttachmentType;

	class VulkanRenderPass : public IRenderPass
	{
	public:

		INVISION_API VulkanRenderPass() = delete;

		INVISION_API VulkanRenderPass(VulkanInstance* instance);

		INVISION_API VulkanBaseRenderPass GetRenderPass();

		INVISION_API void CreateMainRenderPass(VulkanBaseTexture& depthTexture, VulkanBaseTexture& colorTexture); // without MSAA
		INVISION_API void CreateMainRenderPass(VulkanBaseTexture& depthTexture); // with MSAA
		INVISION_API void AddAttachment(AttachmentType attachmentType, std::shared_ptr<ITexture>& attachment) override;
		INVISION_API std::vector< Invision::VulkanBaseTexture*> GetAttachmentTextures();
		INVISION_API ~VulkanRenderPass();

	private:

		bool mIsMainRenderpass = false; // needed for clearing mAttachmentRefTextures
		unsigned int mAttachmentIndex = 0;
		Invision::VulkanBaseRenderPass mRenderPass;
		std::vector<VulkanBaseSubPass> mSubPass;
		std::vector<Invision::VulkanBaseTexture*> mAttachmentRefTextures;
		Invision::VulkanInstance *mVulkanInstance;

	};

}

#endif // VULKAN_RENDER_PASS_H