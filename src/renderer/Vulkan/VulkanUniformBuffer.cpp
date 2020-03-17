#include "precompiled.h"

#include "Vulkan.h"
#include "VulkanException.h"


#include "VulkanUniformBuffer.h"

namespace Invision
{
	VulkanUniformBinding::VulkanUniformBinding(uint32_t binding,
		VkDescriptorType descriptorType,
		uint32_t descriptorCount,
		VkShaderStageFlags stageFlags,
		VkDeviceSize bufferSize,
		VkDeviceSize offset)
	{
		this->mBinding = binding;
		this->mDescriptorCount = descriptorCount;
		this->mDescriptorType = descriptorType;
		this->mStageFlags = stageFlags;
		this->mBufferSize = bufferSize;
		this->mOffset = offset;
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

	VkDeviceSize VulkanUniformBinding::GetBufferSize()
	{
		return mBufferSize;
	}

	VkDeviceSize VulkanUniformBinding::GetOffset()
	{
		return mOffset;
	}

	std::vector<VkDescriptorSet> VulkanUniformBinding::GetDescriptorSets()
	{
		return mDescriptorSets;
	}

	void VulkanUniformBinding::SetBuffers(std::vector<VulkanBuffer> uniformBuffer)
	{
		this->mUniformBuffer = uniformBuffer;
	}
	
	void VulkanUniformBinding::SetDescriptorSets(std::vector<VkDescriptorSet> descriptorSets)
	{
		mDescriptorSets = descriptorSets;
	}

	std::vector<VulkanBuffer> VulkanUniformBinding::GetBuffers()
	{
		return mUniformBuffer;
	}

	void VulkanUniformBinding::ClearAndDestroyBuffers(const SVulkan &vulkanInstance)
	{
		for (unsigned int i = 0; i < mUniformBuffer.size(); i++)
		{
			mUniformBuffer[i].DestroyBuffer(vulkanInstance);
		}
		mUniformBuffer.clear();
	}

	VulkanUniformBuffer::VulkanUniformBuffer()
	{
		this->mDescriptorSetLayout = VK_NULL_HANDLE;
	}

	VulkanUniformBuffer& VulkanUniformBuffer::CreateUniformBinding(uint32_t binding,
		VkDescriptorType descriptorType,
		uint32_t descriptorCount,
		VkShaderStageFlags stageFlags,
		VkDeviceSize bufferSize,
		VkDeviceSize offset)
	{
		VulkanUniformBinding uniformBinding(binding, descriptorType, descriptorCount, stageFlags, bufferSize, offset);
		bindings.push_back(uniformBinding);
		return *this;
	}

	void VulkanUniformBuffer::CreateUniformBuffer(const SVulkan &vulkanInstance)
	{
		CreateUniformSet(vulkanInstance);
		CreateBuffers(vulkanInstance);
		CreateDescriptorSets(vulkanInstance);
	}

	void VulkanUniformBuffer::CreateUniformSet(const SVulkan &vulkanInstance)
	{
		if (mDescriptorSetLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(vulkanInstance.logicalDevice, mDescriptorSetLayout, nullptr);
		}

		std::vector<VkDescriptorSetLayoutBinding> uboLayoutBindings;
		std::vector<VkDescriptorPoolSize> poolElements;

		for (unsigned int j = 0; j < bindings.size(); j++)
		{
			VkDescriptorSetLayoutBinding uboLayoutBinding = {};
			uboLayoutBinding.binding = bindings.at(j).GetBinding();
			uboLayoutBinding.descriptorCount = bindings.at(j).GetDescriptorCount();
			uboLayoutBinding.descriptorType = bindings.at(j).GetDescriptorType();
			uboLayoutBinding.stageFlags = bindings.at(j).GetStageFlags();
			uboLayoutBinding.pImmutableSamplers = nullptr;
			uboLayoutBindings.push_back(uboLayoutBinding);

			VkDescriptorPoolSize poolSize = {};
			poolSize.type = bindings.at(j).GetDescriptorType();
			poolSize.descriptorCount = static_cast<uint32_t>(vulkanInstance.swapChainImages.size());
			poolElements.push_back(poolSize);
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(uboLayoutBindings.size());
		layoutInfo.pBindings = uboLayoutBindings.data();

		mDescriptorPool.CreateDescriptorPool(vulkanInstance, poolElements);
		if (vkCreateDescriptorSetLayout(vulkanInstance.logicalDevice, &layoutInfo, nullptr, &mDescriptorSetLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}

	void VulkanUniformBuffer::CreateBuffers(const SVulkan &vulkanInstance)
	{
		for (unsigned int j = 0; j < bindings.size(); j++)
		{
			VkDeviceSize bufferSize = bindings.at(j).GetBufferSize();
			VkDeviceSize offset = bindings.at(j).GetOffset();
			std::vector<VulkanBuffer> uniformBuffers;

			for (unsigned int i = 0; i < vulkanInstance.swapChainImages.size(); i++)
			{
				VulkanBuffer buffer;
				buffer.CreateBuffer(vulkanInstance, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SHARING_MODE_EXCLUSIVE, offset);
				uniformBuffers.push_back(buffer);
			}
			bindings.at(j).SetBuffers(uniformBuffers);
		}
	}

	void VulkanUniformBuffer::DestroyUniformBuffer(const SVulkan &vulkanInstance)
	{
		DestroyUniformSet(vulkanInstance);
		mDescriptorPool.DestroyDescriptorPool(vulkanInstance);
	}

	void VulkanUniformBuffer::DestroyUniformSet(const SVulkan &vulkanInstance)
	{
		vkDestroyDescriptorSetLayout(vulkanInstance.logicalDevice, mDescriptorSetLayout, nullptr);
		mDescriptorSetLayout = VK_NULL_HANDLE;
		for (unsigned int i = 0; i < bindings.size(); i++)
		{
			bindings.at(i).ClearAndDestroyBuffers(vulkanInstance);
		}
	
	}

	VkDescriptorSetLayout VulkanUniformBuffer::GetDescriptorSetLayout()
	{
		return mDescriptorSetLayout;
	}

	void VulkanUniformBuffer::UpdateUniform(const SVulkan &vulkanInstance, const void* source, size_t size, uint32_t binding)
	{
		unsigned int index = -1;
		for (unsigned int i = 0; i < bindings.size(); i++)
		{
			if (bindings.at(i).GetBinding() == binding)
			{
				index = i;
				break;
			}
		}
		
		void* data;
		vkMapMemory(vulkanInstance.logicalDevice, bindings.at(index).GetBuffers()[vulkanInstance.mImageIndex].GetDeviceMemory(), 0, size, 0, &data);
		memcpy(data, source, size);
		vkUnmapMemory(vulkanInstance.logicalDevice, bindings.at(index).GetBuffers()[vulkanInstance.mImageIndex].GetDeviceMemory());
	}

	std::vector<VkDescriptorSet> VulkanUniformBuffer::GetDescriptorSets(uint32_t binding)
	{
		unsigned int index = -1;
		for (unsigned int i = 0; i < bindings.size(); i++)
		{
			if (bindings.at(i).GetBinding() == binding)
			{
				index = i;
				break;
			}
		}

		return bindings[index].GetDescriptorSets();
	}

	void VulkanUniformBuffer::CreateDescriptorSets(const SVulkan &vulkanInstance)
	{
		std::vector<VkDescriptorSetLayout> layouts(vulkanInstance.swapChainImages.size(), mDescriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = mDescriptorPool.GetDescriptorPool();
		allocInfo.descriptorSetCount = static_cast<uint32_t>(vulkanInstance.swapChainImages.size());
		allocInfo.pSetLayouts = layouts.data();

		for (unsigned int j = 0; j < bindings.size(); j++)
		{
			std::vector<VkDescriptorSet> descriptorSets;
			descriptorSets.resize(vulkanInstance.swapChainImages.size());

			if (vkAllocateDescriptorSets(vulkanInstance.logicalDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate descriptor sets!");
			}
			bindings[j].SetDescriptorSets(descriptorSets);

			for (size_t i = 0; i < vulkanInstance.swapChainImages.size(); i++) {
				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = bindings[j].GetBuffers()[i].GetBuffer();
				bufferInfo.offset = bindings[j].GetOffset();
				bufferInfo.range = bindings[j].GetBufferSize();

				VkWriteDescriptorSet descriptorWrite = {};
				descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrite.dstSet = descriptorSets[i];
				descriptorWrite.dstBinding = bindings[j].GetBinding();
				descriptorWrite.dstArrayElement = 0;

				descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				descriptorWrite.descriptorCount = bindings[j].GetDescriptorCount();

				descriptorWrite.pBufferInfo = &bufferInfo;
				descriptorWrite.pImageInfo = nullptr; // Optional
				descriptorWrite.pTexelBufferView = nullptr; // Optional

				vkUpdateDescriptorSets(vulkanInstance.logicalDevice, 1, &descriptorWrite, 0, nullptr);
			}

		}
	}
}