#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanUniformBuffer.h"
#include "VulkanVertexBuffer.h"
#include "VulkanPushConstant.h"
#include "VulkanRenderPass.h"

#include "VulkanPipeline.h"
namespace Invision
{
	VulkanPipeline::VulkanPipeline(VulkanInstance* instance) :
		IPipeline(instance)
	{
		vulkanInstance = instance;
		//mPipelineProperties ;
		mPipelineProperties = std::make_shared<PipelineProperties>();		
	}

	VulkanPipeline::VulkanPipeline(VulkanInstance* instance, PipelineProperties* pipelineProperties) :
		IPipeline(instance, pipelineProperties)
	{
		vulkanInstance = instance;
		mPipelineProperties = std::make_shared<PipelineProperties>(*pipelineProperties);
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
		case SHADER_STAGE_TESSELLATION_CONTROL_BIT:
			vkShaderStage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			break;
		case SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
			vkShaderStage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			break;
		default:
			throw InvisionBaseRendererException("Unknown ShaderStageFlag passed to Function CreateUniformBinding");

		}

		shaders.push_back(VulkanBaseShader(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), code, vkShaderStage));
	}

	void VulkanPipeline::ClearUniformBuffer()
	{
		pipeline.ClearUniformsBuffer();
	}

	void VulkanPipeline::AddUniformBuffer(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer)
	{
		pipeline.AddUniformBuffer(dynamic_pointer_cast<VulkanUniformBuffer>(uniformBuffer)->GetBuffer());
	}

	void VulkanPipeline::AddUniformBuffer(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer, uint32_t set)
	{
		pipeline.AddUniformBuffer(dynamic_pointer_cast<VulkanUniformBuffer>(uniformBuffer)->GetBuffer(), set);
	}


	void VulkanPipeline::AddVertexBuffer(std::shared_ptr<Invision::IVertexBuffer> vertexBuffer)
	{
		pipeline.AddVertexBuffer(dynamic_pointer_cast<VulkanVertexBuffer>(vertexBuffer)->GetBaseVertexBuffer());
	}

	void VulkanPipeline::BindPushConstant(std::shared_ptr <Invision::IPushConstant> pushConstant)
	{
		pipeline.BindPushConstant(dynamic_pointer_cast<VulkanPushConstant>(pushConstant)->GetBasePushConstant());
	}

	void VulkanPipeline::CreatePipeline(std::shared_ptr<Invision::IRenderPass> renderPass)
	{
		// translate Render PipelineProperties
		VkPrimitiveTopology vkPrimitiveTopology;
		VkPolygonMode vkPolygonMode;
		VkCullModeFlags vkCullMode = 0;
		VkFrontFace vkFrontface;
		

		switch (mPipelineProperties->mPrimitiveTopology)
		{
		case PRIMITIVE_TOPOLOGY_POINT_LIST:
			vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			break;
		case PRIMITIVE_TOPOLOGY_LINE_LIST:
			vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			break;
		case PRIMITIVE_TOPOLOGY_LINE_STRIP:
			vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			break;
		case PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:
			vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			break;
		case PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP:
			vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			break;
		case PRIMITIVE_TOPOLOGY_TRIANGLE_FAN:
			vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
			break;
		case PRIMITIVE_TOPOLOGY_LINE_LIST_ADJ:
			vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
			break;
		case PRIMITIVE_TOPOLOGY_LINE_STRIP_ADJ:
			vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
			break;
		case PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_ADJ:
			vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
			break;
		case PRIMTIVIE_TOPOLOGY_TRIANGLE_STRIP_ADJ:
			vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
			break;

		default:
			throw InvisionBaseRendererException("Unknown Primitive Topology passed to Pipeline");
		}

		switch (mPipelineProperties->mPolygonMode)
		{
		case POLYGON_MODE_FILL:
			vkPolygonMode = VK_POLYGON_MODE_FILL;
			break;
		case POLYGON_MODE_LINE:
			vkPolygonMode = VK_POLYGON_MODE_LINE;
			break;
		case POLYGON_MODE_POINT:
			vkPolygonMode = VK_POLYGON_MODE_POINT;
			break;
		default:
			throw InvisionBaseRendererException("Unknown Polygon Mode passed to Pipeline");
		}

		if (mPipelineProperties->mCullMode & CULL_MODE_NONE)
		{
			vkCullMode = vkCullMode | VK_CULL_MODE_NONE;
		}

		if (mPipelineProperties->mCullMode & CULL_MODE_FRONT_BIT)
		{
			vkCullMode = vkCullMode | VK_CULL_MODE_FRONT_BIT;
		}

		if (mPipelineProperties->mCullMode & CULL_MODE_BACK_BIT)
		{
			vkCullMode = vkCullMode | VK_CULL_MODE_BACK_BIT;
		}

		if (mPipelineProperties->mCullMode & CULL_MODE_FRONT_AND_BACK)
		{
			vkCullMode = vkCullMode | VK_CULL_MODE_FRONT_AND_BACK;
		}

		switch (mPipelineProperties->mFrontFaceMode)
		{
		case FRONT_FACE_COUNTER_CLOCKWISE:
			vkFrontface = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			break;
		case FRONT_FACE_CLOCKWISE:
			vkFrontface = VK_FRONT_FACE_CLOCKWISE;
			break;
		default:
			throw InvisionBaseRendererException("Unknown Front Face Mode passed to Pipeline");
		}

		///////////////////////////////////////////////////

		for (int i = 0; i < shaders.size(); i++)
		{
			pipeline.AddShader(shaders[i]);
		}

		pipeline.SetRenderProperties(vkPrimitiveTopology, vkPolygonMode, vkCullMode, vkFrontface, mPipelineProperties->mLineWidth);
		pipeline.CreatePipeline(vulkanInstance->GetCoreEngine()->GetVulkanInstance(), dynamic_pointer_cast<VulkanRenderPass>(renderPass)->GetRenderPass(), 0,vulkanInstance->GetDepthRessources().AreDepthRessourcesActivated());
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