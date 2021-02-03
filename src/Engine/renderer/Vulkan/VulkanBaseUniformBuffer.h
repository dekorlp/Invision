#ifndef VULKAN_BASE_UNIFORM_BUFFER_H
#define VULKAN_BASE_UNIFORM_BUFFER_H

#include "vulkan\vulkan.h"
#include "VulkanBaseBuffer.h"
#include "VulkanBaseDescriptorPool.h"

namespace Invision
{
	class VulkanBaseMemoryManager;

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
			//VulkanBaseBuffer mUniformBuffer;
			void* mBufferHandle;
			VkDescriptorImageInfo mImageInfo;
			VkDescriptorBufferInfo mBufferInfo;
			

		public:
			INVISION_API VulkanBaseUniformBinding(uint32_t set, uint32_t binding,
				VkDescriptorType descriptorType,
				uint32_t descriptorCount,
				VkShaderStageFlags stageFlags,
				VkDeviceSize bufferSize,
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
			INVISION_API void CreateBaseBuffer(const SVulkanBase &vulkanInstance, VulkanBaseMemoryManager& memoryManager);
			INVISION_API VkBuffer GetBuffer(VulkanBaseMemoryManager& memoryManager);
			INVISION_API void* GetBufferHandle(VulkanBaseMemoryManager& memoryManager);
			INVISION_API void ClearAndDestroyBuffers(const SVulkanBase &vulkanInstance, VulkanBaseMemoryManager& memoryManager);
			INVISION_API VkDescriptorImageInfo& GetImageInfo();
			INVISION_API VkDescriptorBufferInfo& GetBufferInfo();
	};

	// Binding Description
	class VulkanBaseUniformBuffer
	{
	private:

		struct SetProperties
		{
		public:
			VkDescriptorSetLayout mDescriptorSetLayout;
			VulkanBaseDescriptorPool mDescriptorPool;
			VkDescriptorSet mDescriptorSet;
		};

		std::vector<VulkanBaseUniformBinding> bindings;
		//std::vector<VkDescriptorSetLayout> mDescriptorSetLayout; // there is actual one supportes by Invision
		//std::vector<VulkanBaseDescriptorPool> mDescriptorPool;
		//std::vector<VkDescriptorSet> mDescriptorSets;// there is actual one supportes by Invision
		std::vector<SetProperties> mSets;

		INVISION_API void CreateUniformSet(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext);
		INVISION_API void CreateBuffers(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext, VulkanBaseMemoryManager& memoryManager);
		INVISION_API void DestroyUniformSet(const SVulkanBase &vulkanInstance, VulkanBaseMemoryManager& memoryManager);
		INVISION_API void CreateDescriptorSets(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext); 

		uint32_t maxSet;

	public:
		INVISION_API VulkanBaseUniformBuffer();
		INVISION_API VulkanBaseUniformBuffer& CreateUniformBinding(uint32_t set, uint32_t binding,
			VkDescriptorType descriptorType,
			uint32_t descriptorCount,
			VkShaderStageFlags stageFlags,
			VkDeviceSize bufferSize);

		INVISION_API VulkanBaseUniformBuffer& CreateImageBinding(uint32_t set, uint32_t binding,
			VkDescriptorType descriptorType,
			uint32_t descriptorCount,
			VkShaderStageFlags stageFlags,
			VkImageView imageView,
			VkSampler sampler);

		INVISION_API void CreateUniformBuffer(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext, VulkanBaseMemoryManager& memoryManager);
		INVISION_API void DestroyUniformBuffer(const SVulkanBase &vulkanInstance, VulkanBaseMemoryManager& memoryManager);
		//INVISION_API std::vector<VkDescriptorSetLayout> GetDescriptorSetLayout();
		INVISION_API void UpdateUniform(const SVulkanBase &vulkanInstance, const SVulkanContext &vulkanContext, VulkanBaseMemoryManager& memoryManager, const void* source, size_t size, uint32_t set, uint32_t binding);
		INVISION_API std::vector<SetProperties> GetSets();
		INVISION_API size_t GetSizeOfBindings();
	};
}

#endif // VULKAN_BASE_UNIFORM_BUFFER_H