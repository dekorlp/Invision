#ifndef VULKAN_UNIFORM_BUFFER_H
#define VULKAN_UNIFORM_BUFFER_H


#include "IUniformBuffer.h"

namespace Invision
{
	class VulkanEngine;
	class VulkanUniformBuffer : public IUniformBuffer
	{
	public:

		INVISION_API VulkanUniformBuffer() = delete;

		INVISION_API VulkanUniformBuffer(VulkanEngine* engine);

		INVISION_API ~VulkanUniformBuffer();

	private:
		Invision::VulkanEngine *vulkanEngine;

	};

}

#endif // VULKAN_UNIFORM_BUFFER_H