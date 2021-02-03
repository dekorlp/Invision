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
		mVulkanInstance = instance;
		//mPipelineProperties ;
		mPipelineProperties = std::make_shared<PipelineProperties>();		
	}

	VulkanPipeline::VulkanPipeline(VulkanInstance* instance, PipelineProperties* pipelineProperties) :
		IPipeline(instance, pipelineProperties)
	{
		mVulkanInstance = instance;
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

		mShaders.push_back(VulkanBaseShader(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), code, vkShaderStage));
	}

	void VulkanPipeline::ClearUniformBuffer()
	{
		mPipeline.ClearUniformsBuffer();
	}

	void VulkanPipeline::AddUniformBuffer(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer)
	{
		mPipeline.AddUniformBuffer(dynamic_pointer_cast<VulkanUniformBuffer>(uniformBuffer)->GetBuffer());
	}

	void VulkanPipeline::AddUniformBuffer(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer, uint32_t set)
	{
		mPipeline.AddUniformBuffer(dynamic_pointer_cast<VulkanUniformBuffer>(uniformBuffer)->GetBuffer(), set);
	}


	void VulkanPipeline::AddVertexBuffer(std::shared_ptr<Invision::IVertexBuffer> vertexBuffer)
	{
		mPipeline.AddVertexBuffer(dynamic_pointer_cast<VulkanVertexBuffer>(vertexBuffer)->GetBaseVertexBuffer());
	}

	void VulkanPipeline::BindPushConstant(std::shared_ptr <Invision::IPushConstant> pushConstant)
	{
		mPipeline.BindPushConstant(dynamic_pointer_cast<VulkanPushConstant>(pushConstant)->GetBasePushConstant());
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

		for (int i = 0; i < mShaders.size(); i++)
		{
			mPipeline.AddShader(mShaders[i]);
		}
		
		mPipeline.SetRenderProperties(vkPrimitiveTopology, vkPolygonMode, vkCullMode, vkFrontface, mPipelineProperties->mLineWidth);
		mPipeline.CreatePipeline(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), dynamic_pointer_cast<VulkanRenderPass>(renderPass)->GetRenderPass(), 0,mVulkanInstance->GetDepthRessources().AreDepthRessourcesActivated(), mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct().MsaaFlagBits);
		for(int i = 0; i < mShaders.size(); i++)
		{
			mShaders[i].Destroy(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct());
		}

	}

	VulkanBasePipeline VulkanPipeline::GetPipeline()
	{
		return mPipeline;
	}

	VulkanPipeline::~VulkanPipeline()
	{
		mPipeline.DestroyPipeline(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct());
	}

}