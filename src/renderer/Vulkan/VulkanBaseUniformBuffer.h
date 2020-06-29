#ifndef VULKAN_BASE_UNIFORM_BUFFER_H
#define VULKAN_BASE_UNIFORM_BUFFER_H

#include "vulkan\vulkan.h"
#include "VulkanBaseBuffer.h"
#include "VulkanBaseDescriptorPool.h"

namespace Invision
{
	class VulkanBaseUniformBinding
	{
		private:
			uint32_t mBinding;
			VkDescriptorType mDescriptorType;
			uint32_t mDescriptorCount;
			VkShaderStageFlags mStageFlags;
			VkDeviceSize mBufferSize;
			VkDeviceSize mOffset;
			std::vector<VulkanBaseBuffer> mUniformBuffer;
			std::vector<VkDescriptorSet> mDescriptorSets;

		public:
			INVISION_API VulkanBaseUniformBinding(uint32_t binding,
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
			INVISION_API void SetBuffers(std::vector<VulkanBaseBuffer> uniformBuffer);
			INVISION_API std::vector<VulkanBaseBuffer> GetBuffers();
			INVISION_API void ClearAndDestroyBuffers(const SVulkanBase &vulkanInstance);
			INVISION_API void SetDescriptorSets(std::vector<VkDescriptorSet> descriptorSets);
			INVISION_API std::vector<VkDescriptorSet> GetDescriptorSets();
	};

	// Binding Description
	class VulkanBaseUniformBuffer
	{
	private:
		std::vector<VulkanBaseUniformBinding> bindings;
		VkDescriptorSetLayout mDescriptorSetLayout;
		VulkanBaseDescriptorPool mDescriptorPool;

		INVISION_API void CreateUniformSet(const SVulkanBase &vulkanInstance);
		INVISION_API void CreateBuffers(const SVulkanBase &vulkanInstance);
		INVISION_API void DestroyUniformSet(const SVulkanBase &vulkanInstance);
		INVISION_API void CreateDescriptorSets(const SVulkanBase &vulkanInstance);

	public:
		INVISION_API VulkanBaseUniformBuffer();
		INVISION_API VulkanBaseUniformBuffer& CreateUniformBinding(uint32_t binding,
			VkDescriptorType descriptorType,
			uint32_t descriptorCount,
			VkShaderStageFlags stageFlags,
			VkDeviceSize bufferSize,
			VkDeviceSize offset);
		INVISION_API void CreateUniformBuffer(const SVulkanBase &vulkanInstance);
		INVISION_API void DestroyUniformBuffer(const SVulkanBase &vulkanInstance);
		INVISION_API VkDescriptorSetLayout GetDescriptorSetLayout();
		INVISION_API void UpdateUniform(const SVulkanBase &vulkanInstance, const void* source, size_t size, uint32_t binding);
		INVISION_API std::vector<VkDescriptorSet> GetDescriptorSets(uint32_t binding);
		INVISION_API std::vector<VkDescriptorSet> GetDescriptorSetsByIndex(uint32_t index);
		INVISION_API size_t GetSizeOfBindings();
	};
}

#endif // VULKAN_BASE_UNIFORM_BUFFER_H