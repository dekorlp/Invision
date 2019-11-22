#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"
#include "VulkanShader.h"


namespace Invision
{
	class  VulkanPipeline
	{
	public:
		void INVISION_API AddShader(VulkanShader shader);
	private:
		VkPipeline mPipeline; // basic pipeline
		std::vector<VkPipelineShaderStageCreateInfo> mShaderStages; // shaders
		
	};

}


#endif // VULKAN_PIPELINE_H