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
		std::shared_ptr<PipelineProperties> pipelineProperties = std::make_shared<PipelineProperties>();

		SetPrimitiveTopology(pipelineProperties->mPrimitiveTopology);
		SetPolygonMode(pipelineProperties->mPolygonMode);
		SetCullMode(pipelineProperties->mCullMode);
		SetFrontFace(pipelineProperties->mFrontFaceMode);
		SetLineWidth(pipelineProperties->mLineWidth);
	}

	VulkanPipeline::VulkanPipeline(VulkanInstance* instance, PipelineProperties* pipelineProperties) :
		IPipeline(instance, pipelineProperties)
	{
		mVulkanInstance = instance;
		std::shared_ptr<PipelineProperties> lPipelineProperties = std::make_shared<PipelineProperties>(*pipelineProperties);

		SetPrimitiveTopology(lPipelineProperties->mPrimitiveTopology);
		SetPolygonMode(lPipelineProperties->mPolygonMode);
		SetCullMode(lPipelineProperties->mCullMode);
		SetFrontFace(lPipelineProperties->mFrontFaceMode);
		SetLineWidth(lPipelineProperties->mLineWidth);
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

	void VulkanPipeline::SetColorBlendFunction(bool blendEnable, BlendFactor srcColorBlendFactor, BlendFactor dstColorBlendFactor, BlendOp colorBlendOp, BlendFactor srcAlphaBlendFactor, BlendFactor dstAlphaBlendFactor, BlendOp alphaBlendOp)
	{
		VkBlendFactor vksrcColorBlendFactor = TranslateBlendFactor(srcColorBlendFactor);
		VkBlendFactor vkdstColorBlendFactor = TranslateBlendFactor(dstColorBlendFactor);
		VkBlendOp vkcolorBlendOp = TranslateBlendOp(colorBlendOp);
		VkBlendFactor vksrcAlphaBlendFactor = TranslateBlendFactor(srcAlphaBlendFactor);
		VkBlendFactor vkdstAlphaBlendFactor = TranslateBlendFactor(dstAlphaBlendFactor);
		VkBlendOp vkalphaBlendOp = TranslateBlendOp(alphaBlendOp);


		mPipeline.SetColorBlendFunction(blendEnable, vksrcColorBlendFactor, vkdstColorBlendFactor, vkcolorBlendOp, vksrcAlphaBlendFactor, vkdstAlphaBlendFactor, vkalphaBlendOp);
	}

	void VulkanPipeline::SetDepthTest(bool enable, CompareOp compareOp)
	{
		mPipeline.SetDepthTest(enable, TranslateCompareOp(compareOp));
	}

	void VulkanPipeline::SetDepthWrite(bool enable)
	{
		mPipeline.SetDepthWrite(enable);
	}

	void VulkanPipeline::SetPrimitiveTopology(PrimitiveTopology primitiveTopology)
	{
		VkPrimitiveTopology vkPrimitiveTopology;

		switch (primitiveTopology)
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
			if (primitiveTopology >= 101 && primitiveTopology <= 132)
			{
				mPipeline.SetTesselationPatchControlPoints(primitiveTopology - 100);
				vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
			}
			else
			{
				throw InvisionBaseRendererException("Unknown Primitive Topology passed to Pipeline");
			}
		}

		mPipeline.SetRenderPropertyPrimitiveTopology(vkPrimitiveTopology);
	}

	void VulkanPipeline::SetPolygonMode(PolygonMode polygonMode)
	{
		VkPolygonMode vkPolygonMode;

		switch (polygonMode)
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

		mPipeline.SetRenderPropertyPolygonMode(vkPolygonMode);

	}

	void VulkanPipeline::SetCullMode(CullModeBits cullModeFlags)
	{
		VkCullModeFlags vkCullMode = 0;

		if (cullModeFlags & CULL_MODE_NONE)
		{
			vkCullMode = vkCullMode | VK_CULL_MODE_NONE;
		}

		if (cullModeFlags & CULL_MODE_FRONT_BIT)
		{
			vkCullMode = vkCullMode | VK_CULL_MODE_FRONT_BIT;
		}

		if (cullModeFlags & CULL_MODE_BACK_BIT)
		{
			vkCullMode = vkCullMode | VK_CULL_MODE_BACK_BIT;
		}

		if (cullModeFlags & CULL_MODE_FRONT_AND_BACK)
		{
			vkCullMode = vkCullMode | VK_CULL_MODE_FRONT_AND_BACK;
		}

		mPipeline.SetRenderPropertyCullMode(vkCullMode);
	}

	void VulkanPipeline::SetFrontFace(FrontFaceMode frontFace)
	{
		VkFrontFace vkFrontface;
		switch (frontFace)
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

		mPipeline.SetRenderPropertyFrontFace(vkFrontface);
	}

	void VulkanPipeline::SetLineWidth(float lineWidth)
	{
		mPipeline.SetRenderPropertyLineWidth(lineWidth);
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


	void VulkanPipeline::AddVertexDescription(std::shared_ptr<Invision::IVertexBindingDescription> vertexBindingDescription)
	{
		
		mPipeline.AddVertexDescription(dynamic_pointer_cast<VulkanVertexBindingDescription>(vertexBindingDescription)->GetBaseDescription());
	}

	void VulkanPipeline::BindPushConstant(std::shared_ptr <Invision::IPushConstant> pushConstant)
	{
		mPipeline.BindPushConstant(dynamic_pointer_cast<VulkanPushConstant>(pushConstant)->GetBasePushConstant());
	}

	void VulkanPipeline::CreatePipeline(std::shared_ptr<Invision::IRenderPass> renderPass)
	{
		for (int i = 0; i < mShaders.size(); i++)
		{
			mPipeline.AddShader(mShaders[i]);
		}
		
		// Multisampling is only used for Display Output, Secondary CommandBuffers does not uses Multisampling
		VkSampleCountFlagBits usedMultisampleState;
		if (renderPass->IsMainRenderPass())
		{
			usedMultisampleState = mVulkanInstance->GetVulkanContext().MsaaFlagBits;
		}
		else
		{
			usedMultisampleState = VK_SAMPLE_COUNT_1_BIT;
		}


		
		mPipeline.CreatePipeline(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct(), mVulkanInstance->GetVulkanContext(),  dynamic_pointer_cast<VulkanRenderPass>(renderPass)->GetRenderPass(), 0, renderPass->GetCountOfColorAttachments(), usedMultisampleState);
		for(int i = 0; i < mShaders.size(); i++)
		{
			mShaders[i].Destroy(mVulkanInstance->GetCoreEngine()->GetVulkanBaseStruct());
		}

	}

	VkBlendFactor VulkanPipeline::TranslateBlendFactor(BlendFactor blendfactor)
	{
		VkBlendFactor factor;

		switch (blendfactor)
		{
		case BLEND_FACTOR_ZERO:
			factor = VK_BLEND_FACTOR_ZERO;
			break;
		case BLEND_FACTOR_ONE:
			factor = VK_BLEND_FACTOR_ONE;
			break;
		case BLEND_FACTOR_SRC_COLOR:
			factor = VK_BLEND_FACTOR_SRC_COLOR;
			break;
		case BLEND_FACTOR_ONE_MINUS_SRC_COLOR:
			factor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			break;
		case BLEND_FACTOR_DST_COLOR:
			factor = VK_BLEND_FACTOR_DST_COLOR;
			break;
		case BLEND_FACTOR_ONE_MINUS_DST_COLOR:
			factor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			break;
		case BLEND_FACTOR_SRC_ALPHA:
			factor = VK_BLEND_FACTOR_SRC_ALPHA;
			break;
		case BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:
			factor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			break;
		case BLEND_FACTOR_DST_ALPHA:
			factor = VK_BLEND_FACTOR_DST_ALPHA;
			break;
		case BLEND_FACTOR_ONE_MINUS_DST_ALPHA:
			factor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			break;
		case BLEND_FACTOR_SRC_ALPHA_SATURATE:
			factor = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			break;
		case BLEND_FACTOR_SRC1_COLOR:
			factor = VK_BLEND_FACTOR_SRC1_COLOR;
			break;
		case BLEND_FACTOR_ONE_MINUS_SRC1_COLOR:
			factor = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
			break;
		case BLEND_FACTOR_SRC1_ALPHA:
			factor = VK_BLEND_FACTOR_SRC1_ALPHA;
			break;
		case BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA:
			factor = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
			break;
		default:
			throw InvisionBaseRendererException("Unknown BlendFactor passed to Pipeline");
		}

		return factor;
	}

	VkBlendOp VulkanPipeline::TranslateBlendOp(BlendOp blendOp)
	{
		VkBlendOp vkblendOp;
		switch (blendOp)
		{
		case BLEND_OP_ADD:
			vkblendOp = VK_BLEND_OP_ADD;
			break;
		case BLEND_OP_SUBTRACT:
			vkblendOp = VK_BLEND_OP_SUBTRACT;
			break;
		case BLEND_OP_REVERSE_SUBTRACT:
			vkblendOp = VK_BLEND_OP_REVERSE_SUBTRACT;
			break;
		case BLEND_OP_MIN:
			vkblendOp = VK_BLEND_OP_MIN;
			break;
		case BLEND_OP_MAX:
			vkblendOp = VK_BLEND_OP_MAX;
			break;
		default:
			throw InvisionBaseRendererException("Unknown BlendOp passed to Pipeline");
		}

		return vkblendOp;
	}

	VkCompareOp VulkanPipeline::TranslateCompareOp(CompareOp compareOp)
	{
		VkCompareOp vkCompareOp;
		switch (compareOp)
		{
		case COMPARE_OP_NEVER:
			vkCompareOp = VK_COMPARE_OP_NEVER;
			break;
		case COMPARE_OP_LESS:
			vkCompareOp = VK_COMPARE_OP_LESS;
			break;
		case COMPARE_OP_EQUAL:
			vkCompareOp = VK_COMPARE_OP_EQUAL;
			break;
		case COMPARE_OP_LESS_OR_EQUAL:
			vkCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
			break;
		case COMPARE_OP_GREATER:
			vkCompareOp = VK_COMPARE_OP_GREATER;
			break;
		case COMPARE_OP_NOT_EQUAL:
			vkCompareOp = VK_COMPARE_OP_NOT_EQUAL;
			break;
		case COMPARE_OP_GREATER_OR_EQUAL:
			vkCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
			break;
		case COMPARE_OP_ALWAYS:
			vkCompareOp = VK_COMPARE_OP_ALWAYS;
			break;
		default:
			throw InvisionBaseRendererException("Unknown BlendOp passed to Pipeline");
		}

		return vkCompareOp;
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