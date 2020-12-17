#include "precompiled.h"

#include "VulkanEngine.h"
#include "VulkanInstance.h"

#include "VulkanPushConstant.h"

namespace Invision
{
	VulkanPushConstant::VulkanPushConstant(VulkanInstance* instance) :
		IPushConstant(instance)
	{
		vulkanInstance = instance;
	}

	VulkanPushConstant::VulkanPushConstant(VulkanInstance* instance, ShaderStageFlag shaderStages, uint32_t offset, uint32_t size) :
		IPushConstant(instance)
	{
		VkShaderStageFlags vkShaderStage = 0;


		if ((shaderStages & SHADER_STAGE_VERTEX_BIT) != 0)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_VERTEX_BIT;
		}

		if ((shaderStages & SHADER_STAGE_GEOMETRY_BIT) != 0)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_GEOMETRY_BIT;
		}

		if ((shaderStages & SHADER_STAGE_FRAGMENT_BIT) != 0)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		if (shaderStages & SHADER_STAGE_COMPUTE_BIT)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_COMPUTE_BIT;
		}

		if (shaderStages & SHADER_STAGE_TESSELLATION_CONTROL_BIT)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		}

		if (shaderStages & SHADER_STAGE_TESSELLATION_EVALUATION_BIT)
		{
			vkShaderStage = vkShaderStage | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		}

		if (shaderStages == 0)
		{
			throw InvisionBaseRendererException("Unknown ShaderStageFlag passed to Function CreateUniformBinding");
		}

		mPushConstant = VulkanBasePushConstant(vkShaderStage, offset, size);
	}

	/*ShaderStage VulkanPushConstant::GetShaderStages()
	{

	}*/

	uint32_t VulkanPushConstant::GetOffset()
	{
		return mPushConstant.GetOffset();
	}

	uint32_t VulkanPushConstant::GetSize()
	{
		return mPushConstant.GetSize();
	}


	void VulkanPushConstant::SetShaderStages(ShaderStageFlag shaderStages)
	{
		VkShaderStageFlagBits vkShaderStage;


		switch (shaderStages)
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

		mPushConstant.SetShaderStages(vkShaderStage);
	}

	void VulkanPushConstant::SetOffset(uint32_t offset)
	{
		mPushConstant.SetOffset(offset);
	}

	void VulkanPushConstant::SetSize(uint32_t size)
	{
		mPushConstant.SetSize(size);
	}

	Invision::VulkanBasePushConstant VulkanPushConstant::GetBasePushConstant()
	{
		return mPushConstant;
	}
}