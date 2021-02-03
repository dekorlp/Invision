#ifndef VULKAN_UNIFORM_BUFFER_H
#define VULKAN_UNIFORM_BUFFER_H


#include "IUniformBuffer.h"
#include "renderer\Vulkan\VulkanBaseUniformBuffer.h"
#include "renderer\Vulkan\VulkanBaseTexture.h"

namespace Invision
{
	class VulkanInstance;

	class VulkanUniformBuffer : public IUniformBuffer
	{
	public:

		INVISION_API VulkanUniformBuffer() = delete;

		INVISION_API VulkanUniformBuffer(VulkanInstance* instance);

		INVISION_API VulkanUniformBuffer& CreateUniformBinding(uint32_t set, uint32_t binding, uint32_t descriptorCount, ShaderStageFlag shaderStage, uint64_t bufferSize);

		INVISION_API VulkanUniformBuffer& CreateImageBinding(uint32_t set, uint32_t binding, uint32_t descriptorCount, ShaderStageFlag shaderStage, std::shared_ptr < Invision::ITexture> texture);

		INVISION_API void CreateUniformBuffer();

		INVISION_API void UpdateUniform(const void* source, size_t size, uint32_t set, uint32_t binding) override;

		INVISION_API VulkanBaseUniformBuffer GetBuffer();

		INVISION_API ~VulkanUniformBuffer();

	private:
		Invision::VulkanInstance *vulkanInstance;

		Invision::VulkanBaseUniformBuffer uniformBuffer;
	};

}

#endif // VULKAN_UNIFORM_BUFFER_H