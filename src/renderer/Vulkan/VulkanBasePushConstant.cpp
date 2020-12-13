#include "precompiled.h"

#include "VulkanBasePushConstant.h"
#include "VulkanBaseException.h"
namespace Invision
{
	VulkanBasePushConstant::VulkanBasePushConstant()
	{

	}

	VulkanBasePushConstant::VulkanBasePushConstant(VkShaderStageFlagBits shaderStages, uint32_t offset, uint32_t size, const void* values)
	{
		if (size > INVISION_GPU_SPEC_PUSH_COMMAND_SIZE)
		{
			throw VulkanBaseException("size is larger than push command specification. (Allowed max: 128 bytes)");
		}

		this->mShaderStage = shaderStages;
		this->mOffset = offset;
		this->mSize = size;
		this->mValues = (void*)values;
	}


	VkPushConstantRange VulkanBasePushConstant::ConstructPushConstantRange()
	{
		VkPushConstantRange range;

		range.stageFlags = mShaderStage;
		range.offset = mOffset;
		range.size = mSize;

		return range;
	}

	VkShaderStageFlagBits VulkanBasePushConstant::GetShaderStages()
	{
		return mShaderStage;
	}

	uint32_t VulkanBasePushConstant::GetOffset()
	{
		return mOffset;
	}

	uint32_t VulkanBasePushConstant::GetSize()
	{
		return mSize;
	}

	const void* VulkanBasePushConstant::GetValues()
	{
		return mValues;
	}

	void VulkanBasePushConstant::SetShaderStages(VkShaderStageFlagBits shaderStages)
	{
		this->mShaderStage = shaderStages;
	}

	void VulkanBasePushConstant::SetOffset(uint32_t offset)
	{
		this->mOffset = offset;
	}

	void VulkanBasePushConstant::SetSize(uint32_t size)
	{
		this->mSize = size;
	}

	void VulkanBasePushConstant::SetValues(const void* values)
	{
		this->mValues = (void*)values;
	}
}