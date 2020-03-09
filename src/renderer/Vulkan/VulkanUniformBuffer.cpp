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

	VulkanUniformBinding& VulkanSetLayout::GetBinding(unsigned int index)
	{
		return this->bindings->at(index);
	}

	unsigned int VulkanSetLayout::GetSize()
	{
		return this->bindings->size();
	}

	uint32_t VulkanSetLayout::GetSetNumber()
	{
		return mSetNumber;
	}

	VulkanSetLayout VulkanUniformBuffer::CreateUniformBufferSet()
	{
		VulkanSetLayout setLayout(UniformSets.size());
		
		this->UniformSets.push_back(setLayout);

		return setLayout;
	}

	void VulkanUniformBuffer::CreateUniformBuffer(const SVulkan &vulkanInstance)
	{
		if (mDescriptorSetLayouts.size() > 0)
		{
			for (unsigned int i = 0; i < mDescriptorSetLayouts.size(); i++)
			{
				vkDestroyDescriptorSetLayout(vulkanInstance.logicalDevice, mDescriptorSetLayouts[i], nullptr);
			}
			mDescriptorSetLayouts.clear();
		}

		for (unsigned int i = 0; i < UniformSets.size(); i++)
		{
			std::vector<VkDescriptorSetLayoutBinding> uboLayoutBindings;

			for (unsigned int j = 0; j < UniformSets[i].GetSize(); j++)
			{
				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = UniformSets[i].GetBinding(j).GetBinding();
				uboLayoutBinding.descriptorCount = UniformSets[i].GetBinding(j).GetDescriptorCount();
				uboLayoutBinding.descriptorType = UniformSets[i].GetBinding(j).GetDescriptorType();
				uboLayoutBinding.stageFlags = UniformSets[i].GetBinding(j).GetStageFlags();
				uboLayoutBinding.pImmutableSamplers = nullptr;
				uboLayoutBindings.push_back(uboLayoutBinding);
			}

			VkDescriptorSetLayout descriptorSetLayout;

			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = static_cast<uint32_t>(uboLayoutBindings.size());
			layoutInfo.pBindings = uboLayoutBindings.data();

			if (vkCreateDescriptorSetLayout(vulkanInstance.logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
				throw std::runtime_error("failed to create descriptor set layout!");
			}
			mDescriptorSetLayouts.push_back(descriptorSetLayout);
		}
	}

	void VulkanUniformBuffer::DestroyUniformBuffer(const SVulkan &vulkanInstance)
	{
		if (mDescriptorSetLayouts.size() > 0)
		{
			for (unsigned int i = 0; i < mDescriptorSetLayouts.size(); i++)
			{
				vkDestroyDescriptorSetLayout(vulkanInstance.logicalDevice, mDescriptorSetLayouts[i], nullptr);
			}
			mDescriptorSetLayouts.clear();
		}

		for (unsigned int i = 0; i < UniformSets.size(); i++)
		{
			UniformSets[i].DestroyVulkanSetLayout();
		}
		UniformSets.clear();
	}
}