#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"
#include "VulkanShader.h"


namespace Invision
{
	class INVISION_API VulkanPipeline
	{
	public:

	private:
		VkPipeline mPipeline; // basic pipeline
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages; // shaders
		
	};

}


#endif // VULKAN_PIPELINE_H