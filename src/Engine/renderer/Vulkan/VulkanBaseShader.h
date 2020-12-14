#ifndef VULKAN_BASE_SHADER_H
#define VULKAN_BASE_SHADER_H

#include "vulkan\vulkan.h"

namespace Invision
{
	class INVISION_API VulkanBaseShader
	{
	public:
		VulkanBaseShader(const SVulkanBase& vulkanInstance, const std::vector<char>& code, VkShaderStageFlagBits stage);
		VkShaderModule GetShaderModule();
		VkShaderStageFlagBits GetShaderStageFlatBits();
		void Destroy(const SVulkanBase& vulkanInstance);
	private:
		VkShaderModule mShaderModule;
		VkShaderStageFlagBits mShaderStage;
	};
}
#endif // VULKAN_BASE_SHADER_H