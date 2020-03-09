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

	// Binding Description
	class VulkanUniformBuffer
	{
	private:
		std::vector<VulkanUniformBinding> bindings;
		VkDescriptorSetLayout mDescriptorSetLayout;
	public:
		INVISION_API VulkanUniformBuffer();
		INVISION_API VulkanUniformBuffer& CreateUniformBinding(uint32_t binding,
			VkDescriptorType descriptorType,
			uint32_t descriptorCount,
			VkShaderStageFlags stageFlags);

		INVISION_API void CreateUniformSet(const SVulkan &vulkanInstance);
		INVISION_API void DestroyUniformSet(const SVulkan &vulkanInstance);
		INVISION_API VkDescriptorSetLayout GetDescriptorSetLayout();
	};
}

#endif // VULKAN_UNIFORM_BUFFER_H