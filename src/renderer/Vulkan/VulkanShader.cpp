#include "precompiled.h"

#include "Vulkan.h"
#include "VulkanException.h"

#include "VulkanShader.h"

namespace Invision
{

	VulkanShader::VulkanShader(const SVulkan& vulkanInstance, const std::vector<char>& code, VkShaderStageFlagBits stage)
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(vulkanInstance.logicalDevice, &createInfo, nullptr, &mShaderModule) != VK_SUCCESS) {
			throw Invision::InvisionBaseRendererException("failed to create shader module!");
		}
		mShaderStage = stage;
	}

	VkShaderModule VulkanShader::GetShaderModule()
	{
		return mShaderModule;
	}

	VkShaderStageFlagBits VulkanShader::GetShaderStageFlatBits()
	{
		return mShaderStage;
	}

	void VulkanShader::Destroy(const SVulkan& vulkanInstance)
	{
		vkDestroyShaderModule(vulkanInstance.logicalDevice, mShaderModule, nullptr);
	}

}