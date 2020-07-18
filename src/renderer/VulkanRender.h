#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H


#include "IRenderer.h"
#include "renderer\Vulkan\VulkanBaseRenderer.h"

namespace Invision
{
	class VulkanEngine;
	class ICommandBuffer;
	class VulkanRenderer : public IRenderer
	{
	public:

		INVISION_API VulkanRenderer() = delete;

		INVISION_API VulkanRenderer(VulkanEngine* engine);

		INVISION_API bool PrepareFrame() override;
		INVISION_API bool SubmitFrame(std::shared_ptr<ICommandBuffer> commandBuffer) override;

		INVISION_API ~VulkanRenderer();

	private:
		VulkanEngine* vulkanEngine;
		VulkanBaseRenderer renderer;
	};

}

#endif // VULKAN_RENDERER_H