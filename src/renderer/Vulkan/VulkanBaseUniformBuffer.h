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
			uint32_t mSetIndex;
			VkDescriptorType mDescriptorType;
			uint32_t mDescriptorCount;
			VkShaderStageFlags mStageFlags;
			VkDeviceSize mBufferSize;
			VkDeviceSize mOffset;
			VulkanBaseBuffer mUniformBuffer;
			VkDescriptorImageInfo mImageInfo;

		public:
			INVISION_API VulkanBaseUniformBinding(uint32_t binding,
				VkDescriptorType descriptorType,
				uint32_t descriptorCount,
				VkShaderStageFlags stageFlags,
				VkDeviceSize bufferSize,
				VkDeviceSize offset,
				VkImageView imageView,
				VkSampler sampler);

			INVISION_API uint32_t GetBinding();
			INVISION_API VkDescriptorType GetDescriptorType();
			INVISION_API uint32_t GetDescriptorCount();
			INVISION_API VkShaderStageFlags GetStageFlags();
			INVISION_API VkDeviceSize GetBufferSize();
			INVISION_API VkDeviceSize GetOffset();
			INVISION_API void  SetSetIndex(uint32 setIndex);
			INVISION_API VkDeviceSize GetSetIndex();
			INVISION_API void SetBaseBuffer(VulkanBaseBuffer uniformBuffer);
			INVISION_API VulkanBaseBuffer GetBaseBuffer();
			INVISION_API void ClearAndDestroyBuffers(const SVulkanBase &vulkanInstance);
			INVISION_API VkDescriptorImageInfo GetImageInfo();
	};

	// Binding Description
	class VulkanBaseUniformBuffer
	{
	private:
		std::vector<VulkanBaseUniformBinding> bindings;
		VkDescriptorSetLayout mDescriptorSetLayout; // there is actual one supportes by Invision
		VulkanBaseDescriptorPool mDescriptorPool;
		std::vector<VkDescriptorSet> mDescriptorSets;// there is actual one supportes by Invision

		INVISION_API void CreateUniformSet(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext);
		INVISION_API void CreateBuffers(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext);
		INVISION_API void DestroyUniformSet(const SVulkanBase &vulkanInstance);
		INVISION_API void CreateDescriptorSets(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext);

	public:
		INVISION_API VulkanBaseUniformBuffer();
		INVISION_API VulkanBaseUniformBuffer& CreateUniformBinding(uint32_t binding,
			VkDescriptorType descriptorType,
			uint32_t descriptorCount,
			VkShaderStageFlags stageFlags,
			VkDeviceSize bufferSize,
			VkDeviceSize offset);

		INVISION_API VulkanBaseUniformBuffer& CreateImageBinding(uint32_t binding,
			VkDescriptorType descriptorType,
			uint32_t descriptorCount,
			VkShaderStageFlags stageFlags,
			VkImageView imageView,
			VkSampler sampler);

		INVISION_API void CreateUniformBuffer(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext);
		INVISION_API void DestroyUniformBuffer(const SVulkanBase &vulkanInstance);
		INVISION_API VkDescriptorSetLayout GetDescriptorSetLayout();
		INVISION_API void UpdateUniform(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext, const void* source, size_t size, uint32_t binding);
		INVISION_API std::vector<VkDescriptorSet> GetDescriptorSets();
		INVISION_API size_t GetSizeOfBindings();
	};
}

#endif // VULKAN_BASE_UNIFORM_BUFFER_H