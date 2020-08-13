#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanUniformBuffer.h"
#include "VulkanVertexBuffer.h"
#include "VulkanRenderPass.h"

#include "VulkanPipeline.h"
namespace Invision
{

	VulkanPipeline::VulkanPipeline(VulkanInstance* instance) :
		IPipeline(instance)
	{
		vulkanInstance = instance;
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

		shaders.push_back(VulkanBaseShader(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), code, vkShaderStage));
	}

	void VulkanPipeline::AddUniformBuffer(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer)
	{
		pipeline.AddUniformBuffer(dynamic_pointer_cast<VulkanUniformBuffer>(uniformBuffer)->GetBuffer());
		
	}

	void VulkanPipeline::AddVertexBuffer(std::shared_ptr<Invision::IVertexBuffer> vertexBuffer)
	{
		pipeline.AddVertexBuffer(dynamic_pointer_cast<VulkanVertexBuffer>(vertexBuffer)->GetBuffer());
	}

	void VulkanPipeline::CreatePipeline(std::shared_ptr<Invision::IRenderPass> renderPass)
	{
		for (int i = 0; i < shaders.size(); i++)
		{
			pipeline.AddShader(shaders[i]);
		}

		pipeline.CreatePipeline(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), dynamic_pointer_cast<VulkanRenderPass>(renderPass)->GetRenderPass(), 0);
		for(int i = 0; i < shaders.size(); i++)
		{
			shaders[i].Destroy(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
		}

	}

	VulkanBasePipeline VulkanPipeline::GetPipeline()
	{
		return pipeline;
	}

	VulkanPipeline::~VulkanPipeline()
	{
		pipeline.DestroyPipeline(vulkanInstance->GetCoreEngine()->GetVulkanInstance());
	}

}