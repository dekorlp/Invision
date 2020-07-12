#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H


#include "IPipeline.h"
#include "renderer\Vulkan\VulkanBaseShader.h"
#include "renderer\Vulkan\VulkanBasePipeline.h"

namespace Invision
{
	class VulkanEngine;
	class VulkanUniformBuffer;

	class VulkanPipeline : public IPipeline
	{
	

	public:

		INVISION_API VulkanPipeline() = delete;

		INVISION_API VulkanPipeline(VulkanEngine* engine);

		INVISION_API void AddShader(const std::vector<char>& code, ShaderStage stage);

		INVISION_API void AddUniformBuffer(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer);


	private:
		Invision::VulkanEngine *vulkanEngine;
		std::vector< Invision::VulkanBaseShader> shaders;
		Invision::VulkanBasePipeline pipeline;

	};

}

#endif // VULKAN_PIPELINE_H