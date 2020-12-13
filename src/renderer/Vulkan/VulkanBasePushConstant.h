#ifndef VULKAN_BASE_UNIFORM_BUFFER_H
#define VULKAN_BASE_UNIFORM_BUFFER_H

#include "vulkan\vulkan.h"

class VulkanBasePushCommand
{
private:
	VkShaderStageFlagBits mShaderStage;
	uint32_t mOffset;
	uint32_t mSize;
	void* mValues;

public:
	INVISION_API VulkanBasePushCommand(VkShaderStageFlagBits shaderStages, uint32_t offset, uint32_t size, const void* values);
	INVISION_API VkPushConstantRange ConstructPushCommandRange();
	
	INVISION_API VkShaderStageFlagBits GetShaderStages();
	INVISION_API uint32_t GetOffset();
	INVISION_API uint32_t GetSize();
	INVISION_API const void* GetValues();

	INVISION_API void SetShaderStages(VkShaderStageFlagBits shaderStages);
	INVISION_API void SetOffset(uint32_t offset);
	INVISION_API void SetSize(uint32_t size);
	INVISION_API void SetValues(const void* values);
};

#endif // VULKAN_BASE_UNIFORM_BUFFER_H