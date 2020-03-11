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
			VkDeviceSize mBufferSize;
			std::vector<VkBuffer> mUniformBuffers;
			std::vector<VkDeviceMemory> mUniformBuffersMemory;

		public:
			INVISION_API VulkanUniformBinding(uint32_t binding,
				VkDescriptorType descriptorType,
				uint32_t descriptorCount,
				VkShaderStageFlags stageFlags,
				VkDeviceSize bufferSize);
			INVISION_API uint32_t GetBinding();
			INVISION_API VkDescriptorType GetDescriptorType();
			INVISION_API uint32_t GetDescriptorCount();
			INVISION_API VkShaderStageFlags GetStageFlags();
			INVISION_API VkDeviceSize GetBufferSize();
			INVISION_API void SetBuffers(std::vector<VkBuffer> uniformBuffers, std::vector<VkDeviceMemory> uniformBuffersMemory);
			INVISION_API std::vector<VkBuffer> GetBuffers();
			INVISION_API std::vector<VkDeviceMemory> GetBuffersMemory();
	};

	// Binding Description
	class VulkanUniformBuffer
	{
	private:
		std::vector<VulkanUniformBinding> bindings;
		VkDescriptorSetLayout mDescriptorSetLayout;

		INVISION_API void CreateUniformSet(const SVulkan &vulkanInstance);
		INVISION_API void CreateBuffers(const SVulkan &vulkanInstance);
		INVISION_API void DestroyUniformSet(const SVulkan &vulkanInstance);

	public:
		INVISION_API VulkanUniformBuffer();
		INVISION_API VulkanUniformBuffer& CreateUniformBinding(uint32_t binding,
			VkDescriptorType descriptorType,
			uint32_t descriptorCount,
			VkShaderStageFlags stageFlags,
			VkDeviceSize bufferSize);
		INVISION_API void CreateUniformBuffer(const SVulkan &vulkanInstance);
		INVISION_API void DestroyUniformBuffer(const SVulkan &vulkanInstance);
		INVISION_API VkDescriptorSetLayout GetDescriptorSetLayout();
	};
}

#endif // VULKAN_UNIFORM_BUFFER_H