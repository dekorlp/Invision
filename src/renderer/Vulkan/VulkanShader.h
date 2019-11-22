#ifndef VULKAN_SHADER_H
#define VULKAN_SHADER_H

#include "vulkan\vulkan.h"
#include "Vulkan.h"
#include "VulkanException.h"

namespace Invision
{
	class INVISION_API VulkanShader
	{
	public:
		VulkanShader(const SVulkan& vulkanInstance, const std::vector<char>& code, VkShaderStageFlagBits stage);
		VkShaderModule GetShaderModule();
		VkShaderStageFlagBits GetShaderStageFlatBits();
		void Destroy(const SVulkan& vulkanInstance);
	private:
		VkShaderModule mShaderModule;
		VkShaderStageFlagBits mShaderStage;
	};
}
#endif // VULKAN_SHADER_H