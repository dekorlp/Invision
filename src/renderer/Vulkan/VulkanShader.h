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

	private:
		VkShaderModule mModule;
		VkShaderStageFlagBits mShaderStage;
	};
}
#endif // VULKAN_SHADER_H