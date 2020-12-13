#include "precompiled.h"

#include "VulkanBase.h"
#include "VulkanBaseException.h"


#include "VulkanBaseUniformBuffer.h"

namespace Invision
{
	VulkanBaseUniformBinding::VulkanBaseUniformBinding(uint32_t set, uint32_t binding,
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

	void VulkanBaseUniformBinding::SetBaseBuffer(VulkanBaseBuffer uniformBuffer)
	{
		this->mUniformBuffer = uniformBuffer;
	}

	VulkanBaseBuffer VulkanBaseUniformBinding::GetBaseBuffer()
	{
		return mUniformBuffer;
	}

	VkDescriptorImageInfo VulkanBaseUniformBinding::GetImageInfo()
	{
		return mImageInfo;
	}

	void VulkanBaseUniformBinding::ClearAndDestroyBuffers(const SVulkanBase &vulkanInstance)
	{
		mUniformBuffer.DestroyBuffer(vulkanInstance);
	}

	VulkanBaseUniformBuffer::VulkanBaseUniformBuffer()
	{
		//this->mDescriptorSetLayout = VK_NULL_HANDLE;
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
		if (!mDescriptorSetLayout.empty())
		{
			for (int i = 0; i < mDescriptorSetLayout.size(); i++)
			{
				vkDestroyDescriptorSetLayout(vulkanInstance.logicalDevice, mDescriptorSetLayout[i], nullptr);
			}			
		}

		// allocate DescriptorSets
		mDescriptorSetLayout.resize(maxSet + 1);

		std::vector<VkDescriptorPoolSize> poolElements;

		for (int i = 0; i < mDescriptorSetLayout.size(); i++)
		{
			std::vector<VkDescriptorSetLayoutBinding> uboLayoutBindings;
			

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

			
			if (vkCreateDescriptorSetLayout(vulkanInstance.logicalDevice, &layoutInfo, nullptr, &mDescriptorSetLayout[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create descriptor set layout!");
			}
		}

		mDescriptorPool.CreateDescriptorPool(vulkanInstance, vulkanContext, poolElements);
	}

	void VulkanBaseUniformBuffer::CreateBuffers(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext)
	{
		for (unsigned int j = 0; j < bindings.size(); j++)
		{
			if (bindings[j].GetDescriptorType() == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
			{

				VkDeviceSize bufferSize = bindings.at(j).GetBufferSize();
				VkDeviceSize offset = bindings.at(j).GetOffset();
				

				VulkanBaseBuffer buffer;
				buffer.CreateBuffer(vulkanInstance, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SHARING_MODE_EXCLUSIVE, offset);


				bindings.at(j).SetBaseBuffer(buffer);
			}
		}
	}

	void VulkanBaseUniformBuffer::DestroyUniformBuffer(const SVulkanBase &vulkanInstance)
	{
		DestroyUniformSet(vulkanInstance);
		mDescriptorPool.DestroyDescriptorPool(vulkanInstance);
	}

	void VulkanBaseUniformBuffer::DestroyUniformSet(const SVulkanBase &vulkanInstance)
	{
		for (int i = 0; i < mDescriptorSetLayout.size(); i++)
		{
			vkDestroyDescriptorSetLayout(vulkanInstance.logicalDevice, mDescriptorSetLayout[i], nullptr);
		}
		mDescriptorSetLayout.clear();
	
		for (unsigned int i = 0; i < bindings.size(); i++)
		{
			if (bindings[i].GetDescriptorType() == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) // Only Uniform Buffer have an UniformBuffer Object!
			{
				bindings.at(i).ClearAndDestroyBuffers(vulkanInstance);
			}
		}
	
	}

	std::vector<VkDescriptorSetLayout> VulkanBaseUniformBuffer::GetDescriptorSetLayout()
	{
		return mDescriptorSetLayout;
	}

	void VulkanBaseUniformBuffer::UpdateUniform(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext, const void* source, size_t size, uint32_t set, uint32_t binding)
	{
		unsigned int index = -1;
		for (unsigned int i = 0; i < bindings.size(); i++)
		{
			if (bindings.at(i).GetSetIndex() == set && bindings.at(i).GetBinding() == binding)
			{
				index = i;
				break;
			}
		}
		
		void* data;
		vkMapMemory(vulkanInstance.logicalDevice, bindings.at(index).GetBaseBuffer().GetDeviceMemory(), 0, size, 0, &data);
		memcpy(data, source, size);
		vkUnmapMemory(vulkanInstance.logicalDevice, bindings.at(index).GetBaseBuffer().GetDeviceMemory());
	}

	std::vector<VkDescriptorSet> VulkanBaseUniformBuffer::GetDescriptorSets()
	{
		return mDescriptorSets;
	}

	size_t VulkanBaseUniformBuffer::GetSizeOfBindings()
	{
		return bindings.size();
	}

	void VulkanBaseUniformBuffer::CreateDescriptorSets(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext)
	{
		//std::vector<VkDescriptorSetLayout> layouts(1, mDescriptorSetLayout);

		

		
		mDescriptorSets.resize(maxSet+1);

		for (unsigned int i = 0; i < mDescriptorSets.size(); i++)
		{

			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = mDescriptorPool.GetDescriptorPool();
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &mDescriptorSetLayout[i];

			if (vkAllocateDescriptorSets(vulkanInstance.logicalDevice, &allocInfo, &mDescriptorSets[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate descriptor sets!");
			}
		}

			//VkDescriptorSet descriptorSet;

			//if (vkAllocateDescriptorSets(vulkanInstance.logicalDevice, &allocInfo, &descriptorSet) != VK_SUCCESS) {
			//	throw std::runtime_error("failed to allocate descriptor sets!");
			//}
			//mDescriptorSets.push_back(descriptorSet);

			std::vector< VkWriteDescriptorSet> descriptorWrites;
			for (unsigned int j = 0; j < bindings.size(); j++)
			{

				//bindings[j].SetSetIndex(static_cast<uint32>(mDescriptorSets.size()-1)); // Actually there is only one Set

				if (bindings[j].GetDescriptorType() == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
				{
					VkDescriptorBufferInfo bufferInfo = {};
					bufferInfo.buffer = bindings[j].GetBaseBuffer().GetBuffer();
					bufferInfo.offset = bindings[j].GetOffset();
					bufferInfo.range = bindings[j].GetBufferSize();

					VkWriteDescriptorSet descriptorWrite = {};
					descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrite.dstSet = mDescriptorSets[bindings[j].GetSetIndex()];
					descriptorWrite.dstBinding = bindings[j].GetBinding();
					descriptorWrite.dstArrayElement = 0;

					descriptorWrite.descriptorType = bindings[j].GetDescriptorType();
					descriptorWrite.descriptorCount = bindings[j].GetDescriptorCount();

					descriptorWrite.pBufferInfo = &bufferInfo;
					descriptorWrite.pImageInfo = VK_NULL_HANDLE; // Optional
					descriptorWrite.pTexelBufferView = VK_NULL_HANDLE; // Optional
					
					descriptorWrites.push_back(descriptorWrite);
				}
				else if (bindings[j].GetDescriptorType() == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
				{
					

					VkDescriptorImageInfo imageDescriptorInfo = {};
					imageDescriptorInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageDescriptorInfo.imageView = bindings[j].GetImageInfo().imageView;
					imageDescriptorInfo.sampler = bindings[j].GetImageInfo().sampler;

					VkWriteDescriptorSet descriptorWrite = {};
					descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrite.dstSet = mDescriptorSets[bindings[j].GetSetIndex()];
					descriptorWrite.dstBinding = bindings[j].GetBinding();
					descriptorWrite.dstArrayElement = 0;

					descriptorWrite.descriptorType = bindings[j].GetDescriptorType();
					descriptorWrite.descriptorCount = bindings[j].GetDescriptorCount();

					descriptorWrite.pBufferInfo = VK_NULL_HANDLE;
					descriptorWrite.pImageInfo = &imageDescriptorInfo; // Optional
					descriptorWrite.pTexelBufferView = VK_NULL_HANDLE; // Optional
					
					descriptorWrites.push_back(descriptorWrite);

				}
		}
		vkUpdateDescriptorSets(vulkanInstance.logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}