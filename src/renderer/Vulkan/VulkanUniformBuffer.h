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
				VkShaderStageFlags stageFlags);
			INVISION_API uint32_t GetBinding();
			INVISION_API VkDescriptorType GetDescriptorType();
			INVISION_API uint32_t GetDescriptorCount();
			INVISION_API VkShaderStageFlags GetStageFlags();
	};

	class VulkanSetLayout
	{
		private:
			std::vector<VulkanUniformBinding> *bindings;
			uint32_t mSetNumber;
		public:
			INVISION_API VulkanSetLayout(uint32_t setNumber);
			INVISION_API void DestroyVulkanSetLayout();
			INVISION_API VulkanSetLayout& CreateUniformBinding(uint32_t binding,
				VkDescriptorType descriptorType,
				uint32_t descriptorCount,
				VkShaderStageFlags stageFlags);
			INVISION_API VulkanUniformBinding& GetBinding(unsigned int index);
			INVISION_API unsigned int GetSize();
			INVISION_API uint32_t GetSetNumber();
	};

	// Binding Description
	class VulkanUniformBuffer
	{
	private:
		std::vector<VulkanSetLayout> UniformSets;
		std::vector<VkDescriptorSetLayout> mDescriptorSetLayouts;
	public:
		INVISION_API VulkanSetLayout CreateUniformBufferSet();
		INVISION_API void CreateUniformBuffer(const SVulkan &vulkanInstance);
		INVISION_API void DestroyUniformBuffer(const SVulkan &vulkanInstance);
	};
}

#endif // VULKAN_UNIFORM_BUFFER_H