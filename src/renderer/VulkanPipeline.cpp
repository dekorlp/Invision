#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanUniformBuffer.h"

#include "VulkanPipeline.h"
namespace Invision
{

	VulkanPipeline::VulkanPipeline(VulkanEngine* engine) :
		IPipeline(engine)
	{
		vulkanEngine = engine;
	}



	void VulkanPipeline::AddShader(const std::vector<char>& code, ShaderStage stage)
	{
		VkShaderStageFlagBits vkShaderStage;


		switch (stage)
		{
		case SHADER_STAGE_VERTEX_BIT:
			vkShaderStage = VK_SHADER_STAGE_VERTEX_BIT;
			break;
		case SHADER_STAGE_GEOMETRY_BIT:
			vkShaderStage = VK_SHADER_STAGE_GEOMETRY_BIT;
			break;
		case SHADER_STAGE_FRAGMENT_BIT:
			vkShaderStage = VK_SHADER_STAGE_FRAGMENT_BIT;
			break;
		case SHADER_STAGE_COMPUTE_BIT:
			vkShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
			break;
		default:
			throw InvisionBaseRendererException("Unknown ShaderStageFlag passed to Function CreateUniformBinding");

		}

		shaders.push_back(VulkanBaseShader(vulkanEngine->GetVulkanInstance(), code, vkShaderStage));

	}

	void VulkanPipeline::AddUniformBuffer(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer)
	{
		pipeline.AddUniformBuffer(dynamic_pointer_cast<VulkanUniformBuffer>(uniformBuffer)->GetBuffer());
		
	}


}