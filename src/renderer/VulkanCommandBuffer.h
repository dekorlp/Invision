#ifndef VULKAN_COMMAND_BUFFER_H
#define VULKAN_COMMAND_BUFFER_H


#include "ICommandBuffer.h"
#include "renderer\Vulkan\VulkanBaseCommandBuffer.h"
namespace Invision
{
	class VulkanEngine;
	class VulkanCommandBuffer : public ICommandBuffer
	{
	public:

		INVISION_API VulkanCommandBuffer() = delete;

		INVISION_API VulkanCommandBuffer(VulkanEngine* engine, std::shared_ptr<Invision::IFramebuffer> framebuffer);

	private:
		Invision::VulkanEngine *vulkanEngine;
		Invision::VulkanBaseCommandBuffer commandBuffer;

	};

}

#endif // VULKAN_COMMAND_BUFFER_H