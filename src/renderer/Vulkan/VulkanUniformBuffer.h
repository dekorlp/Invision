#ifndef VULKAN_UNIFORM_BUFFER_H
#define VULKAN_UNIFORM_BUFFER_H

#include "vulkan\vulkan.h"
#include "VulkanBuffer.h"
#include "VulkanDescriptorPool.h"

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
			VkDeviceSize mOffset;
			std::vector<VulkanBuffer> mUniformBuffer;
			std::vector<VkDescriptorSet> mDescriptorSets;

		public:
			INVISION_API VulkanUniformBinding(uint32_t binding,
				VkDescriptorType descriptorType,
				uint32_t descriptorCount,
				VkShaderStageFlags stageFlags,
				VkDeviceSize bufferSize,
				VkDeviceSize offset);
			INVISION_API uint32_t GetBinding();
			INVISION_API VkDescriptorType GetDescriptorType();
			INVISION_API uint32_t GetDescriptorCount();
			INVISION_API VkShaderStageFlags GetStageFlags();
			INVISION_API VkDeviceSize GetBufferSize();
			INVISION_API VkDeviceSize GetOffset();
			INVISION_API void SetBuffers(std::vector<VulkanBuffer> uniformBuffer);
			INVISION_API std::vector<VulkanBuffer> GetBuffers();
			INVISION_API void ClearAndDestroyBuffers(const SVulkan &vulkanInstance);
			INVISION_API void SetDescriptorSets(std::vector<VkDescriptorSet> descriptorSets);
			INVISION_API std::vector<VkDescriptorSet> GetDescriptorSets();
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
		INVISION_API void CreateDescriptorSets(const SVulkan &vulkanInstance, VulkanDescriptorPool &pool);

	public:
		INVISION_API VulkanUniformBuffer();
		INVISION_API VulkanUniformBuffer& CreateUniformBinding(uint32_t binding,
			VkDescriptorType descriptorType,
			uint32_t descriptorCount,
			VkShaderStageFlags stageFlags,
			VkDeviceSize bufferSize,
			VkDeviceSize offset);
		INVISION_API void CreateUniformBuffer(const SVulkan &vulkanInstance, VulkanDescriptorPool &pool);
		INVISION_API void DestroyUniformBuffer(const SVulkan &vulkanInstance);
		INVISION_API VkDescriptorSetLayout GetDescriptorSetLayout();
		INVISION_API void UpdateUniform(const SVulkan &vulkanInstance, const void* source, size_t size, uint32_t binding);
		INVISION_API std::vector<VkDescriptorSet> GetDescriptorSets(uint32_t binding);
	};
}

#endif // VULKAN_UNIFORM_BUFFER_H