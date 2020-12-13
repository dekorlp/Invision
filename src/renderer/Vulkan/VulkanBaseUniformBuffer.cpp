#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"


#include "VulkanBaseUniformBuffer.h"

namespace Invision
{
	VulkanBaseUniformBinding::VulkanBaseUniformBinding(
		uint32_t set, 
		uint32_t binding,
		VkDescriptorType descriptorType,
		uint32_t descriptorCount,
		VkShaderStageFlags stageFlags,
		VkDeviceSize bufferSize,
		VkDeviceSize offset,
		VkImageView imageView,
		VkSampler sampler)
	{
		this->mSetIndex = set;
		this->mBinding = binding;
		this->mDescriptorCount = descriptorCount;
		this->mDescriptorType = descriptorType;
		this->mStageFlags = stageFlags;
		this->mBufferSize = bufferSize;
		this->mOffset = offset;

		this->mImageInfo = {};
		this->mImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		this->mImageInfo.imageView = imageView;
		this->mImageInfo.sampler = sampler;

		this->mBufferInfo = {};
		this->mBufferInfo.offset = offset;
		this->mBufferInfo.range = bufferSize;
		this->mBufferInfo.buffer = VK_NULL_HANDLE;
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

	void VulkanBaseUniformBinding::CreateBaseBuffer(const SVulkanBase &vulkanInstance)
	{
		this->mUniformBuffer.CreateBuffer(vulkanInstance, mBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SHARING_MODE_EXCLUSIVE, mOffset);
		this->mBufferInfo.buffer = mUniformBuffer.GetBuffer();
	}

	VulkanBaseBuffer VulkanBaseUniformBinding::GetBaseBuffer()
	{
		return mUniformBuffer;
	}

	VkDescriptorImageInfo& VulkanBaseUniformBinding::GetImageInfo()
	{
		return mImageInfo;
	}

	VkDescriptorBufferInfo& VulkanBaseUniformBinding::GetBufferInfo()
	{
		return mBufferInfo;
	}

	void VulkanBaseUniformBinding::ClearAndDestroyBuffers(const SVulkanBase &vulkanInstance)
	{
		mUniformBuffer.DestroyBuffer(vulkanInstance);
	}

	VulkanBaseUniformBuffer::VulkanBaseUniformBuffer()
	{
		this->maxSet = 0;
	}

	

	VulkanBaseUniformBuffer& VulkanBaseUniformBuffer::CreateUniformBinding(uint32_t set, uint32_t binding,
		VkDescriptorType descriptorType,
		uint32_t descriptorCount,
		VkShaderStageFlags stageFlags,
		VkDeviceSize bufferSize,
		VkDeviceSize offset)
	{
		VulkanBaseUniformBinding uniformBinding(set, binding, descriptorType, descriptorCount, stageFlags, bufferSize, offset, VK_NULL_HANDLE, VK_NULL_HANDLE);
		bindings.push_back(uniformBinding);

		if (set > maxSet)
		{
			maxSet = set;
		}

		return *this;
	}

	INVISION_API VulkanBaseUniformBuffer& VulkanBaseUniformBuffer::CreateImageBinding(uint32_t set, uint32_t binding,
		VkDescriptorType descriptorType,
		uint32_t descriptorCount,
		VkShaderStageFlags stageFlags,
		VkImageView imageView,
		VkSampler sampler)
	{
		VulkanBaseUniformBinding uniformBinding(set, binding, descriptorType, descriptorCount, stageFlags, 0, 0, imageView, sampler);
		bindings.push_back(uniformBinding);

		if (set > maxSet)
		{
			maxSet = set;
		}

		return *this;
	}

	void VulkanBaseUniformBinding::SetSetIndex(uint32 setIndex)
	{
		mSetIndex = setIndex;
	}

	VkDeviceSize VulkanBaseUniformBinding::GetSetIndex()
	{
		return mSetIndex;
	}

	void VulkanBaseUniformBuffer::CreateUniformBuffer(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext)
	{
		CreateUniformSet(vulkanInstance, vulkanContext);
		CreateBuffers(vulkanInstance, vulkanContext);
		CreateDescriptorSets(vulkanInstance, vulkanContext);
	}

	void VulkanBaseUniformBuffer::CreateUniformSet(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext)
	{
		if (!mSets.empty())
		{
			for (int i = 0; i < mSets.size(); i++)
			{
				vkDestroyDescriptorSetLayout(vulkanInstance.logicalDevice, mSets[i].mDescriptorSetLayout, nullptr);
				mSets[i].mDescriptorPool.DestroyDescriptorPool(vulkanInstance);
			}
		}

		// allocate DescriptorSets
		mSets.resize(maxSet+1);

		for (int i = 0; i < mSets.size(); i++)
		{
			std::vector<VkDescriptorSetLayoutBinding> uboLayoutBindings;
			std::vector<VkDescriptorPoolSize> poolElements;

			for (unsigned int j = 0; j < bindings.size(); j++)
			{
				if (bindings.at(j).GetSetIndex() == i)
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
					poolSize.descriptorCount = static_cast<uint32_t>(vulkanContext.swapChainImages.size());
					poolElements.push_back(poolSize);
				}
			}

			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = static_cast<uint32_t>(uboLayoutBindings.size());
			layoutInfo.pBindings = uboLayoutBindings.data();

			
			if (vkCreateDescriptorSetLayout(vulkanInstance.logicalDevice, &layoutInfo, nullptr, &mSets[i].mDescriptorSetLayout) != VK_SUCCESS) {
				throw std::runtime_error("failed to create descriptor set layout!");
			}

			mSets[i].mDescriptorPool.CreateDescriptorPool(vulkanInstance, vulkanContext, poolElements);
		}

		
	}

