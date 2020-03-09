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


	VulkanSetLayout::VulkanSetLayout(uint32_t setNumber)
	{
		bindings = new std::vector<VulkanUniformBinding>();
		this->mSetNumber = setNumber;
	}

	void VulkanSetLayout::DestroyVulkanSetLayout()
	{
		delete bindings;
	}

	VulkanSetLayout& VulkanSetLayout::CreateUniformBinding(uint32_t binding,
		VkDescriptorType descriptorType,
		uint32_t descriptorCount,
		VkShaderStageFlags stageFlags)
	{
		VulkanUniformBinding uniformBinding(binding, descriptorType, descriptorCount, stageFlags);
		bindings->push_back(uniformBinding);
		return *this;
	}


	VulkanSetLayout VulkanUniformBuffer::CreateUniformBufferSet(uint32 setNumber)
	{
		VulkanSetLayout setLayout(setNumber);
		
		this->UniformSets.push_back(setLayout);

		return setLayout;
	}

	void VulkanUniformBuffer::DestroyUniformBuffer()
	{
		for (unsigned int i = 0; i < UniformSets.size(); i++)
		{
			UniformSets[i].DestroyVulkanSetLayout();
		}
		UniformSets.clear();
	}
}