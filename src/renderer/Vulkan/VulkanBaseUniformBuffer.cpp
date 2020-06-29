#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"


#include "VulkanBaseUniformBuffer.h"

namespace Invision
{
	VulkanBaseUniformBinding::VulkanBaseUniformBinding(uint32_t binding,
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

	uint32_t VulkanBaseUniformBinding::GetBinding()
	{
		return mBinding;
	}

	VkDescriptorType VulkanBaseUniformBinding::GetDescriptorType()
	{
		return mDescriptorType;
	}

	uint32_t VulkanBaseUniformBinding::GetDescriptorCount()
	{
		return mDescriptorCount;
	}

	VkShaderStageFlags VulkanBaseUniformBinding::GetStageFlags()
	{
		return mStageFlags;
	}

	VkDeviceSize VulkanBaseUniformBinding::GetBufferSize()
	{
		return mBufferSize;
	}

	VkDeviceSize VulkanBaseUniformBinding::GetOffset()
	{
		return mOffset;
	}

	std::vector<VkDescriptorSet> VulkanBaseUniformBinding::GetDescriptorSets()
	{
		return mDescriptorSets;
	}

	void VulkanBaseUniformBinding::SetBuffers(std::vector<VulkanBaseBuffer> uniformBuffer)
	{
		this->mUniformBuffer = uniformBuffer;
	}
	
	void VulkanBaseUniformBinding::SetDescriptorSets(std::vector<VkDescriptorSet> descriptorSets)
	{
		mDescriptorSets = descriptorSets;
	}

	std::vector<VulkanBaseBuffer> VulkanBaseUniformBinding::GetBuffers()
	{
		return mUniformBuffer;
	}

	void VulkanBaseUniformBinding::ClearAndDestroyBuffers(const SVulkanBase &vulkanInstance)
	{
		for (unsigned int i = 0; i < mUniformBuffer.size(); i++)
		{
			mUniformBuffer[i].DestroyBuffer(vulkanInstance);
		}
		mUniformBuffer.clear();
	}

	VulkanBaseUniformBuffer::VulkanBaseUniformBuffer()
	{
		this->mDescriptorSetLayout = VK_NULL_HANDLE;
	}

	VulkanBaseUniformBuffer& VulkanBaseUniformBuffer::CreateUniformBinding(uint32_t binding,
		VkDescriptorType descriptorType,
		uint32_t descriptorCount,
		VkShaderStageFlags stageFlags,
		VkDeviceSize bufferSize,
		VkDeviceSize offset)
	{
		VulkanBaseUniformBinding uniformBinding(binding, descriptorType, descriptorCount, stageFlags, bufferSize, offset);
		bindings.push_back(uniformBinding);
		return *this;
	}

	void VulkanBaseUniformBuffer::CreateUniformBuffer(const SVulkanBase &vulkanInstance)
	{
		CreateUniformSet(vulkanInstance);
		CreateBuffers(vulkanInstance);
		CreateDescriptorSets(vulkanInstance);
	}

	void VulkanBaseUniformBuffer::CreateUniformSet(const SVulkanBase &vulkanInstance)
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

	void VulkanBaseUniformBuffer::CreateBuffers(const SVulkanBase &vulkanInstance)
	{
		for (unsigned int j = 0; j < bindings.size(); j++)
		{
			VkDeviceSize bufferSize = bindings.at(j).GetBufferSize();
			VkDeviceSize offset = bindings.at(j).GetOffset();
			std::vector<VulkanBaseBuffer> uniformBuffers;

			for (unsigned int i = 0; i < vulkanInstance.swapChainImages.size(); i++)
			{
				VulkanBaseBuffer buffer;
				buffer.CreateBuffer(vulkanInstance, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SHARING_MODE_EXCLUSIVE, offset);
				uniformBuffers.push_back(buffer);
			}
			bindings.at(j).SetBuffers(uniformBuffers);
		}
	}

	void VulkanBaseUniformBuffer::DestroyUniformBuffer(const SVulkanBase &vulkanInstance)
	{
		DestroyUniformSet(vulkanInstance);
		mDescriptorPool.DestroyDescriptorPool(vulkanInstance);
	}

	void VulkanBaseUniformBuffer::DestroyUniformSet(const SVulkanBase &vulkanInstance)
	{
		vkDestroyDescriptorSetLayout(vulkanInstance.logicalDevice, mDescriptorSetLayout, nullptr);
		mDescriptorSetLayout = VK_NULL_HANDLE;
		for (unsigned int i = 0; i < bindings.size(); i++)
		{
			bindings.at(i).ClearAndDestroyBuffers(vulkanInstance);
		}
	
	}

	VkDescriptorSetLayout VulkanBaseUniformBuffer::GetDescriptorSetLayout()
	{
		return mDescriptorSetLayout;
	}

	void VulkanBaseUniformBuffer::UpdateUniform(const SVulkanBase &vulkanInstance, const void* source, size_t size, uint32_t binding)
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

	std::vector<VkDescriptorSet> VulkanBaseUniformBuffer::GetDescriptorSets(uint32_t binding)
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

	std::vector<VkDescriptorSet> VulkanBaseUniformBuffer::GetDescriptorSetsByIndex(uint32_t index)
	{
		return bindings[index].GetDescriptorSets();
	}

	size_t VulkanBaseUniformBuffer::GetSizeOfBindings()
	{
		return bindings.size();
	}

	void VulkanBaseUniformBuffer::CreateDescriptorSets(const SVulkanBase &vulkanInstance)
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

				descriptorWrite.descriptorType = bindings[j].GetDescriptorType();
				descriptorWrite.descriptorCount = bindings[j].GetDescriptorCount();

				descriptorWrite.pBufferInfo = &bufferInfo;
				descriptorWrite.pImageInfo = nullptr; // Optional
				descriptorWrite.pTexelBufferView = nullptr; // Optional

				vkUpdateDescriptorSets(vulkanInstance.logicalDevice, 1, &descriptorWrite, 0, nullptr);
			}

		}
	}
}