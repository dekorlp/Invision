#ifndef VULKAN_BASE_PUSH_COMMAND_H
#define VULKAN_BASE_PUSH_COMMAND_H

#include "vulkan\vulkan.h"
namespace Invision
{
#define INVISION_GPU_SPEC_PUSH_COMMAND_SIZE 128

	class VulkanBasePushConstant
	{
	private:
		VkShaderStageFlagBits mShaderStage;
		uint32_t mOffset;
		uint32_t mSize;

	public:
		INVISION_API VulkanBasePushConstant();
		INVISION_API VulkanBasePushConstant(VkShaderStageFlagBits shaderStages, uint32_t offset, uint32_t size);
		INVISION_API VkPushConstantRange ConstructPushConstantRange();

		INVISION_API VkShaderStageFlagBits GetShaderStages();
		INVISION_API uint32_t GetOffset();
		INVISION_API uint32_t GetSize();

		INVISION_API void SetShaderStages(VkShaderStageFlagBits shaderStages);
		INVISION_API void SetOffset(uint32_t offset);
		INVISION_API void SetSize(uint32_t size);
	};
}
#endif // VULKAN_BASE_PUSH_COMMAND_H