	void VulkanBaseUniformBuffer::CreateBuffers(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext)
	{
		for (unsigned int j = 0; j < bindings.size(); j++)
		{
			if (bindings[j].GetDescriptorType() == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
			{
				bindings.at(j).CreateBaseBuffer(vulkanInstance);
			}
		}
	}

	void VulkanBaseUniformBuffer::DestroyUniformBuffer(const SVulkanBase &vulkanInstance)
	{
		DestroyUniformSet(vulkanInstance);
		for (int i = 0; i < mSets.size(); i++)
		{
			mSets[i].mDescriptorPool.DestroyDescriptorPool(vulkanInstance);
		}

		mSets.clear();
	}

	void VulkanBaseUniformBuffer::DestroyUniformSet(const SVulkanBase &vulkanInstance)
	{
		for (int i = 0; i < mSets.size(); i++)
		{
			vkDestroyDescriptorSetLayout(vulkanInstance.logicalDevice, mSets[i].mDescriptorSetLayout, nullptr);
		}

		for (unsigned int i = 0; i < bindings.size(); i++)
		{
			if (bindings[i].GetDescriptorType() == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) // Only Uniform Buffer have an UniformBuffer Object!
			{
				bindings.at(i).ClearAndDestroyBuffers(vulkanInstance);
			}
		}

	}

	std::vector<VulkanBaseUniformBuffer::SetProperties> VulkanBaseUniformBuffer::GetSets()
	{
		return mSets;
	}

	void VulkanBaseUniformBuffer::UpdateUniform(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext, const void* source, size_t size, uint32_t set, uint32_t binding)
	{
		int index = -1;
		for (unsigned int i = 0; i < bindings.size(); i++)
		{
			if (bindings.at(i).GetSetIndex() == set && bindings.at(i).GetBinding() == binding)
			{
				index = i;
				break;
			}
		}
		
		if (index != -1)
		{
			void* data;
			vkMapMemory(vulkanInstance.logicalDevice, bindings.at(index).GetBaseBuffer().GetDeviceMemory(), 0, size, 0, &data);
			memcpy(data, source, size);
			vkUnmapMemory(vulkanInstance.logicalDevice, bindings.at(index).GetBaseBuffer().GetDeviceMemory());
		}
		else
		{
			throw std::runtime_error("failed to update Uniform Buffer!");
		}
	}

	size_t VulkanBaseUniformBuffer::GetSizeOfBindings()
	{
		return bindings.size();
	}

	void VulkanBaseUniformBuffer::CreateDescriptorSets(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext)
	{
		for (unsigned int i = 0; i < mSets.size(); i++)
		{

			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = mSets[i].mDescriptorPool.GetDescriptorPool();
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &mSets[i].mDescriptorSetLayout;

			if (vkAllocateDescriptorSets(vulkanInstance.logicalDevice, &allocInfo, &mSets[i].mDescriptorSet) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate descriptor sets!");
			}
		}

		std::vector< VkWriteDescriptorSet> descriptorWrites;
		for (unsigned int j = 0; j < bindings.size(); j++)
		{

			if (bindings[j].GetDescriptorType() == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
			{
				VkWriteDescriptorSet descriptorWrite = {};
				descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrite.dstSet = mSets[bindings[j].GetSetIndex()].mDescriptorSet;
				descriptorWrite.dstBinding = bindings[j].GetBinding();
				descriptorWrite.dstArrayElement = 0;

				descriptorWrite.descriptorType = bindings[j].GetDescriptorType();
				descriptorWrite.descriptorCount = bindings[j].GetDescriptorCount();

				descriptorWrite.pBufferInfo = &bindings[j].GetBufferInfo();
				descriptorWrite.pImageInfo = VK_NULL_HANDLE; // Optional
				descriptorWrite.pTexelBufferView = VK_NULL_HANDLE; // Optional
					
				descriptorWrites.push_back(descriptorWrite);
			}
			else if (bindings[j].GetDescriptorType() == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
			{
				VkWriteDescriptorSet descriptorWrite = {};
				descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrite.dstSet = mSets[bindings[j].GetSetIndex()].mDescriptorSet;
				descriptorWrite.dstBinding = bindings[j].GetBinding();
				descriptorWrite.dstArrayElement = 0;

				descriptorWrite.descriptorType = bindings[j].GetDescriptorType();
				descriptorWrite.descriptorCount = bindings[j].GetDescriptorCount();

				descriptorWrite.pBufferInfo = VK_NULL_HANDLE;
				descriptorWrite.pImageInfo = &bindings[j].GetImageInfo(); // Optional
				descriptorWrite.pTexelBufferView = VK_NULL_HANDLE; // Optional
					
				descriptorWrites.push_back(descriptorWrite);

			}
		}
		vkUpdateDescriptorSets(vulkanInstance.logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}