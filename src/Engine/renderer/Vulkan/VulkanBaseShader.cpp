#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"

#include "VulkanBaseShader.h"

namespace Invision
{

	VulkanBaseShader::VulkanBaseShader(const SVulkanContext& vulkanContext, const std::vector<char>& code, VkShaderStageFlagBits stage)
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(vulkanContext.logicalDevice, &createInfo, nullptr, &mShaderModule) != VK_SUCCESS) {
			throw Invision::InvisionBaseRendererException("failed to create shader module!");
		}
		mShaderStage = stage;
	}

	VkShaderModule VulkanBaseShader::GetShaderModule()
	{
		return mShaderModule;
	}

	VkShaderStageFlagBits VulkanBaseShader::GetShaderStageFlatBits()
	{
		return mShaderStage;
	}

	void VulkanBaseShader::Destroy(const SVulkanContext& vulkanContext)
	{
		vkDestroyShaderModule(vulkanContext.logicalDevice, mShaderModule, nullptr);
	}

}