#include "precompiled.h"

#include "VulkanBasePushConstant.h"

VulkanBasePushCommand::VulkanBasePushCommand(VkShaderStageFlagBits shaderStages, uint32_t offset, uint32_t size, const void* values)
{
	this->mShaderStage = shaderStages;
	this->mOffset = offset;
	this->mSize = size;
	this->mValues = (void*) values;
}


VkPushConstantRange VulkanBasePushCommand::ConstructPushCommandRange()
{
	VkPushConstantRange range;

	range.stageFlags = mShaderStage;
	range.offset = mOffset;
	range.size = mSize;

	return range;
}

VkShaderStageFlagBits VulkanBasePushCommand::GetShaderStages()
{
	return mShaderStage;
}

uint32_t VulkanBasePushCommand::GetOffset()
{
	return mOffset;
}

uint32_t VulkanBasePushCommand::GetSize()
{
	return mSize;
}

const void* VulkanBasePushCommand::GetValues()
{
	return mValues;
}

void VulkanBasePushCommand::SetShaderStages(VkShaderStageFlagBits shaderStages)
{
	this->mShaderStage = shaderStages;
}

void VulkanBasePushCommand::SetOffset(uint32_t offset)
{
	this->mOffset = offset;
}

void VulkanBasePushCommand::SetSize(uint32_t size)
{
	this->mSize = size;
}

void VulkanBasePushCommand::SetValues(const void* values)
{
	this->mValues = (void*)values;
}