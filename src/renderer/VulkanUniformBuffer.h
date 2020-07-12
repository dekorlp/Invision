#ifndef VULKAN_UNIFORM_BUFFER_H
#define VULKAN_UNIFORM_BUFFER_H


#include "IUniformBuffer.h"
#include "renderer\Vulkan\VulkanBaseUniformBuffer.h"

namespace Invision
{
	class VulkanEngine;
	class VulkanUniformBuffer : public IUniformBuffer
	{
	public:

		INVISION_API VulkanUniformBuffer() = delete;

		INVISION_API VulkanUniformBuffer(VulkanEngine* engine);

		INVISION_API VulkanUniformBuffer& CreateUniformBinding(uint32_t binding, uint32_t descriptorCount, ShaderStage shaderStage, uint64_t bufferSize, uint64_t offset);

		INVISION_API void CreateUniformBuffer();

		INVISION_API VulkanBaseUniformBuffer GetBuffer();

		INVISION_API ~VulkanUniformBuffer();

	private:
		Invision::VulkanEngine *vulkanEngine;
		Invision::VulkanBaseUniformBuffer uniformBuffer;
	};

}

#endif // VULKAN_UNIFORM_BUFFER_H