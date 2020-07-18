#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H


#include "IRenderer.h"
namespace Invision
{
	class VulkanEngine;
	class ICommandBuffer;
	class VulkanRenderer : public IRenderer
	{
	public:

		INVISION_API VulkanRenderer() = delete;

		INVISION_API VulkanRenderer(VulkanEngine* engine);

		INVISION_API void render(std::shared_ptr<ICommandBuffer> commandBuffer) override;
	};

}

#endif // VULKAN_RENDERER_H