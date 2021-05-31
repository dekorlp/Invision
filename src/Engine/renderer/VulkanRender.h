#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H


#include "IRenderer.h"
#include "renderer\Vulkan\VulkanBaseRenderer.h"

namespace Invision
{
	class VulkanInstance;
	class ICommandBuffer;
	class VulkanRenderer : public IRenderer
	{
	public:

		INVISION_API VulkanRenderer() = delete;

		INVISION_API VulkanRenderer(VulkanInstance* instance);

		INVISION_API bool PrepareFrame() override;
		INVISION_API void Draw(std::shared_ptr<ICommandBuffer> commandBuffer) override;
		INVISION_API bool SubmitFrame() override;

		INVISION_API ~VulkanRenderer();

	private:
		unsigned int mImageIndex = 0;
		Invision::VulkanInstance *mVulkanInstance;
		VkSemaphore mSelectedSemaphore;

		VulkanBaseRenderer mRenderer;
	};

}

#endif // VULKAN_RENDERER_H