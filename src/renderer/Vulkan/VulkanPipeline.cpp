#include "precompiled.h"

#include "VulkanPipeline.h"

namespace Invision
{
	void VulkanPipeline::AddShader(VulkanShader shader)
	{
		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = shader.GetShaderStageFlatBits();
		shaderStageInfo.module = shader.GetShaderModule();
		shaderStageInfo.pName = "main";

		mShaderStages.push_back(shaderStageInfo);
	}

}