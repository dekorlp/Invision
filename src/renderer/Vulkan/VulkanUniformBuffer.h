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
	};

	// Binding Description
	class VulkanUniformBuffer
	{
	private:
		std::vector<VulkanSetLayout> UniformSets;
	public:
		INVISION_API VulkanSetLayout CreateUniformBufferSet(uint32 setNumber);
		INVISION_API void DestroyUniformBuffer();
	};
}

#endif // VULKAN_UNIFORM_BUFFER_H