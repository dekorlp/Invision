#include "precompiled.h"

#include "Vulkan.h"
#include "VulkanException.h"


#include "VulkanUniformBuffer.h"

namespace Invision
{
	VulkanUniformBinding::VulkanUniformBinding(uint32_t binding,
		VkDescriptorType descriptorType,
		uint32_t descriptorCount,
		VkShaderStageFlags stageFlags)
	{
		this->mBinding = binding;
		this->mDescriptorCount = descriptorCount;
		this->mDescriptorType = descriptorType;
		this->mStageFlags = stageFlags;
	}

	uint32_t VulkanUniformBinding::GetBinding()
	{
		return mBinding;
	}

	VkDescriptorType VulkanUniformBinding::GetDescriptorType()
	{
		return mDescriptorType;
	}

	uint32_t VulkanUniformBinding::GetDescriptorCount()
	{
		return mDescriptorCount;
	}

	VkShaderStageFlags VulkanUniformBinding::GetStageFlags()
	{
		return mStageFlags;
	}

	
	VulkanUniformBuffer::VulkanUniformBuffer()
	{
		this->mDescriptorSetLayout = VK_NULL_HANDLE;
	}

	VulkanUniformBuffer& VulkanUniformBuffer::CreateUniformBinding(uint32_t binding,
		VkDescriptorType descriptorType,
		uint32_t descriptorCount,
		VkShaderStageFlags stageFlags)
	{
		VulkanUniformBinding uniformBinding(binding, descriptorType, descriptorCount, stageFlags);
		bindings.push_back(uniformBinding);
		return *this;
	}

	void VulkanUniformBuffer::CreateUniformSet(const SVulkan &vulkanInstance)
	{
		if (mDescriptorSetLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(vulkanInstance.logicalDevice, mDescriptorSetLayout, nullptr);
		}

		std::vector<VkDescriptorSetLayoutBinding> uboLayoutBindings;

		for (unsigned int j = 0; j < bindings.size(); j++)
		{
			VkDescriptorSetLayoutBinding uboLayoutBinding = {};
			uboLayoutBinding.binding = bindings.at(j).GetBinding();
			uboLayoutBinding.descriptorCount = bindings.at(j).GetDescriptorCount();
			uboLayoutBinding.descriptorType = bindings.at(j).GetDescriptorType();
			uboLayoutBinding.stageFlags = bindings.at(j).GetStageFlags();
			uboLayoutBinding.pImmutableSamplers = nullptr;
			uboLayoutBindings.push_back(uboLayoutBinding);
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(uboLayoutBindings.size());
		layoutInfo.pBindings = uboLayoutBindings.data();

		if (vkCreateDescriptorSetLayout(vulkanInstance.logicalDevice, &layoutInfo, nullptr, &mDescriptorSetLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}

	void VulkanUniformBuffer::DestroyUniformSet(const SVulkan &vulkanInstance)
	{
		vkDestroyDescriptorSetLayout(vulkanInstance.logicalDevice, mDescriptorSetLayout, nullptr);
	}

	VkDescriptorSetLayout VulkanUniformBuffer::GetDescriptorSetLayout()
	{
		return mDescriptorSetLayout;
	}
}