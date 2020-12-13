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

	VulkanPushConstant::VulkanPushConstant(VulkanInstance* instance, ShaderStage shaderStages, uint32_t offset, uint32_t size) :
		IPushConstant(instance)
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
		default:
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


	void VulkanPushConstant::SetShaderStages(ShaderStage shaderStages)
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