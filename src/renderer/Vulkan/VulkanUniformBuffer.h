#ifndef VULKAN_UNIFORM_BUFFER_H
#define VULKAN_UNIFORM_BUFFER_H

#include "vulkan\vulkan.h"
#include "VulkanBuffer.h"

namespace Invision
{
	class VulkanUniformBinding
	{
		private:
			uint32_t mBinding;
			VkDescriptorType mDescriptorType;
			uint32_t mDescriptorCount;
			VkShaderStageFlags mStageFlags;
		public:
			INVISION_API VulkanUniformBinding(uint32_t binding,
				VkDescriptorType descriptorType,
				uint32_t descriptorCount,
				VkShaderStageFlags stageFlags)
			{
				this->mBinding = binding;
				this->mDescriptorCount = descriptorCount;
				this->mDescriptorType = descriptorType;
				this->mStageFlags = stageFlags;
			}
	};

	class VulkanSetLayout
	{
		private:
			std::vector<VulkanUniformBinding> *bindings;
			uint32_t mSetNumber;
		public:
			INVISION_API VulkanSetLayout(uint32_t setNumber)
			{
				bindings = new std::vector<VulkanUniformBinding>();
				this->mSetNumber = setNumber;
			}

			INVISION_API void DestroyVulkanSetLayout()
			{
				delete bindings;
			}

			INVISION_API VulkanSetLayout& CreateUniformBinding(uint32_t binding,
			VkDescriptorType descriptorType,
			uint32_t descriptorCount,
			VkShaderStageFlags stageFlags)
			{
				VulkanUniformBinding uniformBinding(binding, descriptorType, descriptorCount, stageFlags);
				bindings->push_back(uniformBinding);
				return *this;
			}
	};

	// Binding Description
	class  VulkanUniformBuffer
	{
	private:
		std::vector<VulkanSetLayout> UniformSets;
	public:
		INVISION_API VulkanSetLayout CreateUniformBufferSet(uint32 setNumber);
		INVISION_API void DestroyUniformBuffer();
	};
}

#endif // VULKAN_UNIFORM_BUFFER_H